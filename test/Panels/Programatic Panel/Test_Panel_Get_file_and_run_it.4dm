
#include "mashy_lib_widgets_panel_handler_std.H"

// THIS NEEDS TO BE DEFINED IN ANY MACRO WHICH INCLUDES "mashy_lib_widgets_panel_handler_std.H" as the H file has a protoype
Text panel_command_parser(Text command, Integer flags){
// flags might be a way of testing that a command is executable without actually executing
    Text return_text = "";
    return return_text;
}


void usage(){
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
  Integer pos_x=-1, pos_y=-1;
  Text title = "USAGE";
  panel_display_list_get_reply(title, pos_x, pos_y, dt, 1);
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
    Text args[4];
    Integer ierr = set_args(args,4);
    if(ierr == 0)usage();
    Text base_path = get_value_or_default(args[1],"$user");
    Text file_wildcard = get_value_or_default(args[2], "*.4do");
    Integer exit_on_selection = from_text(get_value_or_default(args[3], "0"));
    Integer execute_on_selection = from_text(get_value_or_default(args[4], "1"));
    panel_get_file( base_path,  file_wildcard,  exit_on_selection,  execute_on_selection);
}