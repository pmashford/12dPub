#ifndef mashy_lib_panel_defaults_H_included
#define mashy_lib_panel_defaults_H_included

#include "mashy_lib_files.H"
#include "mashy_lib_cast.H"

 // For the modern solution use these. - lot of this is depreciated with 12ds inbuilt Read_SLX() Write_SLX()
 // These custom calls can find the special ddx files in they are not found in .project and falls back to the standard 12d search library(s)
// Integer write_panel(Panel &panel);
// Integer read_panel(Panel &panel);

Text replace_non_alpha_numeric(Text t){
	//Text newt = t;
	Integer length = Text_length(t);
	for(Integer i=1;i<=length;i++){
		Integer c1;
		Get_char(t,i,c1);
		if(c1 >= 'a' && c1 <= 'z')continue;
		if(c1 >= 'A' && c1 <= 'Z')continue;
		if(c1 >= '0' && c1 <= '9')continue;
		Set_char(t,i,'_');
	}
	return t;
}

// this will get the name of the panel (the title)
Text get_name(Panel &panel){
	Widget w = cast(panel);
	Text t;
	Get_name(w,t);
	return t;
}

Text panel_name_to_valid_file_name(Panel &panel){
	return replace_non_alpha_numeric(get_name(panel));
}

Text get_file_name_for_panel_read_write_settings(Panel &panel){
	Text path, base_name, extension;
	get_macro_name_path_parts(path, base_name, extension);
	Text panel_name = panel_name_to_valid_file_name(panel);
	return base_name + "_" + panel_name + ".ddxm";
}

Integer write_panel(Panel &panel, Text file_name, Integer verbose){
	if(File_exists(file_name)){ // Write_SLX() will fail with a return error code 3 if the file exists, so attempt a file delete
		if(verbose) Print("<File_delete("+file_name+")> with <ierr="+To_text(File_delete(file_name))+"> for wite_panel() update\n");
	}
	Integer ierr = Write_SLX(panel,file_name);
	if(verbose) Print("<"+file_name+">< Write_SLX() with ierr="+To_text(ierr)+">\n");
	return(ierr);
}

Integer write_panel_to_appdata(Panel &panel){
	Text file_name_ext = get_file_name_for_panel_read_write_settings(panel);
	Text file_name = valid_path("$appdata" + "\\" + file_name_ext); 
	return write_panel(panel,file_name,1);	// be verbose when using appdata ?
}

Integer write_panel(Panel &panel){
	Text file_name_ext = get_file_name_for_panel_read_write_settings(panel);
	Text dot_project_folder = get_dot_project_folder();
	Text file_name = valid_path(dot_project_folder + "\\" + file_name_ext); 
	return write_panel(panel,file_name,0);
	// if(File_exists(fileA)){ // Write_SLX() will fail with a return error code 3 if the file exists, so attempt a file delete
	// 	Print("<File_delete("+fileA+")> with <ierr="+To_text(File_delete(fileA))+"> for wite_panel() update\n");
	// }

	// Integer ierr = Write_SLX(panel,fileA);
	// Print("<"+fileA+">< Write_SLX() with ierr="+To_text(ierr)+">\n");
	// return(ierr);
}

Integer read_panel(Panel &panel, Text file_name, Integer verbose){
	if(File_exists(file_name)){
		Print("found\n");
		if(verbose) Print("<"+file_name+"> File found, panel settings loaded\n");
		return Read_SLX(panel,file_name);
	}else{
		Print("not found\n");
		Text base_file_name, old_file_name = "" , env = "" ;
		base_file_name = get_file_path_filename(file_name); // instead of a full path we want to ensure this is file.ext only
		file_name = Find_system_file   (base_file_name, old_file_name, env);
		if(verbose) Print("<"+file_name+"> File found using file found from Find_system_file("+base_file_name+"), panel setting loaded\n");
		if(!File_exists(file_name)) return 9;
		return Read_SLX(panel,file_name);
	}
	return -999; // can't get here, will always return somewhere above
}


Integer read_panel_from_appdata(Panel &panel){
	Text file_name_ext = get_file_name_for_panel_read_write_settings(panel);
	Text file_name = valid_path("$appdata" + "\\" + file_name_ext); 
	Print(file_name+"\n");
	return read_panel(panel,file_name,0);	// be verbose when using appdata ?
}


Integer read_panel(Panel &panel){
	Text file_name_ext = get_file_name_for_panel_read_write_settings(panel);
	Text dot_project_folder = get_dot_project_folder();
	Text file_name = valid_path(dot_project_folder + "\\" + file_name_ext);
	return read_panel(panel,file_name,0);
	// Text file_name_ext = get_file_name_for_panel_read_write_settings(panel);
	// Text dot_project_folder = get_dot_project_folder();
	// Text fileA = dot_project_folder + "\\" + file_name_ext;
	// if(File_exists(fileA)){
	// 	Print("<"+fileA+"> File found, panel settings loaded\n");
	// 	return Read_SLX(panel,fileA);
	// }else{
	// 	Text old_file_name = "" , env = "" ;
	// 	fileA = Find_system_file   (file_name_ext, old_file_name, env);
	// 	Print("<"+fileA+"> File found using file found from Find_system_file("+file_name_ext+"), panel setting loaded\n");
	// 	return Read_SLX(panel,fileA);
	// }
	// return 1;
}



///  PROBABLY EVERYTHING BELOW THIS IS ALL DEPRECIATED AND REMAIN FOR OLD PANEL DEFAULTS HACK, SHOULD MOVE

#ifdef INCLUDE_DEPRECIATED_MASHY_LIB_PANEL_DEFAULTS_H
#include "depreciated/ss_mashy_lib_panel_defaults.H"
#endif

#endif