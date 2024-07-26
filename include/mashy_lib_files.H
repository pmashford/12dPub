#ifndef mashy_lib_files_H_included
#define mashy_lib_files_H_included

Integer open_file_save_list(Text file_name,Dynamic_Text &file_contents ); //hack

Integer resolve_env(Text &source);
Integer resolve_env(Text source, Text &output);

#include "mashy_lib_system_calls.H"
#include "12d/standard_library.H"
#include "mashy_lib_text_functions.H"

#ifndef	CREATE_NO_WINDOW
	#define CREATE_NO_WINDOW 0x08000000
#endif

void echo(Text filename){
    if(!File_exists(filename)) return;
    File f;
    File_open(filename,"r",f);
    Text t, u;
    while(!File_read_line(f,u)) t+="\n"+u;
    File_close(f);
    Print(t+"\n");
}

Integer get_list_of_files_incl_search_of_subfolders(Text base_path_no_slash , Text wildcard, Dynamic_Text &file_names_full_path){
    Text cmd_call = "  /ON   /b /a-d /s"; // a == attributes, -d == no directories
    Text cmd = "DIR \"" + base_path_no_slash + "\\" +wildcard+"\"" +  cmd_call;
    file_names_full_path = run_system_command_get_output(cmd);
    p(file_names_full_path);
    Integer items;
    Get_number_of_items(file_names_full_path,items);
    // p(items);
    return items;
}

Integer find_files_in_subfolder(Text base_path_no_slash , Text wildcard, Dynamic_Text &file_names_full_path){
    Text cmd_call = "  /ON   /b /a-d /s"; // a == attributes, -d == no directories
    Text cmd = "DIR \"" + base_path_no_slash + "\\" +wildcard+"\"" +  cmd_call;
    file_names_full_path = run_system_command_get_output(cmd);
    Integer items;
    Get_number_of_items(file_names_full_path,items);
    return items;
}

Text get_cc4d(){
	Text t = Get_program();
	Integer last = 1, pos = 0, length = Text_length(t);
	while(1){
		pos = last + Find_text(Get_subtext(t,last+1,length),"\\");
		if(pos==last)break;
		if(pos = 0){
			pos = last;
			break;
		}
		last = pos;
	}
	return Get_subtext(t,1,pos)+"cc4d.exe";
}

Integer file_delete(Text file_name){
	if(File_exists(file_name)){
		return File_delete(file_name);
	}
	return 0;
}

Text get_temporary_file(Text file_name){
	Text data;
	Get_temporary_project_directory(data);
	return (data+"/"+file_name);
}

Text get_time_for_filename(){
	//Real time;		Time(time);
	Integer itime;		Time(itime);
	Text t;
	Convert_time    (itime,"%Y%m%d-%Hh%Mm(%Ss)",t);
	return (t);
}

Text get_unique_timestamped_filename(Text prefix, Text suffix){
// USAGE 	Print(get_unique_timestamped_filename("random",".12da"));Print();
// builds a unique filename with a unique timestamp
	Text timestamp = get_time_for_filename();
	Text filename = prefix + timestamp + suffix;
	Integer count = 0;
	while(1){
		if(!File_exists(filename))return filename;
		count++;
		filename = prefix + timestamp + "-"+To_text(count) + suffix;
	}
	return "";
}

Integer is_unc(Text path){
// is a path a unc path?
	Integer f1 = '\\';
	for(Integer i=1;i<=2;i++){ // check first 2 chars for \
		Integer c1;
		Get_char(path,i,c1);
		if(c1!=f1){
			return FALSE;
		}
	}
	return TRUE;
}

Integer is_mapped(Text path, Text &drive){
// is a path mapped, and what is the letter?
	Integer c1=0,c2=0;
	Get_char(path,1,c1);
	Get_char(path,2,c2);
	if(c2 == ':'){
		if(c1 > 'a' && c1 <= 'Z'){
			drive = Get_subtext(path,1,1);
			return TRUE;
		}
	}
	drive = "";
	return FALSE;
}

#if 0
Integer get_file_path_parts(Text path, Dynamic_Text &parts){
// pulls apart a path into an list of parts
// this will ignore multiple slashes, for better or worse ( ///this//path\\\\\\\\has/slashes\)
	Null(parts);

	Integer backslash = '\\';
	Integer forwardslash = '/';

	Integer length = Text_length(path);
	Integer start = 0;

	for(Integer i=1;i<=length;i++){
		Integer c1;
		Get_char(path,i,c1);
		if( (c1 == backslash) || (c1 == forwardslash)){
			if(start == 0){
				start = i+1;
			}else{
				if( ((i-1) - start) > 0){ //
					Append( Get_subtext(path,start,i-1),parts );
				}
				start = i+1;
				continue;
			}
		}else if(i==1){
			start = 1;
		}
		if(i==length && start){
			if( (i - start) >= 0 ){
				Append( Get_subtext(path,start,i),parts );
			}
		}
	}

	Integer items;
	Get_number_of_items(parts,items);

	return(items);
}

#else
Integer get_file_path_parts(Text path, Dynamic_Text &parts){
// pulls apart a path into an list of parts
// this will ignore multiple slashes, for better or worse ( ///this//path\\\\\\\\has/slashes\)
// non zero return is success
	Null(parts);

	Integer backslash = '\\';
	Integer forwardslash = '/';

	Integer length = Text_length(path);
	Integer ps = 0;
	Integer pe = 0;

	for(Integer i=1;i<=length;i++){
		Integer c1;
		Get_char(path,i,c1);
		if( ( c1 == backslash) || (c1 == forwardslash) ){
			if(ps){
				Append( Get_subtext(path,ps,pe),parts );
				ps=0;pe=0;
			}
		}else{
			if(ps == 0){
				ps = i;
				pe = i;
			}else{
				pe = i;
			}
		}
	}

	if(ps){
		Append( Get_subtext(path,ps,pe),parts );
	}

	Integer items;
	Get_number_of_items(parts,items);

	return(items);
}
#endif

Integer set_file_path_parts(Dynamic_Text &parts, Text &path){
// takes parts and creates a full path string as &path
	Integer items;
	Get_number_of_items(parts,items);
	if(items == 0){
		return(0);
	}
	path = "";
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t;
		Get_item(parts,i,t);
		if (i != 1 ){
			path += "\\" + t;
		} else {
			path = t;
		}
	}
	return(1);
}

Integer set_file_path_parts(Dynamic_Text &parts, Text &path,Integer max_items){
// takes parts and creates a full path - should do some more testing, for a max_number of items
	Integer items;
	Get_number_of_items(parts,items);
	if(items > max_items)
		items = max_items;
	if(items == 0){
		return(0);
	}
	path = "";
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t;
		Get_item(parts,i,t);
		if (i != 1 ){
			path += "\\" + t;
		} else {
			path = t;
		}
	}
	return(1);
}


Text get_file_path_filename(Text full_path){
//uses function overload, get just the file name from a path
	Text t;
	Dynamic_Text parts;
	Integer i = get_file_path_parts(full_path,parts);
	if(i!=0){
		Get_item(parts,i,t);
	}
	return(t);
}

Integer get_file_path_filename(Text full_path,Text &path, Text &file){
//uses function overload, get just the file name from a path
	Dynamic_Text parts;
	Integer i = get_file_path_parts(full_path,parts);
	if(i!=0){
		Get_item(parts,i,file); //the file is the last part
		for(Integer j=1;j<i;j++){ //note : j<i, not j<=i
			Text t;
			Get_item(parts,j,t);
			path+=t+"\\";
		}
		return(0); //success
	}
	return(-1);//bad
}

Text get_parent(Text full_path){
	Dynamic_Text parts;
	Integer i = get_file_path_parts(full_path,parts);
	if(i>1){
		Text t;
		Get_item(parts,i-1,t); //this is the parent
		return t;
	}
	return(".");//bad
}

Integer get_extension(Text any_path, Text &extension){
// get the extension from a path
	extension = "";
	Text path, file;
	if(get_file_path_filename(any_path,path,file)){
		return 3;
	}
	//need to find the last .
	Integer f = '.';
	Integer len = Text_length(file);
	if(len == 0)return 2;
	for(Integer i = len;i>0;i--){
		Integer c;
		Get_char(file,i,c);
		if(c==f){
			extension = Get_subtext(file,i+1,len);
			return 0;
		}
	}
	return 1;	// no extension in file
}

Integer get_macro_name(Text &name, Text &path){
// what is my name (ie name.4do), what is my path (for running macor)
	name = Get_macro_name();
	name = find_replace_repeat(name,"/","\\");// pre-process string; replace any / with \\
	replace_env_path(name,name);			//added this, if the macro name has a $var then resolve it
	path = "";
	while(1){
		Integer start = Find_text(name,"\\");
		if(start == 0)break;
		path += Get_subtext(name,1,start);
		name = Get_subtext(name,start+1,Text_length(name));
	}
	return 0;
}

Text get_macro_name_path_text(){
// returns the path/to/this/running/macro.4do
	Text name,path;
	get_macro_name(name,path);
	return (path+name);
}


Text get_macro_name_text(){
// returns the name of this running macro
	Text name,path;
	get_macro_name(name,path);
	return name;
}

Integer get_macro_name_path_parts(Text &path, Text &base_name, Text &extension){
	base_name = Get_macro_name();
	base_name = find_replace_repeat(base_name,"/","\\");// pre-process string; replace any / with \\
	//replace_env_path(name,name);			//added this, if the macro name has a $var then resolve it
	path = "";
	while(1){
		Integer start = Find_text(base_name,"\\");
		if(start == 0)break;
		path += Get_subtext(base_name,1,start);
		base_name = Get_subtext(base_name,start+1,Text_length(base_name));
	}

	extension = "";
	//need to find the last .
	Integer len = Text_length(base_name);
	if(len > 0){
		for(Integer i = len; i>2 ; i--){
			Integer c;
			Get_char(base_name,i,c);
			if(c=='.'){
				extension = Get_subtext(base_name,i+1,len);
				base_name = Get_subtext(base_name,1,i-1);
				break;
			}
		}
	}
	return 0;
}

Text get_dot_project_folder(){
   Text folder;   Get_project_folder(folder);
   Text name;   Get_project_name(name);
   name = find_replace_repeat(name," ","_");
   Text path = folder + "\\" + name + ".project";
   return path;
}

Integer find_12d_projects(Text base_path_no_backslash, Dynamic_Text &project_names, Dynamic_Text &project_paths, Dynamic_Text &dot_project_paths){

	Integer items;
	Text command = "dir \"" + base_path_no_backslash + "\\*project\" /ogn /a-d /s /b";
	Dynamic_Text report = run_system_command_get_output(command);
	Get_number_of_items(report,items);

	Dynamic_Text dt;
	Integer i;

	for(i=1;i<=items;i++){
		Text t;
		Get_item(report,i,t);
		Text needle = ".project\\project";
		Text haystack = Get_subtext(t,Text_length(t)-Text_length(needle)+1,Text_length(t));
		if( haystack == needle ){
			Null(dt);
			//project name
			get_file_path_parts(t,dt);
			Integer dt_items;
			Get_number_of_items(dt,dt_items);
			Text project_name;
			Get_item(dt,dt_items-1,project_name);
			project_name = find_replace(Get_subtext(project_name,1,Text_length(project_name)-8),"_"," ");
			Append(project_name,project_names);
			//project path
			Text project_path;
			set_file_path_parts(dt,project_path,dt_items-2);
			Append(project_path,project_paths);
			//dot project path
			Append(Get_subtext(t,1,Text_length(t)-8),dot_project_paths);
		}
	}
	return(0);
}

Integer find_12d_projects(Integer how_deep_back, Text &return_path, Dynamic_Text &project_names, Dynamic_Text &project_paths, Dynamic_Text &dot_project_paths){
	Get_project_folder(return_path);
	Dynamic_Text parts;
	get_file_path_parts(return_path,parts);
	Integer items;
	Get_number_of_items(parts,items);
	set_file_path_parts(parts,return_path,items-how_deep_back);
	return(find_12d_projects(return_path, project_names, project_paths, dot_project_paths));
}

Integer copy_file(Text source, Text target_path, Integer mode){
// modes
// 0 = do not replace existing
// 1 = copy and do backup
// 2 = copy replace

	Text source_path,source_file;
	//does source file exist?
	if(File_exists(source)){
		//nothing to do
		if( get_file_path_filename(source,source_path, source_file) )return(-10);//dud path
	}else{
		return(-2);
	}

	//resolve $variable name
	resolve_env(source);

	Text target = target_path+source_file;
	//does the target exist ?
	if(File_exists(target)){

		if(mode==1){
			Backup_version_file(target,1);//0 gives an error message once 100 backups are reached
		}
		if(mode==1 || mode==2){
			if(File_delete(target)!=-1)return(-3); //could not delete existing file (-1 means success, not as per 12d help)
		}
		if(mode==0){
			return(1);
		}
	}

	Text cmd = "copy \"" + source + "\" \"" + target +"\"";
	run_system_command(cmd);
	Print("Executing: " + cmd + "\n");

	return(0);

}

Integer copy_file_to_project(Text source, Integer mode){
	Text target_path;
	Get_project_folder(target_path);
	return(copy_file(source, target_path+"\\", mode));
}

Integer copy_file_to_project(Text source){
	return(copy_file_to_project(source,2));
}

Integer copy_file(Text source, Text target){
	//the arguments may have been "quoted" or 'quoted'
	source = quotes_strip(source);
	target = quotes_strip(target);
	//if character 1 == $ then
	//Text env = Getenv("USER_LIB_4D"); // could expand the code here

	//chuck text into some ""
	source = quotes_add(source);
	target = quotes_add(target);
	Text cmd = "copy " +  source + " " + target ;
	System(cmd);
	Print("Executing: " + cmd + "\n");
	return 1; // return
}

Integer copy_file(Text source){
	// if only the location is defined then copy to the current project directory
	Text target;
	Get_project_folder(target);
	copy_file(source,target);
	return 1;
}

Integer folder_exists(Text folder_name){
//--- function to check if a directory exists
	Message_Box msge = Create_message_box("") ;
	Directory_Box dbox = Create_directory_box("directory",msge,CHECK_DIRECTORY_MUST_EXIST);
	Set_data(dbox,folder_name);

	Text result;
	Integer doesit = Validate(dbox,GET_DIRECTORY, result) ;

	if (doesit == NO_DIRECTORY ) { return(0);  }//does not exist
	else if ( doesit == DIRECTORY_EXISTS ) { return(1);  }//does exist
	else if ( doesit == NEW_DIRECTORY ) { return(0);  }//does not exist
	else   { return(0);  }//does not exist

	Print("\nFolder error:" + folder_name + "\n" + result + "\n" + To_text(doesit)+ "\n" );

	return(2); //failed
}

Integer open_file_save_list(Text file_name,Dynamic_Text &file_contents ){
//function to save contents of a text file in dynamic list
	File file;
	if(File_open(file_name,"r",file) !=0)return -1;//there was an error opening the file
	Integer lines = 0;
	  while(1) {
		  Text  fname;
		  if(File_read_line( file, fname) == -1) {
			  File_close(file);
			  return(lines);
		  }
		  else {
		    Append(fname,file_contents);
		    ++lines;
		  }
	  }
	File_close(file);
	return(0);
}

Integer read_file(Text file_name,Dynamic_Text &file_contents ){
//function to save contents of a text file in dynamic list
	File file;
	if(File_open(file_name,"r",file) !=0)return 1;//thee was an error opening the file
	Integer lines = 0;
	while(1) {
		  Text  t;
		  if(File_read_line( file, t) == -1) {
			  File_close(file);
			  return(2);
		  }else {
			Append(t,file_contents);
		    ++lines;
		  }
	}
	File_close(file);
	return(0);
}

Integer open_file_write_list(Text file_name,Dynamic_Text &contents_byline)
{
//function to save the contyents of a dynamic list to a text file
	File file;
	File_open(file_name,"w+",file);
	Integer no_items;
	Get_number_of_items(contents_byline,no_items);
	for(Integer i = 1;i<=no_items;i++){
		Text txt;
		Get_item(contents_byline,i,txt);
		File_write_line(file,txt);
	}
	File_close(file);
	return 1;
}

Integer file_write(Text file_name,Text text){
//function to save the contyents of a dynamic list to a text file
	File file;
	File_open(quotes_strip(file_name),"w+",file);
	File_write_line(file,text);
	File_close(file);
	return 0;
}

Integer file_write(Text file_name,Dynamic_Text &contents_byline){
//function to save the contyents of a dynamic list to a text file
	File file;
	File_open(quotes_strip(file_name),"w+",file);
	Integer no_items;
	Get_number_of_items(contents_byline,no_items);
	for(Integer i = 1;i<=no_items;i++){
		Text txt;
		Get_item(contents_byline,i,txt);
		File_write_line(file,txt);
	}
	File_close(file);
	return 0;
}

//practise file write line
Integer open_file_write_list_practise(Text filename,Dynamic_Text &contents_byline)
{
	//function to save the contyents of a dynamic array to a text file
	//File file;
	//File_open(file_name,"w+",file);
	Integer no_items;
	Get_number_of_items(contents_byline,no_items);
	for(Integer i = 1;i<=no_items;i++){
		Text txt;
		Get_item(contents_byline,i,txt);
		txt = find_replace(txt,">","^>");//need to add the escape character
		txt = find_replace(txt,"<","^<");//need to add the escape character
		Text cmd = "echo " + txt + ">> \""+filename+"\"";
		Create_process("cmd.exe", " /Q /E:ON /C "+cmd, "", CREATE_NO_WINDOW, 1, 0); //must wait for the command to finish otherwise
	}
	return 1;
}


Integer file_write_lines(Text file_name,Dynamic_Text dt, Text mode)
{
	//function to open a file with 'mode' and write dynamic text 'dt'
	File file;
	Integer ierr;
	ierr = File_open(file_name,mode,file);
	if(ierr !=0)return ierr;
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		File_write_line(file,t);
	}
	File_close(file);
	return 0;
}


Integer file_write_lines(Text file_name, Text t)
{
	//function overload
	Dynamic_Text dt;
	Append(t,dt);
	Integer ierr = file_write_lines(file_name,dt,"w+");
	return ierr;
}

Integer file_write_lines(Text file_name, Dynamic_Text dt)
{
	//function overload
	Integer ierr = file_write_lines(file_name,dt,"w+");
	return ierr;
}

Integer file_append_lines(Text file_name, Dynamic_Text dt)// !! this function name changed somehow???
{
	//function overload
	Integer ierr = file_write_lines(file_name,dt,"a+");
	return ierr;
}

// function that gets all files in a folder or all subfolders and
// returns their names and number of files
// files_or_folders = 1 Folders only
// files_or_folders = 2 FILES only
// files_or_folders = 3 Both
Integer get_list_of_files(Text base_path, Text wildcard, Integer files_or_folders, Dynamic_Text &file_names){

	Text cmd_call;
	if ( files_or_folders == 1 ) {  cmd_call = "  /OGN   /b /ad";  }
	else if ( files_or_folders == 2 ) {  cmd_call = "  /ON   /b /a-d";  }
	else {  cmd_call = "  /OGN  /b";  }

	Text cmd = "DIR \"" + base_path + "\\" +wildcard+"\"" +  cmd_call;

	file_names = run_system_command_get_output(cmd);
	Integer items;
	Get_number_of_items(file_names,items);
	return items;
}

Dynamic_Text get_list_of_files(Text base_path, Text wildcard){
  Dynamic_Text dt;
  get_list_of_files(base_path, wildcard, 2, dt);
  return dt;
}

Dynamic_Text get_list_of_folders(Text base_path, Text wildcard){
  Dynamic_Text dt;
  get_list_of_files(base_path, wildcard, 1, dt);
  return dt;
}

// function that gets all files in a folder or all subfolders and
// returns their names and number of files
// files_or_folders = 1 Folders only
// files_or_folders = 2 FILES only
// files_or_folders = 3 Both
Integer get_list_of_files(Dynamic_Text &file_names, Text &folder_name, Text &projectfoldername, Text file_ext,Integer files_or_folders){
 Print (folder_name + " " + projectfoldername + " " + file_ext + " " + To_text( files_or_folders) + "\n");
	Text cmd_call;
	if ( files_or_folders == 1 ) {  cmd_call = "  /OGN   /b /ad >  ";  }
	else if ( files_or_folders == 2 ) {  cmd_call = "  /ON   /b /a-d >  ";  }
	else {  cmd_call = "  /OGN  /b >  ";  }

	Text temp_file;
	Integer ran = Get_random_number       ();
	temp_file = "tmpfldrlist_" + To_text(ran) + ".txt";

	Text tmp = "DIR \"" + folder_name + "\\" +file_ext+"\"" +  cmd_call  +  "\"" + projectfoldername+"\\"+temp_file+"\"";
	Create_process("cmd","   /Q /E:ON /C " + tmp,projectfoldername,0,1,0);

	Integer no_fldrs=0, num_files = 0;
  //--- make a dynamic array
	if ( File_exists(temp_file)  != 0 ) {
		Dynamic_Text contents_byline;
		num_files = open_file_save_list(temp_file,contents_byline );
		Append(contents_byline,file_names);
		//--- clean up and delete temp files
		no_fldrs += num_files;

		Null(contents_byline);
		File_delete(temp_file);
	} //if

	return(num_files);
}

Integer is_path_relative(Text t){
	Integer c1 = 0, c2 = 0, c3 = 0;
	Integer length = Text_length(t);
	if(length<2)return 0; //quit if not relative
	Get_char(t,1,c1);
	if(c1 == '.'){
		Get_char(t,2,c2);
		if(c2=='.'){
			if(length >2){
				Get_char(t,3,c3);
				if(c3 == '\\' || c3 == '/'){
					return 2; // ../ or ..\
				}
			}
		}else if(c2 == '\\' || c2 == '/'){
			return 1;  // .\  or ./
		}
	}
	return 0;
}

void remove_items_from_list(Dynamic_Text &dt, Integer i1,Integer i2){
	Dynamic_Text out;
	Integer n;
	Get_number_of_items(dt,n);
	for(Integer i=1;i<=n;i++){
		if(i == i1 || i == i2){
//			Print("skipp");p(i);
		}else{
			Text t;
			Get_item(dt,i,t);
			Append(t,out);
		}
	}
	Null(dt);
	Append(out,dt);
}

#if 1
Integer resolve_relative_path(Text t_in, Text &t_out){
	if(is_path_relative(t_in)){
		Text project_folder;
		Get_project_folder(project_folder);
		t_out = project_folder +"\\" +t_in;
		Integer doit = 1;
		Integer max_count = 100;  // a bit of protection
		Integer counter = 0;
		while(doit){
			Dynamic_Text parts;
			get_file_path_parts(t_out,parts);
			Integer items;
			Get_number_of_items(parts,items);
			if(items<1)break;
			Integer done = 1;
			for(Integer i=2;i<=items;i++){
				Text t;
				Get_item(parts,i,t);
				Integer ret = is_path_relative(t+"/");
				//p(i);
				if(ret == 1){
					remove_items_from_list(parts,i,i);
					done = 0;
					break;
				}else if(ret ==2){
					remove_items_from_list(parts,i-1,i);
					done = 0;
					break;
				}
			}

			set_file_path_parts(parts,t_out);
			//p(t_out);
			if(done){
				doit = 0;
			}
			counter++;
			if(counter > max_count){
				doit=0;
			}
		}

	}else{
		t_out = t_in;
	}
	return 0;
}
#else
Integer resolve_relative_path(Text t_in, Text &t_out){
	// how many directories back for the t_in?
	Integer count_back = 0;
	Integer start_char = 1;
	Integer last_char = Text_length(t_in);
	Integer relative_path_found =0;

	Integer ret = is_path_relative(Get_subtext(t_in,start_char,last_char));
	if(ret == 1){ // './'
		relative_path_found++;
		start_char+=2;
	}else if(ret == 2){ // '../'
		relative_path_found++;
		start_char+=3;
		count_back++;
	}

	if(relative_path_found){
		Dynamic_Text parts_out;
		Dynamic_Text parts_current;
		Text project_folder;
		Get_project_folder(project_folder);
		get_file_path_parts(project_folder,parts_current);
		Integer mparts;
		Get_number_of_items(parts_current,mparts);
		mparts-=count_back;

		for(Integer ii=1;ii>=mparts;ii++){
			Text t;
			Get_item(parts_current,ii,t);
			Append(t,parts_out);
		}

		//get current folder parts;
		Dynamic_Text parts_in;
		get_file_path_parts(t_in,parts_in);
		Integer nparts;
		Get_number_of_items(parts_in,nparts);
		for(Integer i=count_back+1;i<=nparts;i++){
			Text t;
			Get_item(parts_in,i,t);
			Append(t,parts_out);
		}
		set_file_path_parts(parts_out,t_out);
	}else{
		t_out = t_in;
	}
	return 0;
}
#endif

Integer valid_path(Text path, Text &fixed){
// make a path valid, resolving $vars
	Dynamic_Text parts_dt;
	get_file_path_parts(path,parts_dt);
	Integer nparts;
	Get_number_of_items(parts_dt,nparts);
	fixed = path;
	if(nparts < 1)return 1;
	fixed = "";
	Text parts[nparts];
	Integer possible_errors=0;
	for(Integer i=1;i<=nparts;i++){			// expand $vars
		Get_item(parts_dt,i,parts[i]);
		Integer c1;
		Get_char(parts[i],1,c1);
		if(c1=='$'){
			Text suffix = Get_subtext(parts[i],2,Text_length(parts[i]));
			Text this_env = Getenv(Text_upper(suffix));
			if(this_env == ""){
				this_env = Getenv(Text_upper(suffix)+"_4D");//some environment variables are suffixed *_4D, do a search for them
			}
			if(this_env != ""){
				parts[i] = this_env;
			}else{
				//hack
				if(Text_lower(parts[i]) == "$working" ){
					Get_project_folder(parts[i]);
				}else{
					// do nothing
					possible_errors++;
				}
			}
		}
	}
	Null(parts_dt);
	for(i=1;i<=nparts;i++){ // walk the path
		Append(parts[i],parts_dt);
	}
	set_file_path_parts(parts_dt,fixed); // this could have ..\..\ in there
	Null(parts_dt);
	get_file_path_parts(fixed,parts_dt);
	Get_number_of_items(parts_dt,nparts);
	if(nparts < 1)return 1;
	fixed = "";
	Text parts2[nparts];

	Integer this_part_idx = 0;
	for(i=1;i<=nparts;i++){ 			// walk the path and resolve ..\..\..
		Get_item(parts_dt,i,parts2[i]);
		if(parts2[i]==".")continue;
		if(parts2[i]==".."){
			if(this_part_idx>0){
				this_part_idx--;
			}
			continue;
		}
		this_part_idx++;
		parts2[this_part_idx] = parts2[i];
	}
	Null(parts_dt);
	for(i=1;i<=this_part_idx;i++){
		Append(parts2[i],parts_dt);
	}
	set_file_path_parts(parts_dt,fixed);
	return possible_errors;
}

Text valid_path(Text path){
	// this will make c:\path//a\$env\c\..\\\..\..\file.txt => c:\path\file.txt, also
	// will resolve $end within any part of the path
	Text fixed;
	valid_path(path,fixed);
	return fixed;
}

Text force_last_char_in_path(Text t){
	Integer l=Text_length(t);
	Integer c;
	Get_char(t,l,c);
	if(c == '\\'){
		return t;
	}else if(c == '/'){
		return Get_subtext(t,1,l-1)+"\\";
	}
	return t + "\\";
}

Text real_path(Text path_in){
	Text path_out;
	valid_path(path_in,path_out);
	resolve_relative_path(path_out+"\\",path_out);
	return force_last_char_in_path(path_out);
}

Integer get_all_sub_folders(Text start_path, Text wildcard, Dynamic_Text &full_path_names_with_slash){
	Text base_path_with_slash = real_path(start_path); // this resolves env vars and slashes and makes sure there is a slash at the end
	//while(1){
		Dynamic_Text this_subfolders;
		Integer items = get_list_of_files(base_path_with_slash,wildcard,1,this_subfolders); // search for folders only
		if(items > 0){
			for(Integer i=1;i<=items;i++){
				Text t;
				Get_item(this_subfolders,i,t);
				t=base_path_with_slash+t+"\\";
				Append(t,full_path_names_with_slash);
				get_all_sub_folders(t,wildcard,full_path_names_with_slash);
			}
		}
		//else{
		//	break;
		//}
	//}
	return(0);
}


Integer resolve_env(Text source, Text &output){
	Integer ret_val = 0;
	Text prefix ="";
	source = find_replace(source,"/","\\");
	if(Find_text(source,"\"")==1){
		source = Get_subtext(source,2,Text_length(source));
		prefix = "\"";
	}
	if(Find_text(source,"$") == 1){
		//if $ is the first character we need to find resolve the environment variable via getenv
		Text env;
		if(Find_text(source,"\\") != 0 ){
			Integer i = Find_text(source,"\\");
			if(i < 1){
				i = Text_length(source);
			}
			env = Get_subtext(source,2,i-1);
			Text source_path = Getenv(Text_upper(env)); // make the users variable uppercase
			if(source_path == ""){
				source_path = Getenv(Text_upper(env)+"_4D");//some environment variables are suffixed *_4D, do a search for them
			}
			source = source_path + Get_subtext(source,i,Text_length(source));
		}else {
			//there is an error !
			Print("There was an error with the source " + source );Print();
			Print("it may not contain a valid environment variable" );Print();
			ret_val = -1;
			//return;
		}
	}
	output = prefix + source;
	return (ret_val);
}



Integer resolve_env(Text &source){
	Text output;
	Integer ret_val = resolve_env(source,output);
	source = output;
	return (ret_val);
}



Integer directory_exists(Text directory_name){ // TODO - 
// http://forums.12dmodel.com/viewtopic.php?t=846
//Description
//Checks to see if a file of name file_name exists.
//A non-zero function return value indicates the file exists.
//A zero function return value indicates the file does not exist.
//Warning - this is the opposite to most 4DML function return values
   Message_Box message = Create_message_box("") ;
   Directory_Box directory_box = Create_directory_box("directory",message,CHECK_DIRECTORY_MUST_EXIST);
   Set_data(directory_box,directory_name);

   Text result;
   Integer ierr = Validate(directory_box,GET_DIRECTORY, result) ;

   if (ierr == NO_DIRECTORY )  return(0);		//does not exist
   else if ( ierr == DIRECTORY_EXISTS ) return(1);	//exists
   else if ( ierr == NEW_DIRECTORY ) return(0);		//does not exist
   else return(0);					//does not exist

   return(0); //failed
}

Integer get_std_folders(Text &lib_folder,Text &setups_folder,Text &userlib_folder,Text &user_folder,Text &projectname ,Text &projectdata_12_folder,Text &projectfoldername )
// --------------------------------------
// this returns standard folder locations
// --------------------------------------
{
	// get project name and project data folder name
	// pname is the data folder name where no spaces are allowed
	// projectname includes spaces
	Text pname;
	Get_project_name(pname);
	projectname = pname;

	// network path to project folder
	Get_project_folder(projectfoldername);

	// data folder network path used by 12d solutions --- dont use for user data
	//MASHY CHANGED TO NEXT LINE change_spaces(pname); // replaces spaces in project names
	pname =find_replace_repeat(pname," ","_"); //MASHY CHANGED THIS LINE REALLY SHOULD CHECK OUTPUT !!!!!!!!!!!!!!!!
	Print("CHECK THIS OUTPUT, NO SPACES ->  " + pname + "\n");
	projectdata_12_folder = projectfoldername + "\\"  + pname + ".project";

	// get all stadard folders
	setups_folder = replace_env_4d("$SET_UPS","SET_UPS");
	lib_folder = replace_lib_4d("$LIB");
	userlib_folder = replace_user_lib_4d("$USER_LIB");
	user_folder = replace_user_4d("$USER");
	return(0);
}

Integer find_file_std_folders(Text &file_name,Text &path_file){
//--- find and return file name with path to locate it in one of the standard locations
//--- first project -> user_lib -> User -> 12d lib -> setups/images
//--- must INCLUDE standard_library_paths.h file
//--- return value 1 indicates it did find a match
//--- return value 0 indicates no match found - error
  Text lib_folder,setups_folder,userlib_folder,user_folder,projectname ,projectdata_12_folder,projectfoldername;
  get_std_folders(lib_folder,setups_folder, userlib_folder,user_folder,projectname ,projectdata_12_folder,projectfoldername);

	if ( File_exists(file_name) != 0 ) { path_file = file_name; return(1); }
	else if ( File_exists(projectfoldername +"\\" + file_name) != 0 )   { path_file =  projectfoldername +"\\" + file_name ; return(1); }
	else if ( File_exists(userlib_folder +"\\" + file_name) != 0 ) { path_file =  userlib_folder +"\\" + file_name ;return(1); }
	else if ( File_exists(user_folder +"\\" + file_name) != 0 )    { path_file =  user_folder +"\\" + file_name ; return(1); }
	else if ( File_exists(user_folder +"\\Images" +"\\" + file_name) != 0 )    { path_file =  user_folder +"\\Images" +"\\" +  file_name ; return(1); }
	else if ( File_exists(lib_folder +"\\" + file_name) != 0 )     { path_file =  lib_folder +"\\" + file_name ; return(1); }
	else if ( File_exists(setups_folder +"\\" + file_name) != 0 )     { path_file =  setups_folder +"\\" + file_name ; return(1); }
	else if ( File_exists(setups_folder +"\\Images" +"\\" + file_name) != 0 )     { path_file =  setups_folder +"\\Images" +"\\" + file_name ; return(1); }
	else { file_name = "\n" + "failed to find file " + file_name  + " - macro failed check you have all icon files" + "\n";  return(0); }
	return(0);
}
//--------------------------------------------------------------------------------
//                        Some old shit - very prelim below
//--------------------------------------------------------------------------------

Text my_resolve_env(Text &text){
	// preprocess -
	Text t = text;
	t = find_replace_repeat(t,"\\","/"); // all \ should now be /

	while(1){
		Integer pos1 = Find_text(t,"$");
		if(pos1 <= 0)break;
		// get next space, eol, /
		Integer pos2 = pos1;
		Integer length = Text_length(t);
		for(Integer i=pos1+1;i<=length;i++){ // scan
			Integer c1;
			Get_char(t,i,c1);
			if(c1 == ' ' || c1 == '/'){

			}
		}

	}
	Text source_path = Getenv(Text_upper(t)); // make the users variable uppercase
	if(source_path == ""){
		source_path = Getenv(Text_upper(t)+"_4D");//some environment variables are suffixed *_4D, do a search for them
	}
	return t;
}

Integer resolve_env(Dynamic_Text &envs, Dynamic_Text &resolved){
	Integer items;
	Get_number_of_items(envs,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(envs,i,t);
	}
	return 0;
}

Integer get_std_search_paths(Integer do_librarys, Integer do_setups, Dynamic_Text &dt){
	Dynamic_Text envs_to_replace;
	if(do_librarys)	Append("$CUSTOMER_LIB_4D",envs_to_replace);	// customer lib
	if(do_setups)	Append("$CUSTOMER_USER_4D",envs_to_replace);	// customer setups
	if(do_librarys)	Append("$LIB_4D",envs_to_replace);		// user lib
	if(do_setups)	Append("$USER_4D",envs_to_replace); 		// user setups
	if(do_librarys)	Append("$USER_LIB_4D",envs_to_replace);		// 12d lib
	if(do_setups)	Append("$SET_UPS_4D",envs_to_replace);		// 12d setups
	Integer items;
	return 0;
}

Text get_common_prefix_and_normalise_paths(Text &path1, Text &path2){
	path1 = find_replace(path1,"\\","/");
	path2 = find_replace(path2,"\\","/");
	Integer len1 = Text_length(path1);
	Integer len2 = Text_length(path2);


	if(len1<len2)len1=len2;

	for(Integer i=1;i<=len1;i++){
		Integer c1,c2;
		Get_char(path1,i,c1);
		Get_char(path2,i,c2);
		if(c1!=c2){
			i=i-1;
			break;
		}
	}

	Text t = "";

	if(i>1){
		t = Get_subtext(path1,1,i);
	}

	return t;
}

Integer get_relative_path(Text start_path, Text dest_path, Text &relative_path){
	relative_path = dest_path;
	Text common_path = get_common_prefix_and_normalise_paths(start_path,dest_path);
	if(common_path == ""){
		 // start does not match
		return -1; //
	}
	// trim up the strings to where they are different
	start_path = Get_subtext(start_path,Text_length(common_path)+1,Text_length(start_path));
	dest_path = Get_subtext(dest_path,Text_length(common_path)+1,Text_length(dest_path));
	//p("SP",start_path);
	//p("DP",dest_path);

	relative_path = dest_path;
	Dynamic_Text start_parts;
	if(!get_file_path_parts(start_path,start_parts))return 1;
	Integer items;
	Get_number_of_items(start_parts,items);
	for(Integer i=1;i<=items;i++){
		relative_path = "../" + relative_path;
	}
	return 0;
}

#endif
