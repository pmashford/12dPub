
#ifndef	CREATE_NO_WINDOW
	#define CREATE_NO_WINDOW 0x08000000
#endif
d
// days old - +ve means not older than, -ve means older than
Dynamic_Text get_files(Text basepath, Text wildcard, Integer is_recursive, Integer days_old){
	Text dir_name; Get_temporary_project_directory(dir_name);
	Text bat_file = dir_name+"temp_batch_file_name.bat";
	Text rpt_file = dir_name+"temp_report_file_name.txt";
	if(File_exists(bat_file)) File_delete(bat_file);
	if(File_exists(rpt_file)) File_delete(rpt_file);

    basepath = valid_path(basepath);
    Text last_char; Get_subtext(basepath,Text_length(basepath),Text_length(basepath));
    if( (last_char == "\\") ||  (last_char == "/") ) basepath = Get_subtext(basepath,1,Text_length(basepath)-1);
    Text slash_s = "";  if(is_recursive) slash_s = " /S";
    if(wildcard!="") wildcard = " /M " + wildcard;
    Text slash_d = "";
    if(days_old!=0) slash_d = " /D "+To_text(days_old);
    //Text command = "forfiles /P \"" + basepath + "\"" + slash_s + wildcard + slash_d + " /C \"cmd /c if @isdir==FALSE echo @path >> " + quotes_add(rpt_file) + " 2>&1\"";
    Text command = "forfiles /P \"" + basepath + "\"" + slash_s + wildcard + slash_d + " /C \"cmd /c if @isdir==FALSE echo @path >> " + quotes_add(rpt_file) ;
    Print(command+"\n");

    File f;
    File_open(bat_file,"w",f);
    File_write_line(f,command);
    File_close(f);
    Integer wait_for_process_to_finish = 1;
    Create_process("cmd.exe", "/C " + quotes_add(bat_file), "", CREATE_NO_WINDOW, wait_for_process_to_finish, 0); //must wait for the command to finish otherwise not good outcomes

    Dynamic_Text dt;
    open_file_save_list(rpt_file,dt);
    return dt;

}
