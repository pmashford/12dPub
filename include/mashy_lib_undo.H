#ifndef mashy_lib_undo_h_included
#define mashy_lib_undo_h_included
/*
	add_undo_range() can be undone but wont allow a redo
	add_undo_change() should be used for changes - the 1st element should be a Element_duplaicte() of the edited element before it is edited
	use add_undo_add(Text,Dynamic_Element) instead

*/

Integer add_undo_range(Text undo_name, Uid &start_id, Uid &end_id){
//add an undo if entities have been created, expand the base 12d call to only add the undo IF new elements were created
	if(Is_valid(start_id) && Is_valid(end_id)){
		Integer start,end;
		Convert_uid(start_id,start);
		Convert_uid(end_id,end);
		if(start <= end){
			Undo undo = Add_undo_range(undo_name,start_id,end_id);
			return(0);
		}
	}
	return 1;//error
}

Integer add_undo_change(Text undo_name,Dynamic_Element &delt_original, Dynamic_Element &delt){
	Integer items;
	Get_number_of_items(delt_original,items);
	if(items < 1)return 1;
	Undo_List undo_list;
	for(Integer i=1;i<=items;i++){
		Element e[2];
		Get_item(delt_original,i,e[1]);
		Get_item(delt,i,e[2]);
		Append(Add_undo_change("changed",e[1],e[2]),undo_list);   //this element is deleted on an undo 
	}
	Add_undo_list(undo_name,undo_list);
	return 0;
}

Integer add_undo_change(Text undo_name,Element &original[], Element &elt[], Integer nelts){
	Undo_List undo_list;
	for(Integer i=1;i<=nelts;i++){
		Append(Add_undo_change("changed",original[i],elt[i]),undo_list);   //this element is deleted on an undo 
	}
	Add_undo_list(undo_name,undo_list);
	return 0;
}


#if 0
//USAGE


	//  the element which is restored for an undo must exist, but not in a model...
	for(Integer i=1;i<=nelts;i++){ 
		Element e; 
		Get_item(delt,i,e); 
		Element e_undo;
		Element_duplicate(e,e_undo);	//Take a copy of the element
		Set_colour(e,new_colour); 	//Make changes to the element
		Append(Add_undo_change("",e_undo,e),undo_list);	// Append to undo commnad
		//Element_delete(e_undo); //WARNING: Do not delete e_undo or 12d will crash for an undo - e_undo is the element which is restored
	} 


#endif

#endif