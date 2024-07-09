#ifndef mashy_lib_widgest_validate_H_included
#define mashy_lib_widgest_validate_H_included

#include "set_ups.h"
#include "mashy_lib_model_functions.h"

//validate boxes
//must exist returns 0 for success
//check exist returns 1 if a valid value was entered, 0 if none was entered - nulls the value if possible
//must not exist - makes sure that the object type does not already exist in the database (view,tin,model,ect) returns 1 if already exists, 0 otherwise

Integer set_focus(Widget widget){
	Integer ierr=0;
	ierr+= Set_cursor_position(widget);
	ierr+= Set_focus(widget);
	return(ierr);
}

//	MODEL BOX

Integer must_exist(Model_Box box, Model &model, Text &model_name, Message_Box message){
// validates a model box, makes sure the model exists
// 1 = error
	Integer ierr = Validate(box,NEW_MODEL,model);
	if(ierr!=MODEL_EXISTS){
		if (ierr==NO_NAME){
			Set_data(message,"Model name is blank!");
			set_focus(box);
			return(NO_NAME);
		}
		Set_data(message,"Model does not exist!");
		set_focus(box);
		return(NO_MODEL);
	}
	if(Get_name(model,model_name))return(1);
	return (0); //success
}

Integer check_exist_create(Model_Box box, Model &model, Text &model_name, Message_Box message){
// validates a model box, makes sure the model exists
// 1 = error
	Integer ierr = Validate(box,NEW_MODEL,model);
	if(ierr!=MODEL_EXISTS){
		if (ierr==NO_NAME){
			Set_data(message,"Model name is blank!");
			set_focus(box);
			return(NO_NAME);
		}
		Get_data(box,model_name);
		Valid_model_name(model_name,model_name);
		Set_data(box,model_name);
		model = Get_model_create(model_name);
		if ( is_null(model) ){
			Set_data(message,"Model not valid!");
			set_focus(box);
			return(NO_MODEL);
		}
		Set_data(message,"Model <"+model_name+"> created");
		return (0);
	}
	if(Get_name(model,model_name))return(1);
	return (0); //success
}

//	TIN BOX

Integer must_exist(Tin_Box box, Tin &tin, Text &tin_name, Message_Box message){
// validates a tin box, makes sure the tin exists
// 1 = error
	if(Validate(box,CHECK_TIN_MUST_EXIST,tin) != TIN_EXISTS){
		Set_data(message,"Tin does not exist");
		set_focus(box);
		return(1);
	}
	if(Get_name(tin,tin_name))return(2);
	return(0);
}

Integer must_not_exist(Tin_Box box, Tin &tin, Text &tin_name, Message_Box message){
// validates a tin box, makes sure the tin does not exists
// 1 = error
	if(Validate(box,CHECK_TIN_MUST_EXIST,tin) == TIN_EXISTS){
		Set_data(message,"Tin already exists!");
		Get_name(tin,tin_name);
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer check(Tin_Box box, Tin &tin, Text &tin_name, Message_Box message){
// validates a tin box, checks if the tin exists
// 1 = error
	if(Validate(box,CHECK_TIN_MUST_EXIST,tin) != TIN_EXISTS){
		return(1);
	}
	if(Get_name(tin,tin_name))return(2);
	return(0);
}


//	DIRECTORY BOX
Integer must_exist(Directory_Box box, Text &folder, Message_Box message){
	if(Validate(box,GET_DIRECTORY_ERROR,folder) != DIRECTORY_EXISTS){
		Set_data(message,"Folder does not exist!");
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer must_not_exist(Directory_Box box, Text &folder, Message_Box message){
	if(Validate(box,GET_DIRECTORY_ERROR,folder) == DIRECTORY_EXISTS){
		Set_data(message,"Folder already exist!");
		set_focus(box);
		return(1);
	}
	return(0);
}

//	VIEW BOX

Integer must_exist(View_Box box, View &view, Text &view_name, Message_Box message){
	if(Validate(box,CHECK_VIEW_MUST_EXIST,view) != VIEW_EXISTS){
		Set_data(message,"View does not exist!");
		set_focus(box);
		return(1);
	}
	if(Get_name(view,view_name))return(2);
	return(0);
}

Integer must_not_exist(View_Box box, View &view, Text &view_name, Message_Box message){
	if(Validate(box,CHECK_VIEW_MUST_EXIST,view) == VIEW_EXISTS){
		Set_data(message,"View already exist!");
		set_focus(box);
		if(Get_name(view,view_name))return(2);
		return(1);
	}
	return(0);
}

//	FILE BOX
Integer must_exist(File_Box box, Text &file_name, Message_Box message){
		if(Validate(box,GET_FILE_MUST_EXIST,file_name) != FILE_EXISTS) {
			file_name = "";
			set_focus(box);
			return(1);
		}
	return 0;
}

Integer check_exist(File_Box box, Text &file_name, Message_Box message){
	if(Validate(box,GET_FILE_APPEND,file_name) == NO_FILE_ACCESS) {
		file_name = "";
		set_focus(box);
		return(1);
	}
	if(file_name == ""){
		set_focus(box);
		return(2);
	}
	return 0;
}


Integer check(File_Box box, File &file, Text &file_name, Text rwa, Message_Box message){
//this will prompt the user
//THIS IS DANGEROUS TO BE OPENING THE FILE HERE - MUST ENSURE THE FILE IS CLOSED PROPERLY!!!
	switch ( Text_lower(rwa) ) {
		case "r" : {
			if(Validate(box,GET_FILE_MUST_EXIST,file_name) != FILE_EXISTS) {
				set_focus(box);
				return(1);
			}
		} break;
		case "w" : {
			if(Validate(box,GET_FILE_CREATE,file_name) != NO_FILE) {
				set_focus(box);
				return(1);
			}
		} break;
		case "a" : {
			if(Validate(box,GET_FILE_APPEND,file_name) != FILE_EXISTS) {
				set_focus(box);
				return(1);
			}
		} break;

	}
	return ( File_open(file_name,rwa,file) );
}


//	REAL BOX

Integer must_exist(Real_Box box, Real &value, Message_Box message){
	if(Validate(box,value) != 1){
		Set_data(message,"blank not valid!");
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer check(Real_Box box, Real &value, Message_Box message){
	if(Validate(box,value) != 1){
		Null(value);
		return(1);
	}
	return(0);
}

//	INTEGER BOX

Integer must_exist(Integer_Box box, Integer &value, Message_Box message){
	if(Validate(box,value) != 1){
		Set_data(message,"Invalid integer!");
		set_focus(box);
		return(1);
	}
	return(0);
}


Integer check(Integer_Box box, Integer &value, Message_Box message){
	if(Validate(box,value) != 1){
		value = 0;
	}
	return(0);
}

//	COLOUR BOX

Integer must_exist(Colour_Box box, Integer &value, Message_Box message){
	if(Validate(box,value) != 1){
		Set_data(message,"Invalid colour!");
		set_focus(box);
		return(1);
	}
	return(0);
}

//	NAME BOX

Integer must_exist(Name_Box box, Text &value, Message_Box message){
	if(Validate(box,value) !=TRUE){
		Set_data(message,"blank not valid!");
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer check(Name_Box box, Text &value, Message_Box message){
	if(Validate(box,value) == TRUE){
		return(0);
	}
	value = "";
	return(0);
}

//	SOURCE BOX

Integer must_exist(Source_Box box, Dynamic_Element &delt, Message_Box message){
	if(Validate(box,delt) != TRUE) {
		Set_data(message,"data not valid!");
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer check(Source_Box box, Dynamic_Element &delt, Message_Box message){
	Validate(box,delt);
	return(0);
}


//	FUNCTION BOX

// returns 1 if a function is created or exists, returns 0 if there was an error or no function name exists
Integer check(Function_Box box, Text &value, Message_Box message){
	Get_data(box,value);
	if(value!=""){
		Macro_Function macro_function;
		if( Get_macro_function(value,macro_function) ){
			if( Create_macro_function(value,macro_function) ){
				Error_prompt("failed to create function!");
				return 0;
			}

		}
	}else{
		return (0);
	}
	return (1);
}

Integer check(Function_Box box, Macro_Function &macro_function, Message_Box message){
	Text value;
	if( check(box,value,message) ){
		Integer ok = Get_macro_function(value,macro_function);
	}else{
		return (0);
	}
	return (1);
}

//	SYMBOL BOX

Integer must_exist(Symbol_Box box, Text &value, Message_Box message){
	if(Validate(box,CHECK_LINESTYLE_MUST_EXIST,value) !=LINESTYLE_EXISTS){
		Set_data(message,"symbol not valid!");
		set_focus(box);
		return(1);
	}
	return(0);
}

//	INPUT BOX

Integer must_exist(Input_Box box, Text &value, Message_Box message){
	if(Validate(box,value) !=TRUE){
		Set_data(message,"blank not valid!");
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer check(Input_Box box, Text &value, Message_Box message){
	if(Validate(box,value) == TRUE){
		return(0);
	}
	value = "";
	return(0);
}

//	TEXT EDIT BOX

Integer must_exist(Text_Edit_Box box, Text &value, Message_Box message){
	value = "";
	Get_data(box,value);
	if(value == "")return 1;
	return 0;
}

Integer check(Text_Edit_Box box, Text &value, Message_Box message){
	value = "";
	Get_data(box,value);
	return(0);
}

//	CHOICE BOX

Integer must_exist(Choice_Box box, Text choices[], Integer num_choices, Text &choice, Message_Box message){
//the choice is TEXT
	Text data;
	Get_data(box,data);
	for(Integer i=1;i<=num_choices;i++){
		if(data == choices[i]){
			choice = data;
			return(0);
		}
	}
	Set_data(message,"choice not valid!");
	set_focus(box);
	return (1);
}


Integer must_exist(Choice_Box box, Text choices[], Integer num_choices,Integer &choice, Message_Box message){
//the choice is INTEGER
	Text data;
	Get_data(box,data);
	for(Integer i=1;i<=num_choices;i++){
		if(data == choices[i]){
			choice = i;
			return(0);
		}
	}
	Set_data(message,"choice not valid!");
	set_focus(box);
	return (1);
}

//	TEMPLATE BOX

Integer must_exist(Template_Box box, Text &value, Message_Box message){
	if(Validate(box,CHECK_TEMPLATE_MUST_EXIST,value) !=TEMPLATE_EXISTS){
		Set_data(message,"template does not exist!");
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer must_not_be_new(Template_Box box, Text &value, Message_Box message){
	if(Validate(box,CHECK_TEMPLATE_MUST_EXIST,value) == NEW_TEMPLATE){
		value = "";
		Set_data(message,"template does not exist!");
		set_focus(box);
		return(1);
	}
	return(0);
}


//	NEW SELECT BOX

Integer must_exist(New_Select_Box box, Element &e, Message_Box message){
	if(Validate(box,e) != 1){
		Set_data(message,"string does not exist!");
		set_focus(box);
		return(1);
	}
	return(0);
}
Integer check(New_Select_Box box, Element &e, Message_Box message){
	if(Validate(box,e) != 1){
		Set_data(message,"string does not exist!");
		set_focus(box);
		return(1);
	}
	return(0);
}

//	NAMED TICK BOX
Integer check(Named_Tick_Box box, Integer &value, Message_Box message){
	Validate(box,value);
	return (0);
}

Integer is_ticked(Named_Tick_Box box){
	Integer value;
	Validate(box,value);
	return value;
}

//	TARGET BOX

Integer must_exist(Target_Box box, Integer &mode, Text &data, Message_Box message){
#if VERSION_4D >= 1000
	if(Validate(box,mode,data)){
		Set_data(message,"invalid target!");
		set_focus(box);
		return 1;
	}
#endif
	return 0;
}

//	TEXTSTYLE BOX's

Integer check(Text_Style_Box box, Text &value, Message_Box message){
	value = "";
	Get_data(box,value);
	return(0);
}

Integer check(Textstyle_Data_Box box, Text &value, Message_Box message){
	value = "";
	Get_data(box,value);
	return(0);
}

Integer check(Textstyle_Data_Box box, Textstyle_Data &value, Message_Box message){
	Null(value);
	Get_data(box,value);
	return(0);
}

//	NEW XYZ BOX
Integer check(New_XYZ_Box box, Real &x, Real &y, Real &z, Message_Box message){
	Integer ierr = Validate(box,x,y,z);
	if(ierr == NO_NAME || ierr == TRUE){
		return 0;
	}else{
		Set_data(message,"invalid xyz!");
		set_focus(box);
		return 1;
	}
	return (0);
}

Integer must_exist(New_XYZ_Box box, Real &x, Real &y, Real &z, Message_Box message){
	Integer ierr = Validate(box,x,y,z);
	if(ierr == TRUE){
		return 0;
	}else{
		Set_data(message,"invalid xyz!");
		set_focus(box);
		return 1;
	}
	return (0);
}

//	REAL BOX

Integer must_exist(Angle_Box box, Real &value, Message_Box message){
	if(Validate(box,value) != 1){
		Set_data(message,"blank not valid!");
		set_focus(box);
		return(1);
	}
	return(0);
}

Integer check(Angle_Box box, Real &value, Message_Box message){
	if(Validate(box,value) != 1){
		Null(value);
		return(1);
	}
	return(0);
}

#endif