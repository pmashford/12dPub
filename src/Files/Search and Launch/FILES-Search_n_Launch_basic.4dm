#include "mashy_lib_files.H"
#include "mashy_lib_panel_defaults.H"
// get a list of all .4do files

//  

void echo(Text filename){
    if(!File_exists(filename)) return;
    File f;
    File_open(filename,"r",f);
    Text t, u;
    while(!File_read_line(f,u)) t+="\n"+u;
    File_close(f);
    Print(t+"\n");
}

Dynamic_Text get_files(Text basepath, Text wildcard, Integer is_recursive){
    Text command = "dir \"" + valid_path(basepath+"\\"+wildcard) + "\" /ogn /a-d";
    if(is_recursive) command += " /s";
    command += " /b";
    return run_system_command_get_output(command);
}

Integer get_files(Text basepath, Text wildcard, Integer is_recursive, Dynamic_Text &filename, Dynamic_Text &path_to_filename){
	Null(filename);
    Null(path_to_filename);
    Dynamic_Text list = get_files(basepath, wildcard, is_recursive);
	Integer n_items;
	Get_number_of_items(list,n_items);
	for(Integer i=1;i<=n_items;i++){
		Text t,fname,path;
		Get_item(list,i,t);
        get_file_path_filename(t,path, fname);
        Append(fname,filename);
        Append(path,path_to_filename);
	}
	
	return n_items;
}

void wait_on_panel_hide(){}

void add_items(List_Box &b , Dynamic_Text &l){
    Integer n;
    Get_number_of_items(l,n);
    Text t;
    for(Integer i=1;i<=n;i++){
        Get_item(l,i,t);
        Add_item(b,t);
    }
}

void clear(List_Box &b){
    Integer n;
    Get_number_of_items(b,n);
    for(Integer i=n;i>0;i--){
        Delete_item(b,i);
    }
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

void toggle_0_1(Integer &mode){
    mode = (mode) ? 0 : 1;
}

 Integer get_filename_check_exists(Text msg, Dynamic_Text &lb_files, Dynamic_Text &lb_paths, Text &path_file_ext, Text &ext){
    Integer idx;    From_text(msg,idx);
    Text this_file, this_path;
    Get_item(lb_files,idx,this_file);
    Get_item(lb_paths,idx,this_path);
    path_file_ext = valid_path(this_path+"\\"+this_file);
    get_extension(path_file_ext,ext);
    ext = Text_lower(ext);  // TODO - do it in get_extension()!
    return File_exists(path_file_ext);
}

void set_message(Message_Box message_box, Text source_file, Text json_file){
    Text msg ;
    if(File_exists(source_file))msg += "has 4dm... ";
    if(File_exists(json_file))msg += "has json... ";
    if(msg != "") msg = "["+msg+"]";
    Set_data(message_box,msg);
}

void manage_panel(Text basepath , Text wildcard, Integer is_recursive, Text lb_filter_pattern){

    Panel panel = Create_panel("Search with Launch",1);
    Message_Box message_box = Create_message_box("");
    Directory_Box dir_box = Create_directory_box("Folder",message_box,44); // CHECK_DIRECTORY_MUST_EXIST
    Input_Box      filter_box = Create_input_box("Filter  ",message_box);
    Input_Box      wildcard_box = Create_input_box("",message_box);         Set_dump_name(wildcard_box,"Wildcard");
    List_Box list_box = Create_list_box("list title", message_box,6);
    //Button toggle_sort = Create_bitmap_button("T","toggle sort","vu_down.bmp");
    Button toggle_sort = Create_button("T","toggle sort");              Set_tooltip(toggle_sort,"[v=Desending] [^=Ascending]");
    Button toggle_recursive = Create_button("S","toggle recursive");    Set_tooltip(toggle_recursive,"[S=Recursive Search] [$=Not Recursive]");
    Set_width_in_chars(dir_box,12);
    Set_width_in_chars(wildcard_box,4); Use_browse_button(wildcard_box,0);
   /* Set_width_in_chars(filter_box,6);*/ Use_browse_button(filter_box,0);
    Set_width_in_chars(toggle_sort,2);
    Set_width_in_chars(toggle_recursive,2);
    Horizontal_Group hg1 = Create_horizontal_group(1);
    Horizontal_Group hg2 = Create_horizontal_group(1);
    Vertical_Group vg_lb = Create_vertical_group(0); Set_border(vg_lb,"-");
    Append(hg1,panel);
    Append(vg_lb,panel);
    Append(hg2,vg_lb);
    Set_name(toggle_recursive,"X");// a-> z->   // S $
    


    Append(dir_box,hg1);  Set_data(dir_box,basepath);
    Append(wildcard_box,hg1); Set_data(wildcard_box,wildcard);
    Append(filter_box,hg2);   Set_data(filter_box,lb_filter_pattern);
    Append(toggle_sort,hg2);
    Append(toggle_recursive,hg2);
    Append(list_box,vg_lb);
    Append(message_box,panel);
    Show_widget(panel);
    read_panel_from_appdata(panel);
    Integer is_init = 0;
    Integer is_upto_date = 0;
    Integer lb_is_assending = 1;
    //Integer lb_is_recursive = 1;
    Dynamic_Text all_files, all_paths;
    Integer n_files;


    Dynamic_Text lb_files,lb_paths;

    while(1){
        Set_name(toggle_sort,"^");  if(lb_is_assending)Set_name(toggle_sort,"v");
        Set_name(toggle_recursive,"$");  if(is_recursive)Set_name(toggle_recursive,"S");

        Integer id; Text cmd, msg;
        if(!is_init){   // if this is 0 the directory will be searched for file
            is_init = 1;
            is_upto_date = 0;   // and it will also trigger an update the list box also
            n_files = get_files(basepath, wildcard, is_recursive, all_files, all_paths);
        }
        if(!is_upto_date){
            is_upto_date = 1;
            rebuild(list_box, lb_filter_pattern, lb_is_assending, all_files, all_paths, lb_files, lb_paths );
        }

        Wait_on_widgets(id,cmd,msg);
         Print("id<"+To_text(id) +">, cmd<"+cmd+", msg<"+msg+">\n" );

        if(cmd == "keystroke"){
            if(id != Get_id(filter_box)) continue;
        }
        //if(cmd == "Panel About")	manage_about_panel();
        if(cmd == "CodeShutdown")	Set_exit_code(cmd);
        if(cmd == "Panel Hide")		wait_on_panel_hide();
		if(cmd == "Panel Quit") { write_panel_to_appdata(panel); return; }

        if(cmd == "toggle sort"){
            toggle_0_1(lb_is_assending);
            is_upto_date = 0;   // force a rebuild
        }

        if(cmd == "toggle recursive"){
            toggle_0_1(is_recursive);
            is_init = 0;   // force a rebuild
        }

        switch (id) {
            case (Get_id(list_box)) : {
                Text path_file_ext, ext;
                if(!get_filename_check_exists(msg, lb_files, lb_paths, path_file_ext, ext)){
                    Print("ERROR: shouldn't get here !");
                    break;
                }
                Text source_file = Get_subtext(path_file_ext,1,Text_length(path_file_ext)-Text_length(ext)) + "4dm";
                Text json_file = Get_subtext(path_file_ext,1,Text_length(path_file_ext)-Text_length(ext)) + "json";
                set_message(message_box,source_file,json_file);

                switch (cmd) {
                    case ("double_click_lb") :{
                        if(ext == "4do")    Create_macro("-no_console -close_on_exit \""+path_file_ext+"\"",1);   // run a 4do
                        else run_system_command_no_wait("\""+path_file_ext+"\"");                                         // run not a 4do
                    }break;
                    case ("click_lb") :{
                        
                        echo(json_file);
                    }break;
                    case ("double_click_rb") :{
                        
                        if(!File_exists(source_file)) break;
                        run_system_command_no_wait("\""+source_file+"\"");
                    }break;
                    case ("click_rb") :{

                    }break;
                }

            }break;

            case (Get_id(wildcard_box)) : {
                if(cmd == "text selected"){
                    wildcard = msg; // new path
                    is_init = 0; // get files again and rebuild
                }
            }break;

            case (Get_id(dir_box)) : {
                if(cmd == "text selected"){
                    basepath = msg; // new path
                    is_init = 0; // get files again and rebuild
                }
            }break;

            case (Get_id(filter_box)) : {
                if( (cmd == "keystroke") || (cmd == "paste")){
                    Get_data(filter_box,lb_filter_pattern);
                    lb_filter_pattern = "*"+lb_filter_pattern+"*";
                    is_upto_date = 0;   // just refresh
                }
            }break;


        }
        
    }

}

void main(){
    Text basepath = "c:\\github\\12dpub";
    Text wildcard = "*.4do";
    Integer is_recursive = 1;
    Text lb_filter_pattern = "*";

    manage_panel(basepath , wildcard, is_recursive, lb_filter_pattern);
    
}