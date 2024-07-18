#ifndef mashy_lib_panel_handler_std_H_INCLUDED
#define mashy_lib_panel_handler_std_H_INCLUDED
#include "12d/set_ups.h"
#define COMMAND_PARSER_NO_FLAGS 0 // use to echo the command only, good way to test command coverage
#define COMMAND_PARSER_NO_EXECUTE 1 // use to echo the command only, good way to test command coverage

Integer include_split_by_tab(Text command, Text &word1, Text &balance_right_after_tab){
    word1 = ""; balance_right_after_tab = "";
    Dynamic_Text parts;
    From_text(command,'\"','\t',parts);
    Get_item(parts,1,word1);
    Integer n_parts;
    Get_number_of_items(parts,n_parts);
    if(n_parts>1)    balance_right_after_tab = Get_subtext(command,Text_length(word1)+2,Text_length(command));
    return n_parts;
}

Integer include_check_for_panel_change(Text cmd, Text &new_panel_level){ // panel\tpanel_path means to switch panels... TODO - this will do a refresh also.. intended behaviour? otherwise need to check agains current panel level'
    Text word1,balance_right_after_tab;
    Integer parts = include_split_by_tab(cmd, word1, balance_right_after_tab);
    if(Text_lower(word1) == "panel"){
        new_panel_level = balance_right_after_tab;
        return 1;
    }
    return 0;
}

Text panel_command_parser(Text command, Integer flags); // PROTOTYPE ONLY, DEFINE COMMANDS IN SPECIFIC MACRO

Text include_panel_now(Text &title, Integer &pos_x, Integer &pos_y, Text button_label[], Text button_command[], Integer size_b, 
                            Text standard_label[], Text standard_command[], Integer size_s, Integer n_columns, 
                            Integer button_x, Integer button_y){
    if( (pos_x == -1) || (pos_y == -1) ) Get_cursor_position(pos_x,pos_y);
    Text return_text="";
    Panel panel = Create_panel(title,1); // TODO - resize
    if(n_columns<1)n_columns=1;
    Integer n_hg_top = ((size_b - 1) / n_columns )+ 1;
    Integer n_hg_bot = ((size_s - 1) / n_columns )+ 1;
    Vertical_Group vg_all = Create_vertical_group(ALL_WIDGETS_OWN_HEIGHT);
    Append(vg_all,panel);
    Horizontal_Group hg_top[n_hg_top];
    Horizontal_Group hg_bot[n_hg_bot];

    for(Integer i=1;i<=n_hg_top;i++){
        hg_top[i] = Create_horizontal_group(ALL_WIDGETS_OWN_WIDTH);
        Set_sizing_constraints(hg_top[i],2,2);
        Append(hg_top[i],vg_all);
    }
    for(i=1;i<=n_hg_bot;i++){
        hg_bot[i] = Create_horizontal_group(ALL_WIDGETS_OWN_WIDTH);
        Set_sizing_constraints(hg_bot[i],2,2);
        Append(hg_bot[i],vg_all);
    }
    //TODO handle errors...
    if( (size_b < 1) || (size_s < 1)){
        Print("NUKED: due to array size less than 1\n");
        return "Panel Quit";
    }
    Integer all_ids[size_b + size_s];
    Integer id_count = 0;

    Button button_top[size_b];
    Integer hg_count = 0;
    for(i=1;i<=size_b;i++){
        button_top[i]=Create_button(button_label[i],button_command[i]);
        Set_sizing_constraints(button_top[i],2,2);
        if(((i-1)%n_columns == 0))hg_count++;
        Append(button_top[i],hg_top[hg_count]); // hardcoded! - TODO
        all_ids[++id_count] = Get_id(button_top[i]);
    }

    Button button_bot[size_s];
    hg_count = 0;
    for(i=1;i<=size_s;i++){
        button_bot[i]=Create_button(standard_label[i],standard_command[i]);
        Set_sizing_constraints(button_bot[i],2,2);
        if(((i-1)%n_columns == 0))hg_count++;
        Append(button_bot[i],hg_bot[hg_count]); // hardcoded! - TODO
        all_ids[++id_count] = Get_id(button_bot[i]);
    }

    // Button hidden_button_top = Create_button("hidden top","hidden top");    // still need a way to best handle zero sized arrays coming in - TODO
    // Button hidden_button_bot = Create_button("hidden top","hidden top");

    Show_widget(panel,pos_x,pos_y);
    Integer doit = 1;
    Integer flags = COMMAND_PARSER_NO_FLAGS ; // TODO - handle this better
    while (doit) {
        Integer id; Text cmd, msg;
        Wait_on_widgets(id,cmd,msg);
		if(cmd == "keystroke")		continue;
		//if(cmd == "Panel About")	manage_about_panel();   //TODO - include file
		if(cmd == "Panel Quit")		return (cmd);
		if(cmd == "CodeShutdown")	Set_exit_code(cmd);
        for(i=1;i<=id_count;i++){
            if(id == all_ids[i]){
                Print("DEBUG: Button id matched, running cmd <"+cmd+">\n"); // DEBUG - TODO - REMOVE
                Get_widget_position(panel,pos_x,pos_y);
                Text garbage;
                if(include_check_for_panel_change(cmd,garbage))return cmd; // if changing panels just exit
                return_text = panel_command_parser(cmd, flags);             // FLOW CONTROL RETURNS TO THE CUSTOM COMMAND PARSER IN THE MAIN MACRO
                break;
            }
        }

    }
    return return_text;
}

void inlcude_parse_buttons(Text buttons[], Text button_name[], Text panel_level[], Text command[] , Integer s1){
    for(Integer i=1;i<=s1;i++){
        Dynamic_Text parts1;
        From_text(buttons[i],'\"','\t',parts1); // split by tab ; TOOD - this should only split by first tab, more work to do later
        Text t;
        Get_item(parts1,1,t);
        Dynamic_Text parts2;
        From_text(t,'\"','\\',parts2); // for the first part, split by \backslash
        Integer n_parts1,n_parts2;
        Get_number_of_items(parts1,n_parts1);
        Get_number_of_items(parts2,n_parts2);
        if(n_parts2>1){
            for(Integer j=Text_length(t);j>0;j--){
                Integer c;
                Get_char(t,j,c);
                if(c == '\\'){
                    button_name[i] = Get_subtext(t,j+1,Text_length(t));
                    panel_level[i] = Get_subtext(t,1,j-1);
                    break;
                }
            }
        }else{
            button_name[i] = t;
            panel_level[i] = "";
        }
        for(Integer j=2;j<=n_parts1;j++){ // rebuild the command into command[i]
            Get_item(parts1,j,t);
            if(j>2)command[i]+="\t"; //TODO - this is inneficient from the first From_text();
            command[i]+=t;
        }
    }
}

void include_handle_custom_panel(Text title, Text buttons1[], Integer s1, Text buttons2[], Integer n_buttons2, Integer horizontal_button_columns, Integer button_x, Integer button_y){
    Text panel_level[s1];
    Text button_name[s1];
    Text command[s1];
    inlcude_parse_buttons(buttons1,button_name,panel_level,command,s1);

    Text button_names2[n_buttons2];
    Text panel_levels2[n_buttons2]; // not used
    Text button_commands2[n_buttons2];
    inlcude_parse_buttons(buttons2,button_names2,panel_levels2,button_commands2,n_buttons2);

    Integer pos_x=-1,pos_y=-1;
    Text current_panel_level = "";
    Text return_text="";
    do {
        Text this_buttons[s1];
        Text this_commands[s1];
        Integer this_button_count = 0;
        for(Integer i=1;i<=s1;i++){
            if(panel_level[i] == current_panel_level){
                this_button_count++;
                this_buttons[this_button_count] = button_name[i];
                this_commands[this_button_count] = command[i];
            }
        }
        if(!this_button_count){
            current_panel_level = ""; //reset the panel level
            continue;
        }
        return_text = include_panel_now(title, pos_x, pos_y, this_buttons, this_commands, this_button_count, button_names2, button_commands2, n_buttons2, horizontal_button_columns, button_x, button_y);
        Text possible_new_panel_name;
        if(include_check_for_panel_change(return_text,possible_new_panel_name)) current_panel_level = possible_new_panel_name; // check to change panel level
    }while (return_text != "Panel Quit");
}

#endif