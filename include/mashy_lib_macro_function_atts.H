#ifndef mashy_lib_macro_function_atts_H_included
#define mashy_lib_macro_function_atts_H_included

#include "mashy_lib_element_functions.H"
#include "mashy_lib_model_functions.H"

Integer set_as_attribute(Macro_Function &macro_function, Text prefix, Element &e){
//setting a reference string as a function attribute
	Uid model_id, element_id;
	get_id(e,model_id,element_id);

	Text model_name = get_element_model(e);
	Text element_name = get_element_name(e);
	Integer ok=0;
	ok+=Set_function_attribute(macro_function,prefix+"_element_name",element_name);
	ok+=Set_function_attribute(macro_function,prefix+"_element_id",element_id);
	ok+=Set_function_attribute(macro_function,prefix+"_model_name",model_name);
	ok+=Set_function_attribute(macro_function,prefix+"_model_id",model_id);
	return(ok);
}

Integer get_from_attribute(Macro_Function &macro_function, Text prefix, Element &e){
//getting a reference string from a function attribute
	Uid element_id,model_id;
	Text element_name,model_name;
	Get_function_attribute(macro_function,prefix+"_element_name",element_name);
	Get_function_attribute(macro_function,prefix+"_element_id",element_id);
	Get_function_attribute(macro_function,prefix+"_model_name",model_name);
	Get_function_attribute(macro_function,prefix+"_model_id",model_id);

	Element ex;
	if(get_element(model_name, model_id, element_name, element_id, ex))	return (1);
	e = ex;
	Null(ex);
	return(0);
}

Integer set_as_attribute(Macro_Function &macro_function, Text prefix, Model &model){
//setting a model as a function attribute
	Integer ok=0;
	Uid model_id;
	Get_id(model,model_id);
	Text model_name; Get_name(model,model_name);
	ok+=Set_function_attribute(macro_function,prefix+"_model_id",model_id);
	ok+=Set_function_attribute(macro_function,prefix+"_model_name",model_name);
	return(ok);
}

Integer get_from_attribute(Macro_Function &macro_function, Text prefix, Model &model){
//getting a model from a function attribute
	Uid model_id;
	Text model_name;

	Get_function_attribute(macro_function,prefix+"_model_id",model_id);
	Get_function_attribute(macro_function,prefix+"_model_name",model_name);

	if(!Get_model(model_id,model)) return 0;

	model = Get_model(model_name);
	if(!is_null(model))
		return (0);

	return(1); //error no model found!
}

Integer set_as_attribute(Macro_Function &macro_function, Text prefix, Tin &tin){
//setting a model as a function attribute
	Integer ok=0;
	Text tin_name; Get_name(tin,tin_name);
	ok+=Set_function_attribute(macro_function,prefix+"_tin_name",tin_name);
	return(ok);
}


Integer get_from_attribute(Macro_Function &macro_function, Text prefix, Tin &tin){
//getting a model from a function attribute
	Text tin_name;
	Get_function_attribute(macro_function,prefix+"_tin_name",tin_name);
	tin = Get_tin(tin_name);
	if(Tin_exists(tin))
		return (0);

	return(1); //error no tin found!
}

//STRINGS TO DELETE
Integer elements_delete(Macro_Function &macro_function){
	Uid model_id,start_id,end_id;
	Integer ok=0;
	ok+=Get_function_attribute(macro_function,"function_elements_model_id",model_id);
	ok+=Get_function_attribute(macro_function,"function_elements_start_id",start_id);
	ok+=Get_function_attribute(macro_function,"function_elements_end_id",end_id);
	//Print(model_id);
	//Print(start_id);
	//Print(end_id);
	//p("delete ok",ok);
	if(!ok){
		Integer id,last_id;
		Convert_uid(start_id,id);
		Convert_uid(end_id,last_id);
		for(Integer i=id;i<=last_id;i++){
			Uid this_id;
			Convert_uid(i,this_id);
			//p("delete i",i);
			Element e;
			if(!Get_element(model_id,this_id,e)){
				//if(!check_for_locks(e))
					Element_delete(e);
					//Print("deleted\n");
			}
		}
	}else{
		return 1;
	}
	return(0);
}

Integer elements_remember_start(Macro_Function &macro_function,Model &model){
	Uid start_id = Get_next_uid();
	Set_function_attribute(macro_function,"function_elements_start_id",start_id);
	//p("remember start function");
	return(0);
}

Integer elements_remember_end(Macro_Function &macro_function,Model &model){
	//p("remember end function");
	Uid start_id;
	if(Get_function_attribute(macro_function,"function_elements_start_id",start_id))return 1;

	Uid end_id = Get_next_uid();
	if(start_id == end_id){ //this means no elements where created!
		Function_attribute_delete(macro_function,"function_elements_start_id");
		//p("id's are eq!");
	}else{
		//p("id's are NOT eq");
		end_id = Get_last_uid();
		Set_function_attribute(macro_function,"function_elements_end_id",end_id);
		Uid model_id;
		Get_id(model,model_id);
		Set_function_attribute(macro_function,"function_elements_model_id",model_id);
	}
	return(0);
}

Integer delete_atts_keep_recalc_atts(Macro_Function &macro_function){
//this deletes the function attributes but keeps the atts which point to the old strings if the attributes exist
	Uid start_id, end_id, model_id;
	Integer a=1,b=1,c=1;
	if(Get_function_attribute(macro_function,"function_elements_start_id",start_id))a=0;
	if(Get_function_attribute(macro_function,"function_elements_end_id",end_id))b=0;
	if(Get_function_attribute(macro_function,"function_elements_model_id",model_id))c=0;
	Integer ok=Function_attribute_delete_all(macro_function);
	if(a)Set_function_attribute(macro_function,"function_elements_start_id",start_id);
	if(b)Set_function_attribute(macro_function,"function_elements_end_id",end_id);
	if(c)Set_function_attribute(macro_function,"function_elements_model_id",model_id);
	if( (ok == 0) && (a+b+c) == 3)return 0;	//all good
	return 1;

}

Integer set_id(Macro_Function &macro_function){
	Uid model_id,start_id,end_id;
	Integer ok=0;
	ok+=Get_function_attribute(macro_function,"function_elements_model_id",model_id);
	ok+=Get_function_attribute(macro_function,"function_elements_start_id",start_id);
	ok+=Get_function_attribute(macro_function,"function_elements_end_id",end_id);
	Uid fid;
	Get_id(macro_function,fid);
	if(!ok){
		Integer id,last_id;
		Convert_uid(start_id,id);
		Convert_uid(end_id,last_id);
		for(Integer i=id;i<=last_id;i++){
			Uid this_id;
			Convert_uid(i,this_id);
			Element e;
			if(!Get_element(model_id,this_id,e)){
				Set_function_id(e,fid);
			}
		}
	}else{
		return 1;
	}
	return(0);
}

Integer set_id(Macro_Function &macro_function, Dynamic_Element &delt){
	Uid fid;
	Get_id(macro_function,fid);
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(delt,i,e);
		Set_function_id(e,fid);
	}
	return 0;

}



#endif