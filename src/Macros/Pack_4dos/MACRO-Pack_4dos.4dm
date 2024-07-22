#include "12d/set_ups.4d"
#include "mashy_lib_files_base64.H"
// #include "mashy_lib_widgets_validate.H"
#include "mashy_lib_about_panel.H"


void process(Text cc4d_file, Text v14_file, Text v15_file, Text new_file){
	Integer ok=0;
    File fout;
    Text source_file_4dm = Get_subtext(new_file,1,Text_length(new_file)-1)+"m";
	ok+=File_open(source_file_4dm,"w",fout);
    append_binary_file_to_4dm_file_with_function_name(v14_file, fout, "datav14");
    append_binary_file_to_4dm_file_with_function_name(v15_file, fout, "datav15");
    write_unpack_footer(fout );
    ok+=File_close(fout);
    Text command = "\""+cc4d_file+"\" " + source_file_4dm +" -allow_old_calls";
    // Print(command+"\n");
    Dynamic_Text errs = run_system_command_get_output(command);
    // print(errs);
    File_delete(source_file_4dm);
}

void go_panel(){
    Panel panel = Create_panel("4do Packer");
    Message_Box message_box = Create_message_box("");
    File_Box cc4d_box = Create_file_box("v14 cc4d",message_box,CHECK_FILE_MUST_EXIST,"cc4d.exe");
    File_Box v14_box = Create_file_box("v14 4do",message_box,CHECK_FILE_MUST_EXIST,"*.4do");
    File_Box v15_box = Create_file_box("v15 4do",message_box,CHECK_FILE_MUST_EXIST,"*.4do");
    File_Box new_box = Create_file_box("New 4do",message_box,CHECK_FILE_NEW,"*.4do");
    Button go_button = Create_button(" Pack ","go n pack");
    Append(cc4d_box,panel);
    Append(v14_box,panel);
    Append(v15_box,panel);
    Append(new_box,panel);
    Append(go_button,panel);
    Append(message_box,panel);
    Show_widget(panel);
    Set_data(cc4d_box,"C:\\Program Files\\12d\\12dmodel\\14.00\\nt.x64\\cc4d.exe");
    // Set_data(v14_box,".4do");
    // Set_data(v15_box,".4do");
    Set_data(new_box,"V14orV15.4do");
    while(1){
		Integer id; Text cmd,msg;
		Wait_on_widgets(id,cmd,msg);
		// Print("Id=" + To_text(id) + " cmd=" + cmd + " msg=" + msg + "\n");
		if(cmd == "keystroke") continue;
		if(cmd == "Panel Quit") return;
        if(cmd == "Panel About") manage_about_panel();
#if VERSION_4D > 1400
        if(cmd == "CodeShutdown")Set_exit_code(cmd);
#endif
		switch (cmd) {
			case("go n pack") : {
					Text cc4d_file,v14_file,v15_file,new_file;
					if(Validate(cc4d_box,CHECK_FILE_MUST_EXIST,cc4d_file) == 0){
						Set_data(message_box,"Invalid source file");
						Set_focus(cc4d_box);
						break;
					}
					if(Validate(v14_box,CHECK_FILE_MUST_EXIST,v14_file) == 0){
						Set_data(message_box,"Invalid source file");
						Set_focus(v14_box);
						break;
					}
					if(Validate(v15_box,CHECK_FILE_MUST_EXIST,v15_file) == 0){
						Set_data(message_box,"Invalid source file");
						Set_focus(v15_box);
						break;
					}
					if(Validate(new_box,CHECK_FILE,new_file) == 0){
						Set_data(message_box,"Invalid target file");
						Set_focus(new_box);
						break;
					}
					process(cc4d_file,v14_file,v15_file,new_file);
			}
		}
    }
}

void main(){
    go_panel();
}