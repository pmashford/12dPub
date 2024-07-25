#define DEBUG_CHAIN
#include "mashy_lib_system_calls.H"
#include "mashy_lib_files.H"
#include "mashy_lib_panel_defaults.H"
#include "mashy_lib_about_panel.H"
#include "mashy_lib_text_functions.H"
#include "12d/set_ups.h"
#include "mashy_lib_widgets_panel_handler_std.H"
#include "mashy_lib_xml_chains.H"

// NOTES
// ([counts]\t[defaults]\t[page1]...)
// TODO - properly plumb in special characters \t ; : (and protect input boxes from breaking string format), maybe 1st page pointer also
// TODO - Sort by date? or day filter(-100 / +100 days) ... Text command = "forfiles /P \"" + basepath + "\"" + slash_s + wildcard + slash_d + " /C \"cmd /c if @isdir==FALSE echo @path >> " + quotes_add(rpt_file) ;
// DONE + blow aways settings if they somehow get in a dodgey state (partially done on new pages page)
// TODO - show OPW ?
// TODO - do HIDE ?
// TODO - [ Reveal ] | [ copy /?]
// TODO - reserve first position in the settings string for future... 1st position should hold panel height as a setting-> ([settings]\t[counts]\t[defaults]\t[page1]...)
// ???? - Text_Text_Multimap / Text_Map -> Integer_Text_Map, might complicate click actions for list_box? - nogo, no key lookup in critical path
// TODO - cleanup the list of $libs on the page_panel()
// TODO - quick select wildcard
// DONE + native 12d handling to [run: 4do,chain  ] [edit: chain]


Text handle_possible_multiple_wildcards(Text basepath, Text wildcard){  // "C:\x\y\*.4dm" "c:\b\a\*.4do" /s /b  vs "C:\temp\temp\*.4dm" /s /b
    wildcard = Text_justify(wildcard);
    wildcard = find_replace_char(wildcard,',',' '); // ensure space separated
    wildcard = find_replace_char(wildcard,';',' '); // ensure space separated
    Dynamic_Text words;
    From_text(wildcard,'"',' ',words);
    Integer count;  Get_number_of_items(words,count);
    if(!count) return "\"" + valid_path(basepath+"\\"+wildcard) + "\"";
    Text cmd;
    for(Integer i=1;i<=count;i++){
            Text t;
            Get_item(words,i,t);
            if(i!=1)cmd+=" ";
            cmd += "\"" + valid_path(basepath+"\\"+t) + "\"";
    }
    return cmd;
}

Dynamic_Text get_files(Text basepath, Text wildcard, Integer is_recursive){
    Text command = "dir " +handle_possible_multiple_wildcards(basepath, wildcard) + " /ogn /a-d"; // Handles multiple wildcards safely *.4dm,*.4do  OR *.4dm *.4do
    //Text command = "dir \"" + valid_path(basepath+"\\"+wildcard) + "\" /ogn /a-d"; // SAFE - this only handled 1 wildcard
    if(is_recursive) command += " /s";
    command += " /b";
    // Print(command+"\n");
    return run_system_command_get_output(command);
}

// TODO - days old here and interface
Integer get_files(Text basepath, Text wildcard, Integer is_recursive, Integer days_old, Dynamic_Text &filename, Dynamic_Text &path_to_filename){
	Null(filename);
    Null(path_to_filename);
    //Dynamic_Text list = get_files(basepath, wildcard, is_recursive, days_old);
    Dynamic_Text list = get_files(basepath, wildcard, is_recursive);
	Integer n_items;
	Get_number_of_items(list,n_items);
	for(Integer i=1;i<=n_items;i++){
		Text t,fname,path;
		Get_item(list,i,t);
        get_file_path_filename(t,path, fname);
        Append(fname,filename);
        if(is_recursive) Append(path,path_to_filename);
        else Append(basepath,path_to_filename);  // fixed bug here where no /s doesn't echo full path
	}
	return n_items;
}

void wait_on_panel_hide(){}

void clear(List_Box &b){
    Integer n;  Get_number_of_items(b,n);
    for(Integer i=n;i>0;i--)    Delete_item(b,i);
}

Integer rebuild(List_Box &box, Text filter_pattern, Integer order_assending, Dynamic_Text &files_in, Dynamic_Text &paths_in, Dynamic_Text &files_out, Dynamic_Text &paths_out ){
    clear(box);
    Null(files_out); Null(paths_out);
    Integer items;
    Get_number_of_items(files_in,items);
    if(items < 1) return 1;
    filter_pattern = Text_lower(filter_pattern);
    Integer filtered_size=0;
    Text filtered_file[items];
    Text filtered_path[items];
    Integer sort_index[items];
    for(Integer i=1;i<=items;i++){
        Text this_name;
        Get_item(files_in,i,this_name);
        if(Match_name(Text_lower(this_name),filter_pattern)){
            filtered_size++;
            filtered_file[filtered_size] = this_name; // this is the filename
            Get_item(paths_in,i,this_name);
            filtered_path[filtered_size] = this_name; // this is sort of a pointer ->use: Get_item(path_to_filename,filtered_path_idx[1],path);
        }
    }
    Quick_sort(filtered_size,sort_index,filtered_file);
    if(order_assending){
        for(i=1;i<=filtered_size;i++){
            Add_item(box,filtered_file[sort_index[i]]);
            Append(filtered_file[sort_index[i]],files_out); // this may not be needed?
            Append(filtered_path[sort_index[i]],paths_out);
        }
    }else{
        for(i=filtered_size;i>0;i--){
            Add_item(box,filtered_file[sort_index[i]]);
            Append(filtered_file[sort_index[i]],files_out); // this may not be needed?
            Append(filtered_path[sort_index[i]],paths_out);
        }
    }
    return 0;
}

 Integer get_filename_check_exists(Text msg, Dynamic_Text &lb_files, Dynamic_Text &lb_paths, Text &path_file_ext, Text &ext){
    Integer idx;    From_text(msg,idx);
    Text this_file, this_path;
    Get_item(lb_files,idx,this_file);
    Get_item(lb_paths,idx,this_path);
    path_file_ext = valid_path(this_path+"\\"+this_file);
    get_extension(path_file_ext,ext);
    ext = Text_lower(ext);  // TODO - do it in get_extension()!
    Print("INSIDE, f<" + this_file +"> p<"+ this_path +"> path:<"+path_file_ext +"> ext:<" +ext+">\n");
    return File_exists(path_file_ext);
}

Text valid_filter_string(Text t){
    if(Get_subtext(t,1,1) != "*") t = "*" + t;
    if(Get_subtext(t,Text_length(t),Text_length(t)) != "*") t+="*";
    return t;
}

void toggle_0_1(Integer &mode){
    mode = (mode) ? 0 : 1;
}


// consistancy in the defaults file (ddxm)
#define PANEL_NAME "Search n Launch"
#define HIDDEN_SETTINGS_NAME "settings"

/// /// /// /// /// /// /// /// /// panel helper helpers for pages

Text make_page(Text t1, Text t2, Text t3, Integer i4, Integer i5){
    return t1 + ";" + t2 + ";" + t3 + ";" + To_text(i4) + ";" + To_text(i5);
}

Dynamic_Text get_parts(Text word)                     { Dynamic_Text parts;    From_text(word,'"',';',parts);    return parts;}
Dynamic_Text get_words(Text data)                     { Dynamic_Text words;    From_text(data,'"','\t',words);    return words;   }
//Text         get_item(Dynamic_Text &list, Integer idx){ Text t;                Get_item(list,idx,t);   return t; }
Text         get_word(Text data, Integer idx)         { Dynamic_Text words = get_words(data);    return get_item(words,idx);  }
Text         get_part(Text word, Integer idx)         { Dynamic_Text parts = get_parts(word);
    Text t = get_item(parts,idx);
    if(t == ":")t="";
    return   t;}


void get_counts(Text data, Integer &active_set, Integer &n_sets){
    Text t = get_word(data,1);
    Dynamic_Text parts = get_parts(t);
    Text t1,t2;
    Get_item(parts,1,t1);
    Get_item(parts,2,t2);
    From_text(t1,active_set);
    From_text(t2,n_sets);
    if(n_sets<1)n_sets = 1;
    if(active_set<1)active_set =1;
    if(active_set>n_sets)active_set = n_sets;
}

Text set_counts(Text data, Integer active_set, Integer n_sets){ // returns full data
    Text t = get_word(data,1);
    data = Get_subtext(data,Text_length(t)+1,Text_length(data)); // truncate keeping first tab
    if(active_set<1)active_set = n_sets;
    if(active_set>n_sets)active_set = 1;
    return To_text(active_set)+";"+To_text(n_sets)+data;
}


Text valid_states(Text data, Text default_page){    // this makes sure everything is sweet in the settings string / state
    Dynamic_Text words = get_words(data);
    Integer items;  Get_number_of_items(words,items);
    Dynamic_Text default_parts = get_parts(default_page);
    Integer d_items;    Get_number_of_items(default_parts,d_items);
    Integer active_set, n_sets;
    get_counts(data, active_set, n_sets);
    n_sets = 0; //restart
    Text string = "";
    for(Integer i=3;i<=items;i++){
        n_sets++;
        string+="\t";
        Text page = get_item(words,i);
        Dynamic_Text parts = get_parts(page);
        for(Integer j=1;j<=d_items;j++){
            Text t = get_item(parts,j);
            if(j>1)string+=";";
            if(t == "")string+=get_item(default_parts,j);
            else string+=t;
        }
    }
    if(active_set>n_sets)active_set = n_sets;
    return To_text(active_set)+ ";" + To_text(n_sets) + "\t" + default_page + string;
}

Text get_state(Input_Box box, Text default_page){   // this gets the state and makes sure everything is valid
    Text data;
    Integer ierr = Get_data(box,data);
    if(ierr || data == ""){                                              //|  idx1  | idx2    | idx3...
        data =  "1;1\t"+default_page+"\t"+default_page; // most primative  | counts | default | page1 |
    }
    data = valid_states(data, default_page);    // TODO - should be checking for n;n\t as a minimum
    return data;
}

Integer set_state(Text data, Input_Box &box){
    return Set_data(box,data);
}

Text get_page(Text data, Integer active_set){
    Dynamic_Text list = get_words(data);
    return get_item(list,active_set+2);
}

Text get_default(Text data){
    Dynamic_Text list = get_words(data);
    return get_item(list,2);
}

Text add_page(Text data){       // this will add a page to the end, based on whatever the default page is, the counts are set to the new page
    Integer active_set,n_sets;
    get_counts(data, active_set, n_sets);
    n_sets++;
    active_set = n_sets;
    data+="\t"+ get_default(data);
    return set_counts(data,active_set,n_sets);
}

Text reset_pages_from_default(Text data){
    Text default_page = get_default( data);
    return "1;1\t"+default_page+"\t"+default_page;
}

Text remove_page(Text data, Integer page_to_remove){
    Integer active_set,n_sets;
    get_counts(data, active_set, n_sets);
    Dynamic_Text list = get_words(data);
    Integer n_list; Get_number_of_items(list,n_list);
    data = get_item(list,1) + "\t" + get_item(list,2) ;
    if(n_sets == 1)return data + "\t" + get_item(list,2); // make page 1 the default
    Integer count=0;
    for(Integer i=3;i<=n_list;i++){
        if(i-2 == page_to_remove)continue;
        data+="\t" + get_item(list,i);
    }
    n_sets--;
    if(active_set>n_sets)active_set=n_sets;

    return set_counts(data, active_set, n_sets);
}

Text set_page(Text data, Integer active_set, Text page){
    Dynamic_Text list = get_words(data);
    data = "";
    Integer items;  Get_number_of_items(list,items);
    active_set+=2;
    for(Integer i=1;i<=items;i++){
        if(i>1)data+="\t";
        if(i==active_set)data+=page;
        else data+=get_item(list,i);
    }
    return data;
}

Text set_part(Text page, Integer idx, Text value){
    Dynamic_Text list = get_parts(page);
    page = "";
    Integer items;  Get_number_of_items(list,items);
    if(value=="")value=":"; // null holder
    for(Integer i=1;i<=items;i++){
        if(i>1)page+=";";
        if(i==idx){
            page+=value;
        }else{
            page+=get_item(list,i);
        }
    }
    return page;
}

Text set_part(Text data, Integer active_set, Integer idx, Text value){
    Text page = get_page(data,active_set);
    // Print("a:"+page+"\n");
    page = set_part(page,idx,value);
    // Print("b:"+page+"\n");
    return set_page(data,active_set,page);
}


Integer to_int(Text t){
    Integer i;
    From_text(t,i);
    return i;
}
/// /// /// /// /// /// /// /// /// panel helpers



Text get_basepath(Text data, Integer active_set)              {     return get_part(get_page(data,active_set), 1);}            // returns the field
Text set_basepath(Text data, Integer active_set, Text value)  {    return set_part(data,active_set,           1,value);      }  // returns the whole data structure with the field updated
Text get_wildcard(Text data, Integer active_set)              {    return get_part(get_page(data,active_set), 2);  }
Text set_wildcard(Text data, Integer active_set, Text value)  {    return set_part(data,active_set,           2,value);      }
Text get_filter(Text data, Integer active_set)              {    return get_part(get_page(data,active_set),   3);  }
Text set_filter(Text data, Integer active_set, Text value)  {    return set_part(data,active_set,             3,value);      }
Integer get_ascending(Text data, Integer active_set)              {    return to_int(get_part(get_page(data,active_set),   4));  }
Text    set_ascending(Text data, Integer active_set, Integer value)  {    return set_part(data,active_set,                 4,To_text(value));      }
Integer get_recursive(Text data, Integer active_set)              {    return to_int(get_part(get_page(data,active_set),   5));  }
Text    set_recursive(Text data, Integer active_set, Integer value)  {    return set_part(data,active_set,                 5,To_text(value));      }

/// /// /// /// /// /// /// /// /// 
{Text horrible_hack;}

Text panel_command_parser(Text command, Integer flags){
// flags might be a way of testing that a command is executable without actually executing
    //Print(command+"\n");
    Text return_text = "";
    Text this_command, this_command_balance_right_after_tab;
    include_split_by_tab(command, this_command, this_command_balance_right_after_tab);

    Integer active_set,n_sets;
    get_counts(horrible_hack, active_set, n_sets); // this could be used by multiple

    switch (Text_lower(this_command)){
        case ("continue") :{
            Print("continue\n"); // DEBUG - TODO - REMOVE
        }break;

        case ("active") :{
            Text idx_t; Integer idx;    From_text(this_command_balance_right_after_tab, idx);
            horrible_hack = set_counts(horrible_hack, idx,n_sets);
            return "Panel Quit";
        }break;

        case ("remove") :{
            Text idx_t; Integer idx;    From_text(this_command_balance_right_after_tab, idx);
            horrible_hack = remove_page(horrible_hack, idx);
            return "Panel Quit";
        }break;
        case ("new") :{ 
            horrible_hack = add_page(horrible_hack);
            return "Panel Quit";
        }break;
        case ("add") :{ 
            horrible_hack = add_page(horrible_hack);
            get_counts(horrible_hack, active_set, n_sets);
            horrible_hack = set_basepath(horrible_hack,n_sets,this_command_balance_right_after_tab);
            return "Panel Quit";
        }break;

        case ("rebuild") :{ 
            horrible_hack = reset_pages_from_default(horrible_hack);
            return "Panel Quit";
        }break;
        default :{
            Print("\nERROR: ("+this_command+") not found! - full text passed in <"+command+">\n");
        }
    }
    return return_text;
}


//// TODO - WIP to panel
Text part_panel(Text data){ // pass in the text, the text builds the panel, and the response of the panel rebuilds the text.
    Integer active_set, n_sets;
    get_counts(data, active_set, n_sets);
    Dynamic_Text top_buttons;
    for(Integer i=1;i<=n_sets;i++){
        Text t = find_replace_char(get_basepath(data,i),'\\','/') + "\" (" + get_wildcard(data,i) + ")";
        Append("Make Active ->     "+t+"\tactive\t"+To_text(i),top_buttons);
        Append("Remove ->     "+t+"\tremove\t"+To_text(i),top_buttons);
    }
    Dynamic_Text bot_buttons;
    Append("Add new -> $CUSTOMER_LIB\tadd\t$CUSTOMER_LIB",bot_buttons);    Append("Add new -> $CUSTOMER_USER\tadd\t$CUSTOMER_USER",bot_buttons);
    Append("Add new -> $USER_LIB\tadd\t$USER_LIB",bot_buttons);    Append("Add new -> $USER\tadd\t$USER",bot_buttons);
    Append("Add new -> $LIB\tadd\t$LIB",bot_buttons);               Append("Add new -> $SET_UPS\tadd\t$SET_UPS",bot_buttons);
    Append("NEW -> Add 'blank' page to end\tnew",bot_buttons);    Append("REBUILD -> Reset to 1 default page)\trebuild",bot_buttons); //Append("Exit\texit",buttons);

    horrible_hack = data;
    Integer n_button_columns = 2,  button_width= -2, button_height = 40;
    Text title = "Pages Configs";
    Integer autohide_tick_state = -1; // negative to turn it off totally
    handle_custom_panel(title, top_buttons, bot_buttons, n_button_columns,  autohide_tick_state, button_width, button_height);
    data = horrible_hack;
    horrible_hack = "";
    return data;
}


//////////////


// panel messages helper - TODO - expand for chains
void set_message(Message_Box message_box, Text source_file, Text json_file){
    Text msg ="[run] []";
    if(File_exists(source_file)){
         msg+= " [open 4dm] ";
    }else{
        msg+=" [] ";
    }
    if(File_exists(json_file))msg += ", json to OPW, ";
    msg+= "../"+get_parent(source_file)+"/";
    Set_data(message_box,msg);
}


Text to_bool(Integer i){    return i ? "True" : "False" ;   }

Integer manage_panel(Integer &pos_x, Integer &pos_y, Text default_page){
    if((pos_x <0 ) || (pos_y<0)) Get_cursor_position(pos_x,pos_y);
    Panel panel = Create_panel( PANEL_NAME ,1);// panel name can't be dynamic.. Create_panel("Launch: Set["+To_text(active_set)+"/"+To_text(n_sets)+"]",1);
    Message_Box message_box = Create_message_box("");

    Directory_Box dir_box =     Create_directory_box("",message_box,44);    Set_dump_name(dir_box,"Folder"); // CHECK_DIRECTORY_MUST_EXIST
    Input_Box wildcard_box =    Create_input_box("",message_box);           Set_dump_name(wildcard_box,"Wildcard");
    Set_width_in_chars(dir_box,10);
    Set_width_in_chars(wildcard_box,4); Use_browse_button(wildcard_box,0);

    Button prev_set = Create_button("<","set prev");    Set_tooltip(prev_set,"prev set");       Set_width_in_chars(prev_set,1);
    Button next_set = Create_button(">","set next");    Set_tooltip(next_set,"next set");       Set_width_in_chars(next_set,1);
    Button new_set = Create_button("+","set new");      Set_tooltip(new_set,"add a set");       Set_width_in_chars(new_set,1);
    Button rem_set = Create_button("-","set rem");      Set_tooltip(rem_set,"remove this set"); Set_width_in_chars(rem_set,1);
    Button panel_set = Create_button("P","set panel");      Set_tooltip(panel_set,"edit set from a panel"); Set_width_in_chars(panel_set,1);

    Input_Box      filter_box = Create_input_box("Filter  ",message_box);   Use_browse_button(filter_box,0);    /* Set_width_in_chars(filter_box,6);*/ 
    Button toggle_sort = Create_button("T","toggle sort");             Set_width_in_chars(toggle_sort,2);      Set_tooltip(toggle_sort,"[v=Desending] [^=Ascending]");
    Button toggle_recursive = Create_button("S","toggle recursive");   Set_width_in_chars(toggle_recursive,2); Set_tooltip(toggle_recursive,"[S=Recursive Search] [$=Not Recursive]");

    List_Box list_box = Create_list_box("list title", message_box,6);

    Input_Box hidden_settings_box = Create_input_box( "" ,message_box);   Set_width_in_chars(hidden_settings_box,0);            Use_browse_button(hidden_settings_box,0);     Set_dump_name(hidden_settings_box,HIDDEN_SETTINGS_NAME);//Append(n_sets_box,hg_hack);
    Set_sizing_constraints(hidden_settings_box,4,4); // magic
    Horizontal_Group hg1 = Create_horizontal_group(1);
    Horizontal_Group hg2 = Create_horizontal_group(1);
    Vertical_Group vg_lb = Create_vertical_group(0); Set_border(vg_lb,"-");

    Append(hg1,panel);
    Append(vg_lb,panel);
    Append(hg2,vg_lb);
    Append(hidden_settings_box,hg1); Append(dir_box,hg1); Append(wildcard_box,hg1);    Append(prev_set,hg1);    Append(next_set,hg1);    Append(new_set,hg1);    Append(rem_set,hg1);    Append(panel_set,hg1);
    Append(filter_box,hg2);   Append(toggle_sort,hg2);    Append(toggle_recursive,hg2);
    Append(list_box,vg_lb);
    Append(message_box,panel);

    Show_widget(panel,pos_x,pos_y);
    read_panel_from_appdata(panel);
    Text state = get_state(hidden_settings_box,default_page);// Print(state+"\n");
    Integer is_init = 0;
    Integer is_upto_date = 0;
    Dynamic_Text all_files, all_paths;
    Integer n_files;
    Dynamic_Text lb_files,lb_paths;

    while(1){
        // Print(state+"\n");
        Integer active_set, n_sets;     get_counts(state,active_set, n_sets);
        Text basepath = valid_path(get_basepath(state,active_set));
        Text wildcard = get_wildcard(state,active_set);
        Text filter = get_filter(state,active_set);
        Integer is_ascending = get_ascending(state,active_set);
        Integer is_recursive = get_recursive(state,active_set);
        Set_data(hidden_settings_box,state);
        // Print("basepath:"+basepath+"\n"+"wildcard:"+wildcard+"\n"+"filter:"+filter+"\n");
        if(!is_init){   // if this is 0 the directory will be searched for file
            is_init = 1;
            is_upto_date = 0;   // and it will also trigger an update the list box also
            n_files = get_files(basepath, wildcard, is_recursive, 0, all_files, all_paths);
        }
        if(!is_upto_date){
            is_upto_date = 1;
            rebuild(list_box, valid_filter_string(filter), is_ascending, all_files, all_paths, lb_files, lb_paths );
            Integer n_listed; Get_number_of_items(lb_files,n_listed);
            Set_data(message_box,"[ Page " +To_text(active_set)+" of "+ To_text(n_sets) + " ]  "+To_text(n_listed)+" / "+To_text(n_files)+ " files  , (recursive="+to_bool(is_recursive)+")");
            Set_data(dir_box,basepath);
            Set_data(wildcard_box,wildcard);
            Set_data(filter_box,filter);
        }

        Set_name(toggle_sort,"^");       if(is_ascending)Set_name(toggle_sort,"v");
        Set_name(toggle_recursive,"$");  if(is_recursive)Set_name(toggle_recursive,"/s");

        Integer id; Text cmd, msg;
        Wait_on_widgets(id,cmd,msg);
        // Print("id<"+To_text(id) +">, cmd<"+cmd+">, msg<"+msg+">\n" );

        if(cmd == "keystroke"){
            if(id != Get_id(filter_box)) continue;
        }
        if(cmd == "Panel About")	manage_about_panel();
        if(cmd == "CodeShutdown")	Set_exit_code(cmd);
        if(cmd == "Panel Hide")		wait_on_panel_hide();
		if(cmd == "Panel Quit") { write_panel_to_appdata(panel); return -1; }

        if(cmd == "toggle sort"){
            toggle_0_1(is_ascending);
            state = set_ascending(state,active_set,is_ascending);
            is_upto_date = 0;   // force a rebuild
        }

        if(cmd == "toggle recursive"){
            toggle_0_1(is_recursive);
            state = set_recursive(state,active_set,is_recursive);
            is_init = 0;   // force a rebuild
        }

        switch (id) {
            case (Get_id(list_box)) : {
                Text path_file_ext, ext;
                if(!get_filename_check_exists(msg, lb_files, lb_paths, path_file_ext, ext)){
                    Print("ERROR: shouldn't get here!, thinks file does not where fullpath:<"+path_file_ext +"> ext:<" +ext+">\n");
                    break;
                }
                
                Text source_file = Get_subtext(path_file_ext,1,Text_length(path_file_ext)-Text_length(ext)) + "4dm";
                Text json_file = Get_subtext(path_file_ext,1,Text_length(path_file_ext)-Text_length(ext)) + "json";
                set_message(message_box,source_file,json_file);
                Print(cmd+"\n");
                switch (cmd) {
                    case ("double_click_lb") :{
                        switch (ext){
                            case("4do")     :{ Create_macro("-no_console -close_on_exit \""+path_file_ext+"\"",1);   } break; // run a 4do
                            case("chain")   :{ Run_chain_as_macro(path_file_ext);   } break;                                           // run a chain, dont quote
                            default         :{ execute_file("\""+path_file_ext+"\"");   }break;                               // run a non native file
                        }
                    }break;
                    case ("click_lb") :{
                        
                        echo(json_file);
                    }break;
                    case ("double_click_rb") :{ 
                        switch (ext){
                            case("chain")   :{ edit_chain_via_chain("\""+path_file_ext+"\""); } break;
                            default         :{ 
                                if(!File_exists(source_file)) break;
                                execute_file("\""+source_file+"\"");
                            }break;
                        }
                    }break;
                    case ("click_rb") :{

                    }break;
                }

            }break;

            case (Get_id(wildcard_box)) : {
                if(cmd == "text selected"){ // || "kill_focus" also?... should keep previous value and check if it changes
                    state = set_wildcard(state,active_set,msg);
                    is_init = 0; // get files again and rebuild
                }
            }break;

            case (Get_id(dir_box)) : {
                if(cmd == "text selected"){
                    state = set_basepath(state,active_set,msg);
                    is_init = 0; // get files again and rebuild
                }
            }break;

            case (Get_id(filter_box)) : {
                if( (cmd == "keystroke") || (cmd == "paste")){
                    Get_data(filter_box,filter);
                    state = set_filter(state,active_set,filter);
                    is_upto_date = 0;   // just refresh
                }
            }break;
            
            case (Get_id(prev_set)) : {
                active_set--;
                state = set_counts(state,active_set,n_sets);
                is_init = 0;
            }break;
            case (Get_id(next_set)) : {
                active_set++;
                state = set_counts(state,active_set,n_sets);
                is_init = 0;
            }break;
            case (Get_id(new_set)) : {
                state = add_page(state);
                is_init = 0;
            }break;
            case (Get_id(rem_set)) : {
                if(n_sets > 1){
                    state = remove_page(state,active_set);
                    is_init = 0;
                }
            }break;
            case (Get_id(panel_set)) : {
                Set_enable(panel,0);
                state = part_panel(state);
                is_init = 0;
                Set_enable(panel,1);
            }break;
        }
        
    }
    return -2;

}

// notes on data structure
// \t for words
// ; for parts
// : is a special character to handle a blank value for a part
// - if more parts are needed expand to the right to be backwards compatible
// - use make_page() for the default_page

void main(){
    Text basepath = "$USER_LIB";
    Text wildcard = "*.4do";
    Text filter_pattern = "*"; // ":" from here use for a blank.  It's all handled within the panel logic elsewhere
    Integer is_ascending = 1;
    Integer is_recursive = 1;
    Text default_page = make_page(basepath,wildcard,filter_pattern,is_ascending,is_recursive);

    Integer pos_x=-1,pos_y=-1;
    manage_panel(pos_x,pos_y,default_page);
}