#ifndef mashy_lib_widgets_menu_handler_std_H_INCLUDED
#define mashy_lib_widgets_menu_handler_std_H_INCLUDED

#include "mashy_lib_files.H"
#include "mashy_lib_arguments.H"
#include "mashy_lib_xml_chains.H"
#include "mashy_lib_system_calls.H"


Text menu_display_list_get_reply(Text t[], Integer size, Text title, Integer &x, Integer &y){
  Menu menu = Create_menu(title);
  for(Integer i=1;i<=size;i++)  Create_button(menu,t[i],t[i]);
  Text menu_reply="";
  Display(menu,x,y,menu_reply);
  return menu_reply;
}

Text menu_display_list_get_reply(Dynamic_Text dt, Text title, Integer &x, Integer &y){
  Integer size;
  Get_number_of_items(dt,size);
  if(size<1)return"";
  Text t[size];
  for(Integer i=1;i<=size;i++)  Get_item(dt,i,t[i]);
  return menu_display_list_get_reply(t, size, title, x, y);
}

Text menu_display_list_get_reply(Dynamic_Text dt, Text title){
  Integer x=-1,y=-1;
  return menu_display_list_get_reply(dt, title, x, y);
}

void add_prefix_suffix(Dynamic_Text &dt, Text prefix, Text suffix){
  Integer items;  Get_number_of_items(dt,items);
  for(Integer i=1;i<=items;i++){
    Text t; Get_item(dt,i,t); Set_item(dt,i,prefix+t+suffix);
  }
}

Text menu_walk_dir(Text base, Text file_wildcard, Text quit_text_at_top_of_list, Integer &x, Integer &y){
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
  return menu_display_list_get_reply(list,file_wildcard,x,y);
}

Integer is_last_char(Text text_to_search, Integer c){
  Integer last_c;
  Get_char(text_to_search,Text_length(text_to_search),last_c);
  return c == last_c ;
}

Text menu_get_file(Text base_path, Text file_wildcard, Integer exit_on_selection, Integer execute_on_selection){
  Text base_in = valid_path(base_path);
  Text base = base_in;
  Text quit_text_at_top_of_list = " - Exit - ";
  if(exit_on_selection) quit_text_at_top_of_list = ""; // the menu will handle this
  Integer x=-1,y=-1;
  Text reply;
  do {
     Text msg;
     reply =   menu_walk_dir(base, file_wildcard, quit_text_at_top_of_list, x, y);
     if(reply=="")continue;
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
            Integer run_now = 1;
            //Create_macro("\""+this_file+"\"",run_now,handle); // Paul Hardwick email
            Create_macro("-no_console -close_on_exit \""+this_file+"\"",run_now); 
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


#endif
