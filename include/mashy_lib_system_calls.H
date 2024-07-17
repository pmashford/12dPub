#ifndef mashy_lib_system_calls_h_included
#define mashy_lib_system_calls_h_included

Integer run_system_command(Text command); // hack, do protoypes here
Dynamic_Text run_system_command_get_output(Dynamic_Text &commands);
Dynamic_Text run_system_command_get_output(Text command);

#include "mashy_lib_text_functions.H"
#include "mashy_lib_files.H"


#ifndef	CREATE_NO_WINDOW
	#define CREATE_NO_WINDOW 0x08000000
#endif

Dynamic_Text run_system_command_get_output(Dynamic_Text &commands, Integer wait_for_process_to_finish){
// run a system command, each command has the output redirected and read in 1 at a time, combining them all into 1 Dynamic_Text array
	Dynamic_Text dt_return;
	Text dir_name; Get_temporary_project_directory(dir_name);
	Text batch_file_name = dir_name+"temp_batch_file_name.bat";
	Text report_file_name = dir_name+"temp_report_file_name.txt";
	Integer ierr;
	ierr = File_exists(batch_file_name);if(ierr != 0) File_delete(batch_file_name);
	ierr = File_exists(report_file_name);if(ierr != 0) File_delete(report_file_name);
	Integer items;
	Get_number_of_items(commands,items);
	for(Integer i = 1; i <= items ; i++ ){
		Text cmd;
		Get_item(commands,i,cmd);
		//cmd += " > " + quotes_add(report_file_name); // redirect output - NOTE POSSIBLE BUG HERE, > SHOULD BE >> (> WILL DELETE THE FILE?, >> APPENDS TO THE FILE??) <= not a bug, safe to delete the file each time
		// https://learn.microsoft.com/en-us/troubleshoot/developer/visualstudio/cpp/language-compilers/redirecting-error-command-prompt
		cmd += " 1> " + quotes_add(report_file_name) + "2>&1"; // redirect output - NOTE POSSIBLE BUG HERE, > SHOULD BE >> (> WILL DELETE THE FILE?, >> APPENDS TO THE FILE??) <= not a bug, safe to delete the file each time
		File batch_file;
		File_open(batch_file_name,"w+",batch_file);
		//File_write_line(batch_file,"start /min");
		File_write_line(batch_file,cmd);
		//File_write_line(batch_file,"exit");
		File_close(batch_file);
		//System(quotes_add(batch_file_name));
		//Integer Create_process(Text program_name,Text command_line,Text start_directory,Integer flags,Integer wait,Integer inherit);
		Create_process("cmd.exe", "/C " + quotes_add(batch_file_name), "", CREATE_NO_WINDOW, wait_for_process_to_finish, 0); //must wait for the command to finish otherwise
		// finally - can call a command without the dos window !!! yay !!!
		Dynamic_Text dt;
		open_file_save_list(report_file_name,dt);
		File_delete(batch_file_name);
		File_delete(report_file_name);
		Append(dt,dt_return);
		Null(dt);
	}
	return dt_return;
}

Dynamic_Text run_system_command_get_output(Dynamic_Text &commands){
	Integer wait_for_process_to_finish = 1;
	return run_system_command_get_output(commands, wait_for_process_to_finish);
}

Dynamic_Text run_system_command_get_output(Text command){
// function overload
	Dynamic_Text commands;
	Append(command,commands);
	return ( run_system_command_get_output(commands) );
}

Integer run_system_command(Dynamic_Text &commands, Integer wait_for_process_to_finish){
// run a bunch of system commands
	Text dir_name; Get_temporary_project_directory(dir_name);
	Text batch_file_name = dir_name+"temp_batch_file_name.bat";
	if(File_exists(batch_file_name)) File_delete(batch_file_name);
	Integer items;
	Get_number_of_items(commands,items);
	if(items == 0)return -1;
	for(Integer i = 1; i <= items ; i++ ){
		Text cmd;
		Get_item(commands,i,cmd);
		File batch_file;
		File_open(batch_file_name,"w+",batch_file);
		File_write_line(batch_file,cmd);
		File_close(batch_file);
		Create_process("cmd.exe", "/C " + quotes_add(batch_file_name), "", CREATE_NO_WINDOW, wait_for_process_to_finish, 0); //must wait for the command to finish otherwise
		File_delete(batch_file_name);
	}
	return 0;
}



Integer run_system_command_no_wait(Dynamic_Text &commands){
// run a bunch of system commands
	Text q = "\"";
	Text dir_name; Get_temporary_project_directory(dir_name);
	Text batch_file_name = dir_name+"temp_batch_file_name.bat";
	if(File_exists(batch_file_name)) File_delete(batch_file_name);
	Integer items;
	Get_number_of_items(commands,items);
	if(items == 0)return -1;
	for(Integer i = 1; i <= items ; i++ ){
		Text cmd;
		Get_item(commands,i,cmd);
		cmd = "start "+q+q+" "+cmd;
		File batch_file;
		File_open(batch_file_name,"w+",batch_file);
		File_write_line(batch_file,cmd);
		File_close(batch_file);
		Create_process("cmd.exe", "/C " + quotes_add(batch_file_name), "", CREATE_NO_WINDOW, 1, 0); //must wait for the command to finish otherwise
		File_delete(batch_file_name);
	}
	return 0;
}

Integer run_system_command(Dynamic_Text &commands){
	Integer wait_for_process_to_finish = 1;
	return run_system_command(commands, wait_for_process_to_finish);
}

Integer run_system_command(Text command){
// function overload
	Dynamic_Text commands;
	Append(command,commands);
	return ( run_system_command(commands) );
}


Integer run_system_command_no_wait(Text command){
	Dynamic_Text commands;
	Append(command,commands);
	return run_system_command_no_wait(commands);
}

Dynamic_Text get_dir_list(Text folder, Text pat){
//return all the files in folder matching pat as dt
// + appends quotes ect as required
	folder = text_justify(quotes_strip(text_justify(folder)));
	folder = find_replace_repeat(folder,"/","\\");	//pre-process string: replace any '/' with '\'
	if(folder == ""){
		Get_project_folder(folder);
	}
	if ( "\\" != Get_subtext(folder,Text_length(folder),Text_length(folder))){ //make the last character a slash
		folder = folder + "\\";
	}
	//Directory_exists(Text dir_name);// could check if it exists!
	pat = text_justify(quotes_strip(text_justify(pat)));
	if (pat == "" ) {pat = "*.*";}
	Text command = "dir " + "\"" + folder + pat + "\"" + " /a-d /b"; //there's a problem with sending /b via system call, write temp batch file
	Dynamic_Text dt = run_system_command_get_output(command);
	return dt;
}

Dynamic_Text get_dir_list(Text pat){
	Text working_folder;
	Get_project_folder(working_folder);
	return get_dir_list(working_folder,pat);
}

Integer open_website(Text website){
	// website = "www.google.com";
	Text operation = "open";
	Text parameters = "";
	Text folder = "";
	Integer showcmd = 0;
	Panel panel = Create_panel("title");
	Show_widget(panel,2000,10);
	return Shell_execute(panel,operation,website,parameters,folder,showcmd);
}


void delete_file_pattern(Text pattern){ Create_process("cmd.exe", "/C del " + pattern, "", 0x08000000, 0, 0); }
void execute_file(Text filename_ext){ Create_process("cmd.exe", "/C " + filename_ext, "", 0x08000000, 0, 0); }

#endif