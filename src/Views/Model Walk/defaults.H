#ifndef macro_defaults_included
#define macro_defaults_included

#include "mashy_lib_text_functions.H"
#include "mashy_lib_files.H"
#include "mashy_lib_system_calls.H"

#define DEFAULTS_FILENAME "VIEW-Model_Walk.def"


Integer write_defaults_file(View_Box &view_box, Input_Box &input_box, Integer &size){
	Text space = " ";
	Dynamic_Text lines;
	Text t;
	Get_data(view_box,t);	Append("VIEW"+space+t,lines);
	Get_data(input_box,t);	Append("PATTERN"+space+t,lines);
				Append("SIZE"+space+To_text(size),lines);
	open_file_write_list(get_dot_project_folder()+"\\"+DEFAULTS_FILENAME,lines);
	return 0;
}

Integer read_defaults_file(View_Box &view_box, Input_Box &input_box, Integer &size){
	Dynamic_Text lines;
	if(open_file_save_list(get_dot_project_folder()+"\\"+DEFAULTS_FILENAME,lines)<0)return 1;
	Integer items;	Get_number_of_items(lines,items);
	for(Integer i = 1 ; i <= items ; i++){
		Text t,var,value;
		Get_item(lines,i,t);
		Dynamic_Text words;
		From_text(t,words);
		Integer numwords;	Get_number_of_items(words,numwords);
		if(numwords<2)continue;
		Get_item(words,1,var);
		Integer pos = Find_text(t,var);
		if(pos == 0 )continue;

		value = text_justify(Get_subtext(t,pos+Text_length(var),Text_length(t)));
		//p("var<"+var+"> value <"+value+">");

		switch(var){

			case "VIEW" : {
				Set_data(view_box,value);
			} break;

			case "PATTERN" : {
				Set_data(input_box,value);
			} break;

			case "SIZE" : {
				 From_text(value,size);
			} break;

		}

	}
	return 0;
}

#endif

