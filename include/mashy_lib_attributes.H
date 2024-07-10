#ifndef mashy_lib_attributes_h_included
#define mashy_lib_attributes_h_included

#include "mashy_lib_element_functions.H"


Integer check_if_attribute_group_is_dynamic_results(Attributes &source){
// if the attribute is a group, and it also contains an attribute group call 'dynamic', then assume that this is a dynamic results attribute group
	Integer ok=0;
	Integer natts;
	Get_number_of_attributes(source,natts);
	for(Integer i=1;i<=natts;i++){
		Integer type;
		Get_attribute_type(source,i,type);
		if(type == 5){ //attribute group found
			Text name;
			Get_attribute_name(source,i,name);
			if(Text_lower(name) == "dynamic"){ //dyanamic dislexic
				return(1);
			}
		}

	}
	return 0;
}

void print_g(Text t){
	Create_text_log_line(t,1);
}

void print_r(Text t){
	Create_text_log_line(t,3);
}

Log_Line att_delete_dynamic_results_from_atts(Attributes &atts, Integer &errors){
// nuke the drainage atts
	Integer ok=0;
	Integer natts;
	Get_number_of_attributes(atts,natts);
	Log_Line log_tree = Create_group_log_line("Log for (att_delete_dynamic_results_from_atts)",0);
	Integer this_errors = 0;
	Integer deleted_items = 0;
	for(Integer i=natts;i>=1;i--){ // iterate in reverse in case some re-ordering happens
		Integer type;
		Get_attribute_type(atts,i,type);
		if(type == 5){ //attribute group found
			Text name="";
			Get_attribute_name(atts,i,name);
			Integer no=0;
			if(Attribute_exists(atts,name+"/dynamic",no)){
				// now check its a group
				type =0;
				Get_attribute_type(atts,name+"/dynamic",type);
				if(type == 5){
					if(Attribute_delete(atts,i)){
						Append_log_line(Create_text_log_line("Failed to delete attribute group ["+name+"] where it contains an attribute group ["+name+"/dynamic] : group appears to be dynamic results, FAILED AT [Attribute_delete()]",3),log_tree);
						this_errors++;
					}else{
						Append_log_line(Create_text_log_line("Deleting attribute group ["+name+"] as it contains an attribute group ["+name+"/dynamic] : group appears to be dynamic results",1),log_tree);
						deleted_items++;
					}
				}
			}else{
				if( (name == "dyn tc") || (name == "dynamic") ){
					if(Attribute_delete(atts,i)){
						Append_log_line(Create_text_log_line("Failed to delete attribute group ["+name+"] ie. /dyn tc/... : group appears to be dynamic results, FAILED AT [Attribute_delete()]",3),log_tree);
						this_errors++;
					}else{
						Append_log_line(Create_text_log_line("Deleting attribute group ["+name+"] ie. /dyn tc/... : group appears to be dynamic results",1),log_tree);
						deleted_items++;
					}
				}
			}
		}
	}
	if(!this_errors&&!deleted_items){
		Append_log_line(Create_text_log_line("OK",1),log_tree);
	}
	errors+=this_errors;
	return log_tree;
}

Log_Line att_delete_dynamic_results_from_element(Element &e, Integer &errors){
// nuke the drainage atts
	Text t;
	Get_name(e,t);
	Log_Line log_tree = Create_group_log_line("Checking element ["+t+"] for dynamic analysis results",0);
	Attributes atts;
	Get_attributes(e,atts);
	Append_log_line(att_delete_dynamic_results_from_atts(atts,errors),log_tree);
	Set_attributes(e,atts);

	Integer npits;
	Get_drainage_pits(e,npits);
	for(Integer i=1;i<=npits;i++){
		Get_drainage_pit_attributes(e,i,atts);
		Append_log_line(att_delete_dynamic_results_from_atts(atts,errors),log_tree);
		Set_drainage_pit_attributes(e,i,atts);
		if(i<npits){
			Get_drainage_pipe_attributes(e,i,atts);
			Append_log_line(att_delete_dynamic_results_from_atts(atts,errors),log_tree);
			Set_drainage_pipe_attributes(e,i,atts);
		}
	}
	return log_tree;
}

Integer att_delete_dynamic_results_from_strings(Dynamic_Element &delt){
// nuke the drainage atts
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(delt,i,e);
		Integer errors = 0;
		Log_Line log_line = att_delete_dynamic_results_from_element(e,errors);
		Integer is_error = 0;
		if(errors){
			is_error = 1;
		}
		Print_log_line(log_line,is_error);
	}
	return 0;
}


Integer att_delete_dynamic_results_from_model_atts(Model &m){
// nuke the drainage atts
	Attributes atts;
	Get_model_attributes(m,atts);
	Integer errors = 0;
	Log_Line log_tree;
	Text t;
	Get_name(m,t);
	Append_log_line(Create_text_log_line("Checking model ["+t+"] for dynamic analysis results",0),log_tree);
	Append_log_line(att_delete_dynamic_results_from_atts(atts,errors),log_tree);
	Set_model_attributes(m,atts);
	Integer is_error = 0;
	if(errors){
		is_error = 1;
	}
	Print_log_line(log_tree,is_error);
	return 0;
}



Integer merge_to_target(Attributes &source, Attributes &target){
// take the source attributes and merge with the target (ie preserve atts which do not exist)
	Integer ok=0;
	Integer natts;
	Get_number_of_attributes(source,natts);
	for(Integer i=1;i<=natts;i++){
		Integer type;
		Text name;
		Get_attribute_type(source,i,type);
		Get_attribute_name(source,i,name);
		switch ( type ) {
			case (1) : {		// "integer"
				Integer v;
				Get_attribute(source,i,v);
				if(Set_attribute(target,name,v))ok++;
			}break;
			case (2) : {	// "real"
				Real v;
				Get_attribute(source,i,v);
				if(Set_attribute(target,name,v))ok++;
			}break;
			case (3) : {	// "text"
				Text v;
				Get_attribute(source,i,v);
				if(Set_attribute(target,name,v))ok++;
			}break;
			case (5) : {	// "attributes"
				Attributes v;
				Get_attribute(source,i,v);
				if(Set_attribute(target,name,v))ok++;
			}break;
			case (6) : {	// "uid"
				Uid v;
				Get_attribute(source,i,v);
				if(Set_attribute(target,name,v))ok++;
			}break;
			case (7) : {// "64bitinteger"
				Integer64 v;
				Get_attribute(source,i,v);
				if(Set_attribute(target,name,v))ok++;
			}break;
			case (8) : {// "Guid"
				Guid v;
				Get_attribute(source,i,v);
				if(Set_attribute(target,name,v))ok++;
			}break;
			default : {
				Print("Unknown attribute type <"+To_text(type)+">!\n");
				ok++;
			}break;
		}
	}
	return ok;
}

//--------------------------------------------------------------------------------
//                        for tin
//--------------------------------------------------------------------------------
Integer set_as_attribute(Element &parent, Text prefix, Tin &tin){
//setting a reference tin as a function attribute to an element
	Text tin_name;
	Get_name(tin,tin_name);
	Integer ok=0;
	ok+=Set_attribute(parent,prefix+"_tin_name",tin_name);
	return(ok);
}

Integer get_from_attribute(Element &parent, Text prefix, Tin &tin){
// from an element, retrieve a reference tin
	Text tin_name;
	Get_attribute(parent,prefix+"_tin_name",tin_name);
	tin = Get_tin(tin_name);
	if(!Tin_exists(tin)){ //error if tin not retrieved
		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------
//                        for model
//--------------------------------------------------------------------------------
Integer set_as_attribute(Element &parent, Text prefix, Model &m){
//setting a reference model as a function attribute to an element
	Uid model_id;
	Get_id(m,model_id);
	Text model_name;
	Get_name(m,model_name);
	Integer ok=0;
	ok+=Set_attribute(parent,prefix+"_model_name",model_name);
	ok+=Set_attribute(parent,prefix+"_model_id",model_id);
	return(ok);
}

Integer get_from_attribute(Element &parent, Text prefix, Model &m){
// from an element, retrieve a reference model
	Uid model_id;
	Text model_name;
	Get_attribute(parent,prefix+"_model_name",model_name);
	m = Get_model(model_name);
	Get_id(m,model_id);
	if(Is_null(model_id)){ //if the text name did not return a valid id, then get the attribute ID to try and retrieve the model
		Get_attribute(parent,prefix+"_model_id",model_id);
		if(Get_model(model_id,m)){
			return 1;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------------
//                        for element
//--------------------------------------------------------------------------------
Integer set_as_attribute(Element &parent, Text prefix, Element &e){
// setting a reference string as a function attribute
// that is, details for parent will be set to e
// prefix is important
	Uid model_id, element_id;
	get_id(e,model_id,element_id);

	Text model_name = get_element_model(e);
	Text element_name = get_element_name(e);
	Integer ok=0;
	ok+=Set_attribute(parent,prefix+"_element_name",element_name);
	ok+=Set_attribute(parent,prefix+"_element_id",element_id);
	ok+=Set_attribute(parent,prefix+"_model_name",model_name);
	ok+=Set_attribute(parent,prefix+"_model_id",model_id);
	return(ok);
}

Integer get_from_attribute(Element &parent, Text prefix, Element &e){
// getting a reference string from a function attribute
// for a parent element, retieve a child element
// prefix is important
	Uid element_id,model_id;
	Text element_name,model_name;
	Get_attribute(parent,prefix+"_element_name",element_name);
	Get_attribute(parent,prefix+"_element_id",element_id);
	Get_attribute(parent,prefix+"_model_name",model_name);
	Get_attribute(parent,prefix+"_model_id",model_id);

	Element ex;
	if(get_element(model_name, model_id, element_name, element_id, ex))	return (1);
	e = ex;
	Null(ex);
	return(0);
}

Integer filter_attributes(Attributes &attr, Dynamic_Text &dt_integer_name, Dynamic_Integer &dt_integer_value, Dynamic_Text &dt_real_name, Dynamic_Real &dt_real_value, Dynamic_Text &dt_text_name, Dynamic_Text &dt_text_value){
// take attributes and spit out attributes into arrays of the corresponding type
// NOTE: nothing is done for UID attributes!!! (what are the consequences?)
	Integer no_atts;
	Get_number_of_attributes(attr,no_atts);
	for(Integer i = 1 ; i <= no_atts ; i++){
		Integer type;
		Get_attribute_type(attr,i,type);
		Text att_name;	Get_attribute_name(attr,i,att_name);
		switch(type) {
			case (1) : {	// Integer
				Append(att_name,dt_integer_name);
				Integer value;	Get_attribute(attr,i,value);
				Append(value,dt_integer_value);
			}break;
			case (2) : {	// Real
				Append(att_name,dt_real_name);
				Real value;	Get_attribute(attr,i,value);
				Append(value,dt_real_value);
			}break;

			case(3) : {	// Text
				Append(att_name,dt_text_name);
				Text value;	Get_attribute(attr,i,value);
				Append(value,dt_text_value);
			}break;
		}
	}
	return 0;
}


Integer set_as_attribute(Element &element, Attributes &att){
// for an element, apply its name and model to &att
	Text model_name;
	Uid model_id;
	Text element_name;
	Uid element_id;
	if(get_element_names_ids(element, model_name, model_id, element_name, element_id)){
		return 1;
	}
	Integer ok=0;
	ok+=Set_attribute(att,"model_name",model_name);
	ok+=Set_attribute(att,"model_uid",model_id);
	ok+=Set_attribute(att,"string_name",element_name);
	ok+=Set_attribute(att,"string_uid",element_id);
	return ok;
}


Integer get_from_attribute(Attributes &att, Element &element){
// from a know att group, retrieve the &element
// if not found by id's it goes and finds by names
	Null(element);
	Text model_name;
	Uid model_id;
	Text element_name;
	Uid element_id;

	Integer ok=0;
	ok+=Get_attribute(att,"model_name",model_name);
	ok+=Get_attribute(att,"model_uid",model_id);
	ok+=Get_attribute(att,"string_name",element_name);
	ok+=Get_attribute(att,"string_uid",element_id);

	Element e;

	if(!get_element(model_name, model_id, element_name, element_id, e)){ // return 0 is good, means 1 element was found, 1 means no element found, and >2 means the first search failed and it falls back to a name->name search and found 2 or more strings
		element =e;
		return 0;
	}
	return 1;
}



//--------------------------------------------------------------------------------
//                        GET INTEGER ATTS
//--------------------------------------------------------------------------------
Integer get_attributes(Attributes &attr, Integer max_atts, Text &name[], Integer &value[], Integer &count){
// get all the integer attributes
	count = 0; //reset the count
	for(Integer i = 1 ; i <= max_atts ; i++){
		Integer type;
		Get_attribute_type(attr,i,type);
		if( type == 1 ) {	// type = Integer
			count++;
			Get_attribute_name(attr,i,name[count]);
			Get_attribute(attr,i,value[count]);
		}
	}
	Integer null;
	for(i = count+1;i<=max_atts;i++){ //null
		name[i] = "";
		value[i]=null;
	}
	return 0;
}

//--------------------------------------------------------------------------------
//                        GET REAL ATTS
//--------------------------------------------------------------------------------
Integer get_attributes(Attributes &attr, Integer max_atts, Text &name[], Real &value[], Integer &count){
// get all the real attributes
	count = 0; //reset the count
	for(Integer i = 1 ; i <= max_atts ; i++){
		Integer type;
		Get_attribute_type(attr,i,type);
		if( type == 2 ) {	// type = Real
			count++;
			Get_attribute_name(attr,i,name[count]);
			Get_attribute(attr,i,value[count]);
		}
	}
	Integer null;
	for(i = count+1;i<=max_atts;i++){ //null
		name[i] = "";
		value[i]=null;
	}
	return 0;
}

//--------------------------------------------------------------------------------
//                        GET TEXT ATTS
//--------------------------------------------------------------------------------
Integer get_attributes(Attributes &attr, Integer max_atts, Text &name[], Text &value[], Integer &count){
// get all the text attributes
	count = 0; //reset the count
	for(Integer i = 1 ; i <= max_atts ; i++){
		Integer type;
		Get_attribute_type(attr,i,type);
		if( type == 3 ) {	// type = Text
			count++;
			Get_attribute_name(attr,i,name[count]);
			Get_attribute(attr,i,value[count]);
		}
	}
	for(i = count+1;i<=max_atts;i++){ //null
		name[i] = "";
		value[i]= "";
	}
	return 0;
}

Integer get_attribute_as_text(Attributes &attr, Text name, Integer decimals, Text &value){
	value = "";
	Integer type;
	Get_attribute_type(attr,name,type);
	if(type == 3){//text
		return Get_attribute(attr,name,value);
	}else if(type == 2){ //real
		Real r;
		Get_attribute(attr,name,r);
		value = To_text(r,decimals);
		return 0;
	}
	return 0;
}

Text get_attribute_as_text(Attributes &attr, Text name){
	Text value;
	Integer decimals = 7;
	get_attribute_as_text(attr, name, decimals, value);
	return value;
}



//--------------------------------------------------------------------------------
//                        GET UID ATTS
//--------------------------------------------------------------------------------
Integer get_attributes(Attributes &attr, Integer max_atts, Text &name[], Uid &value[], Integer &count){
// get all the Uid attributes
	count = 0; //reset the count
	for(Integer i = 1 ; i <= max_atts ; i++){
		Integer type;
		Get_attribute_type(attr,i,type);
		if( type == 4 ) {	// type = UID
			count++;
			Get_attribute_name(attr,i,name[count]);
			Get_attribute(attr,i,value[count]);
		}
	}
	Uid null;
	Null(null);
	for(i = count+1;i<=max_atts;i++){ //null
		name[i] = "";
		value[i]= null;
	}
	return 0;
}

//--------------------------------------------------------------------------------
//                        GET ATTRIBUTES ATTS
//--------------------------------------------------------------------------------
Integer get_attributes(Attributes &attr, Integer max_atts, Text &name[], Attributes &value[], Integer &count){
// get all the attribute attributes
	count = 0; //reset the count
	for(Integer i = 1 ; i <= max_atts ; i++){
		Integer type;
		Get_attribute_type(attr,i,type);
		if( type == 5 ) {	// type = ATTRIBUTES
			count++;
			Get_attribute_name(attr,i,name[count]);
			Get_attribute(attr,i,value[count]);
		}
	}
	Attributes null;
	for(i = count+1;i<=max_atts;i++){ //null
		name[i] = "";
		value[i]= null;
	}
	return 0;
}

//--------------------------------------------------------------------------------
//                        SET ATTRIBUTES TO A DYNAMIC_ELEMENT
//                        (SET ATTS TO MANY ELEMENTS IN 1 HIT)
//--------------------------------------------------------------------------------

Integer set_attribute(Dynamic_Element &de, Text att_name, Text att){
	Integer ok=0;
	Integer items;
	Get_number_of_items(de,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(de,i,e);
		ok+=Set_attribute(e,att_name,att);
	}
	return(ok);
}

Integer set_attribute(Dynamic_Element &de, Text att_name, Integer att){
	Integer ok=0;
	Integer items;
	Get_number_of_items(de,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(de,i,e);
		ok+=Set_attribute(e,att_name,att);
	}
	return(ok);
}

Integer set_attribute(Dynamic_Element &de, Text att_name, Real att){
	Integer ok=0;
	Integer items;
	Get_number_of_items(de,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(de,i,e);
		ok+=Set_attribute(e,att_name,att);
	}
	return(ok);
}

Integer set_attribute(Dynamic_Element &de, Text att_name, Uid att){
	Integer ok=0;
	Integer items;
	Get_number_of_items(de,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(de,i,e);
		ok+=Set_attribute(e,att_name,att);
	}
	return(ok);
}

Integer set_attribute(Dynamic_Element &de, Text att_name, Attributes att){
	Integer ok=0;
	Integer items;
	Get_number_of_items(de,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(de,i,e);
		ok+=Set_attribute(e,att_name,att);
	}
	return(ok);
}


//--------------------------------------------------------------------------------
//                        SPECIFIC ROUTINES
//--------------------------------------------------------------------------------

Integer vertex_text_to_attribute(Dynamic_Element &elts, Text attribute_name){
//ok is the number of elements skipped (not of type Super)
	Integer ok =0;
	Integer items;
	Get_number_of_items(elts,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(elts,i,e);
		Text type;
		Get_type(e,type);
		if(type =="Super"){
			Set_super_use_vertex_attribute(e,1);
			Integer points;
			Get_points(e,points);
			for(Integer j=1;j<=points;j++){
				Text value;
				if(!Get_super_vertex_text(e,j,value)){
					if(value!="")
						Set_super_vertex_attribute(e,j,attribute_name,value);
				}
			}
		}else{
			ok++;
		}
	}
	return(ok);
}


//--------------------------------------------------------------------------------
//                        SPECIFIC ROUTINES
//--------------------------------------------------------------------------------

// text to attribute routines - these are for reading in text files and automatically going to attributes

Integer attribute_type_to_integer(Text t){

	switch ( Text_lower(t) ) {
		case ("integer") : {
			return 1;
		}break;
		case ("real") : {
			return 2;
		}break;
		case ("text") : {
			return 3;
		}break;
		case ("binary") : {		// ?
			return 4;
		}break;
		case ("attributes") : {		// ?
			return 5;
		}break;
		case ("uid") : { 		// ?
			return 6;
		}break;
		case ("64bitinteger") : {	// ?
			return 7;
		}break;
		default : {
			return 0;
		}break;
	}
	return 0;
}

Integer to_attribute(Text t, Integer &type, Text &name, Text &value){
	Dynamic_Text parts;
	From_text(t,parts);
	Integer n;
	Get_number_of_items(parts,n);
	if(n>=3){
		Text t_type;	Get_item(parts,1,t_type);
		type = attribute_type_to_integer(t_type);
		Get_item(parts,2,name);
		Get_item(parts,3,value);
		if(!type)return 1;
	}else{
		return 2; // invalid number of arguments
	}
	return 0;
}

Integer append(Text t, Attributes &att){
	Integer type;
	Text name,value;
	if(!to_attribute(t, type, name, value)){

		switch ( type ) {
			case (1) : {		// "integer"
				Integer v;
				From_text(value,v);
				Set_attribute(att,name,v);
			}break;
			case (2) : {	// "real"
				Real v;
				From_text(value,v);
				Set_attribute(att,name,v);
			}break;
			case (3) : {	// "text"
				Set_attribute(att,name,value);
			}break;
			case (4) : {	// "binary"
				// not allowed by me as I dont know how to do it??
			}break;
			case (5) : {	// "attributes"
				// not allowed by me as I dont know how to do it??
				// need to recursively do this ... need to think more
			}break;
			case (6) : {	// "uid"
				Uid v;
				From_text(value,v);
				Set_attribute(att,name,v);
			}break;
			case (7) : {// "64bitinteger"
				// not allowed by me as I dont know how to do it??
			}break;
			default : {
				return 1;
			}break;
		}
	}
	return 0;
}

#ifndef ATT_PREC
#define ATT_PREC 10
#endif
// this will take att and put in the text format
// <tab> type <tab> name <tab> value
Integer attribute_to_text(Attributes att, Dynamic_Text &dt){
	Integer n=0;
	Get_number_of_attributes(att,n);
	for(Integer i=1;i<=n;i++){
		Integer type;
		Text name="";
		Get_attribute_type(att,i,type);
		Get_attribute_name(att,i,name);
		Text value = "";
		Text t_type = "";
		switch ( type ) {
			case (1) : {		// "integer"
				Integer v;
				Get_attribute(att,i,v);
				To_text(v,value);
				t_type = "integer";
			}break;
			case (2) : {	// "real"
				Real v;
				Get_attribute(att,i,v);
				value = To_text(v,ATT_PREC);
				t_type = "real";
			}break;
			case (3) : {	// "text"
				Get_attribute(att,i,value);
				t_type = "text";
			}break;
			case (4) : {	// "binary"
				// not allowed by me as I dont know how to do it??
			}break;
			case (5) : {	// "attributes"
				// not allowed by me as I dont know how to do it??
				// need to recursively do this ... need to think more
			}break;
			case (6) : {	// "uid"
				Uid v;
				Get_attribute(att,i,v);
				Convert_uid(v,value);
				t_type = "uid";
			}break;
			case (7) : {// "64bitinteger"
				// not allowed by me as I dont know how to do it??
			}break;
			default : {
				return 1;
			}break;
		}
		Append("\t" + t_type + "\t" + name + "\t" + value,dt);
	}
	return 0;
}

#endif