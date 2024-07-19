#include "mashy_lib_widgets_menu_handler_std.H"
void usage_to_console(){
  Print("" +
  "example usage from command line args.  $path can be hardcoded paths" + "\n" +
  "TYPICAL - to have a menu sitting there wanting to run a macro" + "\n" +
  "$path/MACRO-Launcher_Menu.4do $macro_path *.4do 0 1" + "\n" +
  "ALT USAGE - to have a menu sitting there wanting to run any file" + "\n" +
  "$path/MACRO-Launcher_Menu.4do $any_path *.* 0 1" + "\n");
}

void usage_to_menu(){
  Dynamic_Text dt;
  Append("",dt);
  Append("arg1 = path [ie. c:\\ or $user]",dt);
  Append("arg2 = wildcard [ie. *.* or *.4do or *.exe ...]",dt);
  Append("arg3 = exit on selection? [0/1]",dt);
  Append("arg4 = execute on selection? [0/1]",dt);
  Append("",dt);
  Append("On OK running with args set to: $user *.4do 0 1",dt);
  Append("",dt);
  Append("  OK  ",dt);
  menu_display_list_get_reply( dt,  " USAGE ");

}

Integer set_args(Text t[], Integer size){
// this returns number of command line arguments set to t[], return wont be more than size
  Integer lower  = Get_number_of_command_arguments();
  if ( size < lower ) lower = size; // guard
  for(Integer i=1;i<=lower;i++){
    if(Get_command_argument(i,t[i])) t[i] = ""; // if error set ""
  }
  return lower;
}

Text get_value_or_default(Text text_input, Text value_if_blank){
  if(text_input == "") return value_if_blank;
  return text_input;
}

Integer from_text(Text t){
  Integer i;
  if(From_text(t,i))return 0;
  return i;
}

void main(){
    usage_to_console();
    Text args[4];
    Integer ierr = set_args(args,4);
    if(ierr == 0)usage_to_menu();
    Text base_path = get_value_or_default(args[1],"$user");
    Text file_wildcard = get_value_or_default(args[2], "*.4do");
    Integer exit_on_selection = from_text(get_value_or_default(args[3], "0"));
    Integer execute_on_selection = from_text(get_value_or_default(args[4], "1"));
    menu_get_file( base_path,  file_wildcard,  exit_on_selection,  execute_on_selection);
}