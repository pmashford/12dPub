#ifndef mashy_lib_functions_H_included
#define mashy_lib_functions_H_included

#include "12d/set_ups.H"
#include "mashy_lib_text_functions.H"
#include "mashy_lib_files.H"


Integer get_function_info(Text function_name, Text &type, Integer &created, Integer &updated){
	type = "unknown";	created = 0;	updated = 0;
	if(!Function_exists(function_name))return 1;
	Function function = Get_function(function_name);
	Get_time_created(function,created);
	Get_time_updated(function,updated);

	Message_Box message = Create_message_box("");
	Function_Box function_box = Create_function_box("Function",message,1,RUN_MACRO_T);
	Set_data(function_box,function_name);
	Dynamic_Integer known_function_types;
	Append(APPLY_TEMPLATE_MACRO_T        ,known_function_types);
	Append(APPLY_TEMPLATES_MACRO_T       ,known_function_types);
	Append(INTERFACE_MACRO_T             ,known_function_types);
	Append(TURKEY_NEST_MACRO_T           ,known_function_types);
	Append(KERB_RETURN_MACRO_T           ,known_function_types);
	Append(RETRIANGULATE_MACRO_T         ,known_function_types);
	Append(RUN_MACRO_T                   ,known_function_types);
	Append(STRING_MODIFIERS_MACRO_T      ,known_function_types);
	Append(SURVEY_DATA_REDUCTION_MACRO_T ,known_function_types);
	Append(SIMPLE_MACRO_T                ,known_function_types);
	Append(CREATE_ROADS_MACRO_T          ,known_function_types);
	Append(SLF_MACRO_T                   ,known_function_types);
	Integer items;
	Get_number_of_items(known_function_types,items);
	Integer function_found = 0;
	for(Integer i=1;i<=items;i++){
		Integer i_type;
		Get_item(known_function_types,i,i_type);
		Set_type(function_box,i_type);
		Function func;
		if(Validate(function_box,CHECK_FUNCTION_EXISTS,func) == FUNCTION_EXISTS){
			function_found = i_type;
			break;
		}
	}
	if(function_found){
		switch (function_found) {
			case(APPLY_TEMPLATE_MACRO_T        ) : {
				type = "apply";
			}break;
			case(APPLY_TEMPLATES_MACRO_T       ) : {
				type = "apply many";
			}break;
			case(INTERFACE_MACRO_T             ) : {
				type = "interface";
			}break;
			case(TURKEY_NEST_MACRO_T           ) : {
				type = "turkey nest";
			}break;
			case(KERB_RETURN_MACRO_T           ) : {
				type = "kerb return";
			}break;
			case(RETRIANGULATE_MACRO_T         ) : {
				type = "retriangulate";
			}break;
			case(RUN_MACRO_T                   ) : {
				type = "run";
			}break;
			case(STRING_MODIFIERS_MACRO_T      ) : {
				type = "string modifiers";
			}break;
			case(SURVEY_DATA_REDUCTION_MACRO_T ) : {
				type = "survey data reduction";
			}break;
			case(SIMPLE_MACRO_T                ) : {
				type = "contour";
			}break;
			case(CREATE_ROADS_MACRO_T          ) : {
				type = "create roads";
			}break;
			case(SLF_MACRO_T                   ) : {
				type = "slf";
			}break;
		}
		return 0;
	}
	Text name;
	if(!Get_function_attribute(function,1,name)){
		type = name;
		return 0;
	}

	return 2;
}

Integer get_all_functions_info(Dynamic_Text &function_names, Dynamic_Text &types, Dynamic_Integer &createds, Dynamic_Integer &updateds){
	Null(function_names);
	Get_all_functions(function_names);
	Integer items;
	Get_number_of_items(function_names,items);
	for(Integer i=1;i<=items;i++){
		Text function_name,type;	Integer created, updated;
		Get_item(function_names,i,function_name);
		if(!get_function_info(function_name, type, created, updated)){
			Set_item(function_names,i,function_name);
			Set_item(types,i,type);
			Set_item(createds,i,created);
			Set_item(updateds,i,updated);
			Print(function_name + "<" + type +"> <" + To_text(created) +"> <" + To_text(updated)+">");Print();
		}else{
			Print(function_name + " not found!");Print();
		}
	}
	return 0;
}

Integer get_function_type_info(Text function_type_to_get,Dynamic_Text &function_names, Dynamic_Text &types, Dynamic_Integer &createds, Dynamic_Integer &updateds){
	Null(function_names);
	Get_all_functions(function_names);
	Integer items;
	Get_number_of_items(function_names,items);
	for(Integer i=1;i<=items;i++){
		Text function_name,type;	Integer created, updated;
		Get_item(function_names,i,function_name);
		if(!get_function_info(function_name, type, created, updated)){
			if(function_type_to_get == type){
				Set_item(function_names,i,function_name);
				Set_item(types,i,type);
				Set_item(createds,i,created);
				Set_item(updateds,i,updated);
			}
		}
	}
	return 0;
}

Integer get_function_info_from_volume_report(Function function, Text &model_string_name){
	Text name;
	Get_name(function,name);
	Dynamic_Text list = run_system_command_get_output("dir /b \"*"+name+"*.rpt\"");
	Integer items;
	Get_number_of_items(list,items);
	Print("FUCNTION NAME: "+name+"\n");
	if(!items){
		Print("nothing to report\n");
	}
	Integer correct_volume_report_found = 0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(list,i,t);
		File file;
		if(!File_open(t,"r",file)){
			Integer max_read = 20;
			Integer count = 1;
			while(count<=max_read){
				if(File_read_line(file,t)){
					count = max_read;
				}else{
					t = text_justify(t);
					//Print(t+"\n");
					if(Get_subtext(t,1,8) == "function"){
						if( name == text_justify(Get_subtext(t,9,Text_length(t))) ){
							correct_volume_report_found = 1;
							continue;
						}
					}

					if(Get_subtext(t,1,16) == "reference string"){
						model_string_name = text_justify(Get_subtext(t,17,Text_length(t)));
					}
				}
				count++;
			}
			if(correct_volume_report_found == 1)return 0;
		}
		//Print(t+"\n");
	}
	return 1;
}

//v10 calls

Integer get_apply_many_functions(Dynamic_Text &dt){
#if VERSION_4D >= 1000
	Dynamic_Text functions;
	Get_project_functions(functions);
	Integer items;
	Get_number_of_items(functions,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(functions,i,t);
		Apply_Many_Function function;
		if(!Get_apply_many_function(t,function))//we have an AM function!
			Append(t,dt);
	}
	return 0;
#else
	Dynamic_Text types;
	Dynamic_Integer createds, updateds;
	get_function_type_info("apply many",dt, types, createds, updateds);
	return 0;
#endif
}



#endif
