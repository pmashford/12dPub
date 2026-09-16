#ifndef mashy_lib_get_item_h_included
#define mashy_lib_get_item_h_included


Real get_item(Dynamic_Real &list, Integer i){
	//return the specified real from a Dynamic_Real
	Real r;
	Get_item(list,i,r);
	return r;
}

Text get_item(Dynamic_Text &list, Integer i){
	//return the specified real from a Dynamic_Text
	Text t;
	Get_item(list,i,t);
	return t;
}

Integer get_item(Dynamic_Integer &list, Integer i){
	//return the specified integer from a Dynamic_Integer
	Integer j;
	Get_item(list,i,j);
	return j;
}


Element get_item(Dynamic_Element &list, Integer i){
	//return the specified integer from a Dynamic_Element
	Element e;
	Get_item(list,i,e);
	return e;
}

Integer get_count(Dynamic_Real &list){
	Integer i;
	Get_number_of_items(list,i);
	return(i);
}

Integer get_count(Dynamic_Text &list){
	Integer i;
	Get_number_of_items(list,i);
	return(i);
}

Integer get_count(Dynamic_Integer &list){
	Integer i;
	Get_number_of_items(list,i);
	return(i);
}

Integer get_count(Dynamic_Element &list){
	Integer i;
	Get_number_of_items(list,i);
	return(i);
}

Integer is_unique(Integer value, Dynamic_Integer &list){
	Integer items = get_count(list);
	for(Integer i=1;i<=items;i++){
		Integer this_value;
		Get_item(list,i,this_value);
		if(this_value == value)return 0;
	}
	return 1;
}

Integer is_unique(Real value, Dynamic_Real &list){
	Integer items = get_count(list);
	for(Integer i=1;i<=items;i++){
		Real this_value;
		Get_item(list,i,this_value);
		if(this_value == value)return 0;
	}
	return 1;
}

Integer is_unique(Text value, Dynamic_Text &list){
	Integer items = get_count(list);
	for(Integer i=1;i<=items;i++){
		Text this_value;
		Get_item(list,i,this_value);
		if(this_value == value)return 0;
	}
	return 1;
}

Integer is_unique(Element value, Dynamic_Element &list){
	Uid uid;
	Get_id(value,uid);
	Integer items = get_count(list);
	for(Integer i=1;i<=items;i++){
		Element this_value;
		Get_item(list,i,this_value);
		Uid this_uid;
		Get_id(this_value,this_uid);
		if(this_uid == uid)return 0;
	}
	return 1;
}

#endif