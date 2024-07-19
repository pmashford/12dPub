#ifndef mashy_lib_widgets_panel_handler_std_H_INCLUDED
#define mashy_lib_widgets_panel_handler_std_H_INCLUDED
#include "12d/set_ups.h"
#include "mashy_lib_about_panel.h"
#include "mashy_lib_files.H"
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


Text include_panel_hide(){
    Integer x,y;    Get_cursor_position(x,y);
    Panel panel = Create_panel("");
    Append(Create_button("  Restore  ","restore"),panel);
    Show_widget(panel,9000,9000);
    Set_cursor_position(x,y);
    while (1) {
        Integer id; Text cmd, msg;
        Wait_on_widgets(id,cmd,msg);
		if(cmd == "restore")    return(cmd);
        if(cmd == "keystroke")		continue;
		if(cmd == "Panel Quit")		return (cmd); // TODO - make this functional back in the caller function
		if(cmd == "CodeShutdown")	Set_exit_code(cmd);
    }
    return "";
}


Text panel_command_parser(Text command, Integer flags); // PROTOTYPE ONLY, DEFINE COMMANDS IN SPECIFIC MACRO

// TODO - need a way to left justify the buttons text

// mode == 0, contol flows to panel_command_parser()
// mode == 1, the selected buttons command is returned as text
Text include_panel_now(Integer mode, Text title, Integer &pos_x, Integer &pos_y, Text button_label[], Text button_command[], Integer size_b, 
                            Text standard_label[], Text standard_command[], Integer size_s, Integer n_columns, Integer &autohide_tick_state, 
                            Integer button_x, Integer button_y){
    if( (pos_x == -1) || (pos_y == -1) ) Get_cursor_position(pos_x,pos_y);
    Text return_text="";
    Panel panel = Create_panel(title,1); // TODO - resize
    if(n_columns<1)n_columns=1;
    Integer n_hg_top = ((size_b - 1) / n_columns )+ 1;
    Integer n_hg_bot = ((size_s - 1) / n_columns )+ 1;
    Vertical_Group vg_all = Create_vertical_group(ALL_WIDGETS_OWN_HEIGHT);
    Append(vg_all,panel);
    Vertical_Group vg_bot = Create_vertical_group(ALL_WIDGETS_OWN_HEIGHT);
    Set_border(vg_bot,"-");
    Horizontal_Group hg_top[n_hg_top];
    Horizontal_Group hg_bot[n_hg_bot];

    for(Integer i=1;i<=n_hg_top;i++){
        hg_top[i] = Create_horizontal_group(ALL_WIDGETS_OWN_WIDTH);
        Set_sizing_constraints(hg_top[i],2,2);
        Append(hg_top[i],vg_all);
    }
    Horizontal_Group hg_autohide = Create_horizontal_group(COMPRESS_WIDGETS_OVER_WIDTH); // ALL_WIDGETS_OWN_WIDTH
    Named_Tick_Box autohide_tick_box= Create_named_tick_box("Auto Hide",autohide_tick_state,"toggle autohide");   // TODO - remember state... it should be passed into the function... maybe 3 modes. 0= off, 1 = on, -ve dont expose?
    if(autohide_tick_state >= 0){
        Append(autohide_tick_box,hg_autohide);
        Append(hg_autohide,vg_all);
        //Set_data(autohide_tick_box,autohide_tick_state);
    }
    Set_width_in_chars(autohide_tick_box,10);   // TODO - get proper centre alignment... at the moment it is filling up HG space in width

    for(i=1;i<=n_hg_bot;i++){
        hg_bot[i] = Create_horizontal_group(ALL_WIDGETS_OWN_WIDTH);
        Set_sizing_constraints(hg_bot[i],2,2);
        Append(hg_bot[i],vg_bot);
    }
    Append(vg_bot,vg_all);

    Integer size_b_plus_s = size_b + size_s;
    if( (size_b_plus_s < 1) ){ // dont allow the main list to have a zero size!
        Print("NUKED: due to array size less than 1\n");
        return "Panel Quit";
    }
    Integer all_ids[size_b_plus_s];
    Integer id_count = 0;
    Integer hg_count = 0;
    if(size_b > 0){
        Button button_top[size_b];
        hg_count = 0;
        for(i=1;i<=size_b;i++){
            button_top[i]=Create_button(button_label[i],button_command[i]);
            Set_sizing_constraints(button_top[i],2,2);
            if(button_x>0)  Set_width_in_chars(button_top[i],button_x);
            if(((i-1)%n_columns == 0))hg_count++;
            Append(button_top[i],hg_top[hg_count]); 
            all_ids[++id_count] = Get_id(button_top[i]);
        }
    }

    if(size_s > 0){ 
        Button button_bot[size_s];
        hg_count = 0;
        for(i=1;i<=size_s;i++){
            button_bot[i]=Create_button(standard_label[i],standard_command[i]);
            Set_sizing_constraints(button_bot[i],2,2);
            if(button_x>0)  Set_width_in_chars(button_bot[i],button_x);
            if(((i-1)%n_columns == 0))hg_count++;
            Append(button_bot[i],hg_bot[hg_count]); 
            all_ids[++id_count] = Get_id(button_bot[i]);
        }
    }

    Show_widget(panel,pos_x,pos_y);
    Integer doit = 1;
    Integer flags = COMMAND_PARSER_NO_FLAGS ; // TODO - handle this better
    while (doit) {
        Integer id; Text cmd, msg;
        Wait_on_widgets(id,cmd,msg);
        // Print("id<"+To_text(id) +">, cmd<"+cmd+", msg<"+msg+">\n" );
		if(cmd == "keystroke")		continue;
		if(cmd == "Panel About")	manage_about_panel();
		if(cmd == "Panel Quit")		return (cmd);
		if(cmd == "CodeShutdown")	Set_exit_code(cmd);
        if(cmd == "Panel Hide")		return (cmd);
        for(i=1;i<=id_count;i++){
            if(id == all_ids[i]){
                // Print("DEBUG: Button id matched, running cmd <"+cmd+">\n"); // DEBUG - TODO - REMOVE
                Get_widget_position(panel,pos_x,pos_y);
                if(autohide_tick_state >= 0){ // -ve means the tick box is not on the panel
                    Validate(autohide_tick_box,autohide_tick_state);
                }

                Text garbage;
                if(include_check_for_panel_change(cmd,garbage))return cmd; // if changing panels just exit

                if(mode == 0){
                    return_text = panel_command_parser(cmd, flags);             // FLOW CONTROL GOES TO THE CUSTOM COMMAND PARSER IN THE MAIN MACRO
                    if(autohide_tick_state==1) return "Panel Hide";    // TODO - the panel command parser could want to set something different here, not sure this is too smart setting a fresh return value
                }

                if(mode == 1){
                    return cmd; // simply return the cmd, panel_display_list_get_reply() builds a list and this line retuns the selection
                }
                break;
            }
        }

    }
    return return_text;
}

Text panel_display_list_get_reply(Text title, Integer &pos_x, Integer &pos_y, Text button_label[], Integer size, Integer n_columns){
    Text button_command[size];
    Text dummy[1];
    Integer button_x = -1,button_y=20;  // TODO - do a sweep of the commands and check if it makes sense to set a width, -1 for now should let the widgets decide what to do
    Integer mode = 1; // this means the main function handling the panel will just return the command (we set the command here to be the button name)
    Integer autohide_tick_state = -1;
    for(Integer i=1;i<=size;i++) button_command[i] = button_label[i];
    return include_panel_now(mode, title, pos_x, pos_y, button_label, button_command, size, 
                            dummy, dummy, 0, n_columns, autohide_tick_state,
                            button_x, button_y);
}

Text panel_display_list_get_reply(Text title, Integer &pos_x, Integer &pos_y, Dynamic_Text &dt, Integer n_columns){
  Integer size;
  Get_number_of_items(dt,size);
  if(size<1)return"";
  Text button_label[size];
  for(Integer i=1;i<=size;i++)  Get_item(dt,i,button_label[i]);
  return panel_display_list_get_reply(title, pos_x, pos_y, button_label, size, n_columns);
}

void inlcude_parse_buttons(Text buttons[], Text button_name[], Text panel_level[], Text command[] , Integer s1){
    for(Integer i=1;i<=s1;i++){
        Dynamic_Text parts1;
        From_text(buttons[i],'\"','\t',parts1); // split by tab ; TOOD - this should only split by first tab, for now means more work to do later within this function
        Text t;
        Get_item(parts1,1,t);
        Dynamic_Text parts2;
        From_text(t,'\"','\\',parts2); // for the first part, split by \backslash
        Integer n_parts1,n_parts2;
        Get_number_of_items(parts1,n_parts1);
        Get_number_of_items(parts2,n_parts2);
        if(n_parts2 > 1){
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
            if(j > 2)command[i]+="\t"; //TODO - this is inneficient from the first From_text();
            command[i]+=t;
        }
    }
}

void handle_custom_panel(Text title, Text buttons_top[], Integer n_buttons_top, Text buttons_bot[], Integer n_buttons_bot, Integer n_button_columns, Integer &autohide_tick_state, Integer button_x, Integer button_y){
    Text panel_level[n_buttons_top];
    Text button_name[n_buttons_top];
    Text command[n_buttons_top];
    inlcude_parse_buttons(buttons_top,button_name,panel_level,command,n_buttons_top);

    Text button_names2[n_buttons_bot];
    Text panel_levels2[n_buttons_bot]; // not used
    Text button_commands2[n_buttons_bot];
    inlcude_parse_buttons(buttons_bot,button_names2,panel_levels2,button_commands2,n_buttons_bot);

    Integer pos_x=-1,pos_y=-1;
    Text current_panel_level = "";
    Text return_text="";
    do {
        Text child_title = current_panel_level;
        if(child_title == "") child_title = title;
        Text this_buttons[n_buttons_top];
        Text this_commands[n_buttons_top];
        Integer this_button_count = 0;
        for(Integer i=1;i<=n_buttons_top;i++){
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
        Integer mode = 0;
        return_text = include_panel_now(mode , child_title, pos_x, pos_y, this_buttons, this_commands, this_button_count, button_names2, button_commands2, n_buttons_bot, n_button_columns, autohide_tick_state, button_x, button_y);
        if(return_text == "Panel Hide") include_panel_hide();   // hide it?
        Text possible_new_panel_name;
        if(include_check_for_panel_change(return_text,possible_new_panel_name)) current_panel_level = possible_new_panel_name; // check to change panel level
    }while (return_text != "Panel Quit");
}

void handle_custom_panel(Text title, Dynamic_Text buttons_top_as_dt, Dynamic_Text buttons_bot_as_dt, Integer n_button_columns, Integer &autohide_tick_state, Integer button_x, Integer button_y){
    Integer n_buttons_top,n_buttons_bot;
    Get_number_of_items(buttons_top_as_dt,n_buttons_top);
    Get_number_of_items(buttons_bot_as_dt,n_buttons_bot);
    Integer ierr_n_buttons_top = 0 , ierr_n_buttons_bot = 0;
    if(n_buttons_top<1){ ierr_n_buttons_top = 1;  n_buttons_top = 1;  } // guard
    if(n_buttons_bot<1){ ierr_n_buttons_bot = 1;  n_buttons_bot = 1;  } // guard
    Text buttons_top[n_buttons_top];
    Text buttons_bot[n_buttons_bot];
    for(Integer i=1;i<=n_buttons_top;i++)   Get_item(buttons_top_as_dt,i,buttons_top[i]);
    for(        i=1;i<=n_buttons_bot;i++)   Get_item(buttons_bot_as_dt,i,buttons_bot[i]);
    if(ierr_n_buttons_top)buttons_top[1]="";
    if(ierr_n_buttons_bot)buttons_bot[1]="";
    return handle_custom_panel(title, buttons_top, n_buttons_top, buttons_bot, n_buttons_bot, n_button_columns, autohide_tick_state, button_x, button_y);
}

///////// SPEFICIC FUNCTIONS

// START AUX
void add_prefix_suffix(Dynamic_Text &dt, Text prefix, Text suffix){
  Integer items;  Get_number_of_items(dt,items);
  for(Integer i=1;i<=items;i++){
    Text t; Get_item(dt,i,t); Set_item(dt,i,prefix+t+suffix);
  }
}

Integer is_last_char(Text text_to_search, Integer c){
  Integer last_c;
  Get_char(text_to_search,Text_length(text_to_search),last_c);
  return c == last_c ;
}

// END AUX

Text panel_walk_dir(Text base, Text file_wildcard, Text quit_text_at_top_of_list, Integer &pos_x, Integer &pos_y){
  // for a base path walk it
  Text base_path = valid_path(base);
  Dynamic_Text dirs = get_list_of_folders(base_path,"*") , files = get_list_of_files(base_path,file_wildcard);
  add_prefix_suffix(dirs,"\\","\\");
  Dynamic_Text list;
  if(quit_text_at_top_of_list != ""){
    Append(quit_text_at_top_of_list,list);
    Append("",list);
  } 
  Append(files,list);
  Append("",list);
  Append("..\\",list);
  Append(dirs,list);
  //return menu_display_list_get_reply(list,file_wildcard,x,y);
  Integer n_columns = 1;
  Text title = "File walker";
  return panel_display_list_get_reply(title, pos_x, pos_y, list, n_columns);
}

// this will return a selected file name, but there are options to run it and also keep the panel open
Text panel_get_file(Text base_path, Text file_wildcard, Integer exit_on_selection, Integer execute_on_selection){
  Text base_in = valid_path(base_path);
  Text base = base_in;
  Text quit_text_at_top_of_list = " - Exit - ";
  if(exit_on_selection) quit_text_at_top_of_list = ""; // the menu will handle this
  Integer x=-1,y=-1;
  Text reply;
  do {
     Text msg;
     reply =   panel_walk_dir(base, file_wildcard, quit_text_at_top_of_list, x, y);
     if(reply=="")continue;
     if(reply=="Panel Quit")reply = quit_text_at_top_of_list;
     if(is_last_char(reply,'\\')){
      base = real_path(base + "\\" + reply);
      msg = ", is a folder";
      if( Text_length(base) <= Text_length(base_in) ){
        base = base_in;
      } 
     }else{
      msg = ", is a file";
      if(execute_on_selection){
        // handle filetypes here
        msg += ", and will be executed";
        Text this_file = valid_path(base+"\\"+reply);
        if(File_exists(this_file)){
          Text file_ext;
          get_extension(reply,file_ext);
          Print("("+file_ext+")\n");
          if(Text_lower(file_ext) == "4do"){
            Macro_Handle handle;
            Integer run_now = 1;
            //Create_macro("\""+this_file+"\"",run_now,handle); // Paul Hardwick email
            Create_macro("-no_console -close_on_exit \""+this_file+"\"",run_now); 
            //run_a_macro_via_chain (this_file);
          }else{
            run_system_command_no_wait("\""+this_file+"\"");
          } 
        }
      }
      if(exit_on_selection){
        Print(reply+msg+"\n");
        break;
      }
     }
     Print(reply+msg+"\n");
  } while(reply != quit_text_at_top_of_list);
  return reply;
}

// this will return the handle to a tin, null handle means the panel was exited (or the tin was deleted before a tin was selected)
Tin panel_get_tin_all_types(){
    Dynamic_Text list;
    Get_project_tins(list); // TODO - sorted?
    Integer pos_x=-1,pos_y=-1,n_columns=1;
    Text title = "Select a tin";
    Text reply = panel_display_list_get_reply(title, pos_x, pos_y, list, n_columns);
    return Get_tin(reply); // try and reply with a handle to the tin, it'll be null if not found
}

#endif