#ifndef mashy_lib_model_functions_included
#define mashy_lib_model_functions_included

#include "mashy_lib_sort.H"

#include "mashy_lib_text_functions.H"
#include "mashy_lib_element_functions.H"
#include "mashy_lib_log_lines.H"
#include "mashy_lib_widgets_list_box.H"


Integer models_draw(Dynamic_Text &model_list, Integer col_num){
	Integer items;	Get_number_of_items(model_list,items);
	Integer ierr = 0;
	for(Integer i=1;i<=items;i++){
		Text t;	Get_item(model_list,i,t);
		ierr+=Model_draw(Get_model(t),col_num);
	}
	return(ierr);
}

Integer models_draw(Dynamic_Text &model_list){
	Integer items;	Get_number_of_items(model_list,items);
	Integer ierr = 0;
	for(Integer i=1;i<=items;i++){
		Text t;	Get_item(model_list,i,t);
		ierr+=Model_draw(Get_model(t));
	}
	return(ierr);
}

#if VERSION_4d > 1400
Dynamic_Text get_unique_model_names(Dynamic_Element &delt){
	Dynamic_Text model_list;
	// get a list of the models within the list of elements
	Integer items;	Get_number_of_items(delt,items);
	Model m;
	Uid_Text_Map uid_text_map;
	for(Integer i=1;i<=items;i++){
		Element e;	Get_item(delt,i,e);
		if(!Get_model(e,m)){
			Uid uid;
			Get_id(m,uid);
			Text t;
			Get_name(m,t);
			Container_insert_key(uid_text_map,uid,t);
		}
	}
	Integer range = -1;
	Integer res = Container_range_begin(uid_text_map,range);
	while(res == 0){
		Text value;
		res = Container_get_value(uid_text_map,range,value);
		if(res !=0 )break;
		//do something here
		Append(value,model_list);
		res = Container_range_next(uid_text_map,range);
	}
	res = Container_range_remove(uid_text_map,range);
	return (model_list);
}


// this will go through and make a list of models
Integer models_draw(Dynamic_Element &delt){
	Dynamic_Text model_list = get_unique_model_names(delt);
	return(models_draw(model_list));
}

Integer models_draw(Dynamic_Element &delt, Integer col_num){
	Dynamic_Text model_list = get_unique_model_names(delt);
	return(models_draw(model_list,col_num));
}
#endif

Integer get_model_create_unique(Model &m){
	Integer time;
	Get_time(time);
	Integer method = 1;
	Set_random_number(time,method);

	Integer max_count = 999;
	Integer count = 0;
	Null(m);
	while(1){
		//Integer this_random = Get_random_number();

		Text model_name = "u"+To_text( Absolute(Get_random_number()) );
		if(!Model_exists(model_name)){
#if DEBUG
			Print("create_unique_model() <"+model_name+">\n");
#endif
			m = Get_model_create(model_name);
			return 0;
		}
		count++;
		if(count>max_count){
			return 1;
		}
	}
	return 1;
}

// this will return a list of models which are loaded in the project
Dynamic_Text get_loaded_models(){
	Uid function_id, model_id_start, model_id_final, element_id_start, element_id_final;
	Null(function_id);
	Convert_uid(1,model_id_start);
	Convert_uid(1,element_id_start);
	model_id_final = Get_last_uid();
	element_id_final = Get_last_uid();
	Integer skip_tins=1, loaded_models=1;
	Dynamic_Text model_list;
	Integer ok = Get_models( function_id, model_id_start, model_id_final, element_id_start, element_id_final, skip_tins, loaded_models, model_list);
	return model_list;
}


Model get_model_create_unique(){
	Model m;
	if(get_model_create_unique(m)){
		Print("Failed to create random model, exhausted retries!\n");
	}
	return m;
}
// this will delete all elements by setting them to a temporary model then using the inbuild Model_delete()
Integer delete(Dynamic_Element &delt){
	Model m = get_model_create_unique();
	Set_model(delt,m);
	return Model_delete(m);
}

Integer set_new_random_model(Dynamic_Element &delt){
	Model m = get_model_create_unique();
	return Set_model(delt,m);
}

// get the name of a model and return it as text
Text get_name (Model &m){Text t;Get_name(m,t);return(t);}


Integer is_valid(Model &model, Integer do_output_messages){
	Uid uid_model;
	Get_id(model,uid_model);
	if(Is_null(uid_model)){
		if(do_output_messages)printe("ERROR: model uid does not exist! - Model name invalid\n");
		return 0;	//if model handle is dud fails
	}
	if(Is_global(uid_model)){
		Text t;
		Get_name(model,t);
		if(do_output_messages)printe("ERROR: model <"+t+"> is a shared model!\n");
		return 0; //if model handle is shared fails
	}
	return 1;
}

// some function overlodes for is_valid()
Integer is_valid(Model &model){
	return (is_valid(model,0));
}

Integer has_write(Model &m){
	return (is_valid(m,1));
}

//this gets a sorted list of the models in the project.... this call for use when a sorted model list is required
Integer get_project_models(Dynamic_Text &model_names){
	Dynamic_Text list;
	Integer ierr = Get_project_models(list);
	Integer items;
	Get_number_of_items(list,items);
	if(items < 1)return 1;
	Integer index[items];
	Qsort(list,index,items);
	for(Integer i=1;i<=items;i++) {
		Text t;
		Get_item(list,index[i],t);
		Append(t,model_names);
	}
	Null(list);
	return ierr;
}

Integer get_model_list_in_order(Dynamic_Text &list){
	Integer ok=0;
	Null(list);
	ok+=Get_project_models(list);
	sort_numeric(list);
	return ok;
}

Integer get_models(Text pattern, Dynamic_Text &model_names){
	Dynamic_Text models;
	Get_project_models(models);
	if(!match_name(models,pattern,model_names))return 1;	//match name returns the number of models matched
	return 0;
}

/* SEE "X:\12d\MACROS\_INCLUDES\mashy_lib_widgets_list_box.H"
Integer create_model_share(Text model_name, Text dot_project_path_no_end_slash){
	Integer ok=0;
	Text t = find_replace(model_name," ","_") + ".model";
	if(File_exists(dot_project_path_no_end_slash+"\\"+t)){
		ok+=Directory_create(dot_project_path_no_end_slash+"\\"+t+".ref");
	}else{
		Print("<"+model_name+"> Share Failed!");Print();
		ok++;
	}
	return ok;
}
*/

Integer is_shared(Model &model){
//the uid of a shared model begins with {... use this as a test to see if a model has been shared in
	Uid uid;
	Get_id(model,uid);
#if 1
	return(Is_global(uid));
#else
	Integer c;
	Get_char(To_text(uid),1,c);
	if(c=='{')	return (1);
	return (0);
#endif
}

Text get_model_name(Model &model){
	Text model_name;
	Get_name(model,model_name);
	return(model_name);
}

Integer clean_model(Model &mdl){
	Uid uid;
	Get_id(mdl,uid);
	if(Is_global(uid)){
		Text name;
		Get_name(mdl,name);
		Print("Failed to clean model ["+name+"], as it is shared into this project!\n");
		return 1;	//model shared in
	}
	return Model_clean(mdl,3); // 3 means clean rasters
	// Integer items;
	// Dynamic_Element delt;
	// Get_elements(mdl, delt, items);
	// if(items){ //if there any elements

	// 	Model temp_mdl;
	// 	if(get_model_create_unique(temp_mdl)){
	// 		temp_mdl = Get_model_create("12d delete model for clean d21");
	//     }
	// 	Set_model(delt,temp_mdl);
	// 	Null(delt);
	// 	return Model_delete(temp_mdl);
	// }
	return 0;
}
Integer clean_model(Dynamic_Text &list){
	Integer items;
	Get_number_of_items(list,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(list,i,t);
		Model m = Get_model(t);
		Uid uid;
		Get_id(m,uid);
		if(Is_null(uid))continue;
		clean_model(m);
	}
	return 0;
}

Integer clean_model_check_locks(Model mdl){
	return Model_clean(mdl,3);//
	// keep code below... the above call should handle locks etc...
	Integer count = 0;
	Dynamic_Element delt;
	Append(mdl,delt);
	Integer no_items;
	Get_number_of_items(delt,no_items);
	for(Integer i=no_items;i>=1;i--){ //you dickhead...there's one less item each time you delete, so can only ever get 1/2 way through with i++
		Element elt;
		Get_item(delt,i,elt);
		if(check_for_locks(elt) == 0){
			Element_delete(elt);
		}else{
			count++;
		}
	}
	Null(delt);
	return count;
}


Integer clean_model(Text model_name){
	Model mdl = Get_model(model_name);
	return Model_clean(mdl,3);//
	// clean_model(mdl);
	// return 0;
}

/*
Integer get_id(Model m){
//return the id as an integer
	Uid uid;
	Integer i;
	Get_id(m,uid);
	Convert_uid(uid,i);
	return i;
}
*/

Integer is_null(Model model){
// check if a model is null
// return 0 indicates the model is NOT null
	Uid uid_model;
	Get_id(model,uid_model);
	if(Is_null(uid_model) != 0 ){
		return-1;
	}
	return 0;
}

Integer delete_model_if_empty(Model model){
	Integer items;
	Get_number_of_items(model,items);
	if(!Model_exists(model)) return(-1); // error if model does not exist
	if(items == 0) {
		Model_delete(model);
	}
	return 0;
}

Integer delete_model_if_empty(Text model_name){
	return delete_model_if_empty(Get_model(model_name));
}

Integer delete_model_check_locks(Model model){
	Integer ierr = clean_model_check_locks(model);
	if( ierr != 0 )return ierr;
	return delete_model_if_empty(model);
}

Integer delete_model_check_locks(Text model_name){
	return delete_model_check_locks(Get_model(model_name));
}

Integer model_delete(Dynamic_Text &list){
	Integer ok=0;
	Integer items;
	Get_number_of_items(list,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(list,i,t);
		Model m = Get_model(t);
		Uid uid; Get_id(m,uid);
		if(Is_global(uid))continue;
		if(Is_null(uid))continue;
		ok+=Model_delete(m);
	}
	return ok;
}

Dynamic_Text get_models_with_element_type(Dynamic_Text &models, Text type){
	Dynamic_Text dt_match;
	Integer items;
	Get_number_of_items(models,items);
	for (Integer i = 1 ; i <= items ; i++ ){
		Text model;
		Get_item(models,i,model);
		Model mdl = Get_model(model);
		Dynamic_Element elts;
		Integer ielts;
		Get_elements(mdl,elts,ielts);
		Integer matches_type = 0;
		for(Integer j = 1 ; j <= ielts ; j++ ){
			Element e;
			Get_item(elts,j,e);
			Text te;
			Get_type(e,te);
			if(type == "-tins"){
				//Print("checking for tins\n");
				if(te == "Tin" || te == "SuperTin"){
					matches_type = 1;
					#if DEBUG
						Print("INFO: found a tin type: " + te + ", in model: " + model + "\n");
					#endif
					break;

				}
			} else {
				if(type == te){
					matches_type = 1;
					#if DEBUG
						Print("INFO: found element type: " + te + ", in model: " + model + "\n");
					#endif
					break;
				}
			}

		}
		if(matches_type != 0){
			Append(model,dt_match);
			#if DEBUG
				Print("Appending: " + model + "\n");
			#endif
		}
		Null(elts);
	}
	return(dt_match);
}

#endif