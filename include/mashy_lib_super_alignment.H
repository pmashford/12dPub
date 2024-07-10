#ifndef mashy_lib_super_alignment_h_included
#define mashy_lib_super_alignment_h_included

#include "mashy_lib_create_super_alignment.H"
#include "12d/standard_library.H"
#include "mashy_lib_super_alignment_info.H"

//--------------------------------------------------------------------------------
//                        SA HG
//--------------------------------------------------------------------------------

Integer swap_super_alignment_horizontal(Element &e1, Element &e2){
	Integer n1=0,n2=0;
	Get_super_alignment_horz_parts(e1,n1);
	Get_super_alignment_horz_parts(e2,n2);

	if(n1 == 0 || n2 == 0)return 1;
	Text part1[n1];
	Text part2[n2];

	Integer i;
	for(i=n1;i>=1;i--){
		Get_super_alignment_horz_part(e1,i,part1[i]);	// get hg from e1
		Super_alignment_horz_part_delete(e1,i);		// and remove it
	}
	for(i=n2;i>=1;i--){
		Get_super_alignment_horz_part(e2,i,part2[i]);	// get hg from e2
		Super_alignment_horz_part_delete(e2,i);		// and remove it
	}
	for(i=1;i<=n2;i++){
		Super_alignment_horz_part_append(e1,part2[i]);//append e2's parts onto e1
	}
	for(i=1;i<=n1;i++){
		Super_alignment_horz_part_append(e2,part1[i]);//append e1's parts onto e2
	}
	Integer ok=0;
	ok+=Calc_super_alignment_horz(e1);
	ok+=Calc_super_alignment_horz(e2);
	return ok;
}

Integer match_super_alignment_horizontal_from_sa(Element &e1, Element &e2){
	Integer n1=0,n2=0;
	Get_super_alignment_horz_parts(e1,n1);
	Get_super_alignment_horz_parts(e2,n2);

	if(n1 == 0)return 1;
	Text part1[n1];

	Integer i;
	for(i=n1;i>=1;i--){
		Get_super_alignment_horz_part(e1,i,part1[i]);	// get hg from e1
	}
	for(i=n2;i>=1;i--){
		Super_alignment_horz_part_delete(e2,i);		// remove hg from e2
	}
	for(i=1;i<=n1;i++){
		Super_alignment_horz_part_append(e2,part1[i]);//append e1's parts onto e2
	}
	Integer ok=0;
	ok+=Calc_super_alignment_horz(e2);
	return ok;
}

Integer get_super_alignment_horizontal_string_as_old_alignment(Element &e, Element &alignment){
	//try and convert directly to alignment, if fails extract the horizontal SA then do a convert
	Element e2,ret;
	Element_duplicate(e,e2);
	if(!Convert(e2,"Alignment",ret)){
		if(Element_exists(ret)){
			alignment = ret;
			return(0);
		}
	}
	Text type;
	Get_type(e,type);
	if(type == "Super_Alignment"){
		e2 = Get_super_alignment_horizontal_string(e);
		if(!Convert(e2,"Alignment",ret)){
			Text name;
			Get_name(e,name);
			Set_name(ret,name);
			alignment = ret;
			return(0);
		}else{
			Null(alignment);
			return 1;
		}
	}else{
		Null(alignment);
		return 2;
	}
	Null(alignment);
	return 3;
}

Text sa_get_part_name(Text part){
// return the name (or type) of the SA part
	Integer pos = Find_text(part, "{");
	Text part_type;
	if(pos != 0){
		return text_justify(Get_subtext(part,1,pos-1));
	}
	return "";
}

Integer sa_get_part_data_value(Text part, Text this_field, Text &value){
// search for a field and return it's value
//  looking through the SA parts one line at a time
	Text t = text_justify(part);
	while (1){
		Text left,right;
		Integer ierr = text_split(t,"\n",left,right);
		left = text_justify(left);
		Integer pos = Find_text(left,this_field + " "); //do we have a match for the value?
		if(pos != 0){
			value = text_justify(Get_subtext(left,Text_length(this_field)+2,Text_length(left)));
			return 0;
		}
		if(right == "")return -1;
		t = right;
	}
	return -2; // shouldn't get here !
}

Integer sa_get_part_data_value(Text part, Text this_field, Real &value){
// function overlaod
	Text t_value; Null(value);
	Integer ierr = sa_get_part_data_value(part, this_field, t_value);
	if(ierr != 0)return ierr;
	ierr = From_text(t_value,value);
	return ierr;
}

//--------------------------------------------------------------------------------
//                        VIP CALLS
//--------------------------------------------------------------------------------

Integer sa_check_vert_is_ip_method(Element &elt){
// check if the vips are a known ip method type
// return value of 0 indicates it is IP method
// will also return 0 if there are no VIP's
	Integer items;
	Get_super_alignment_vert_parts(elt,items);
	for(Integer i = 1 ; i <= items ; i++){
		Text part;
		Get_super_alignment_vert_part(elt,i,part);
		Text ip_type;
		Integer pos;
		pos = Find_text(part,"{");
		if(pos != 0){
			ip_type = text_justify(Get_subtext(part,1,pos-1));
		}else{
			return -2;
		}
		if(ip_type == "ip" || ip_type == "radius" || ip_type == "length" || ip_type == "max_length" || ip_type == "kvalue" || ip_type == "speed"){ // knwn ip methods
			continue;
		}else{
			return -1;
		}
	}
	return 0;
}



Integer sa_get_vips_between(Element &elt, Real start_chainage, Real end_chainage, Dynamic_Integer &vips){
// get the VIP part numbers with IP points between or eq to start/end chainage
// will only work with known VIP's of type IP method
	//p("start",start_chainage);p("end",end_chainage);
	Null(vips);
	Integer num_parts;
	Get_super_alignment_vert_parts(elt,num_parts);
	for(Integer i = 1 ; i <= num_parts ; i++){
		Text part;
		Get_super_alignment_vert_part(elt,i,part);
		Text ip_type = sa_get_part_name(part);
		if(ip_type == "ip" || ip_type == "radius" || ip_type == "length" || ip_type == "max_length" || ip_type == "kvalue" || ip_type == "speed"){ // only delete these VIP types
			Text t;
			Real ch;
			Integer ierr = sa_get_part_data_value(part,"x",ch);
			if(ierr !=0){//there was an error!, x not found!
				print("Error, no 'x' found for vip type "+ip_type);continue;
			}
			//Real ch;From_text(t,ch); // chainage of the VIP

			if(ch > start_chainage && ch < end_chainage || xeqy(ch,start_chainage) || xeqy(ch,end_chainage)){
				Append(i,vips);
				//p("adding to list",ch);
			}
		}
	}
	Integer items;
	Get_number_of_items(vips,items);
	if(items <= 0){
		return -1; // no ips to delete
	}
	return 0;
}

Integer sa_delete_vips(Element &elt, Real start_chainage, Real end_chainage){
// delte ip's if they are between or = to start and end chainges
// last_ip can be used as the starting point for inserting new geometry
	Integer last_ip = 0;
	Dynamic_Integer vips_to_delete;
	Integer ierr = sa_get_vips_between(elt,start_chainage,end_chainage,vips_to_delete);
	if(ierr == 0){
		Integer items;
		Get_number_of_items(vips_to_delete,items);
		for(Integer i = items; i >= 1 ; i--){
			Integer ip_num;
			Get_item(vips_to_delete,i,ip_num);
			Super_alignment_vert_part_delete(elt,ip_num);
			Calc_super_alignment_vert(elt);
			last_ip = ip_num;
		}
	}
	Null(vips_to_delete);
	return last_ip;
}

Integer sa_clear_vg(Element &elt){
	Integer items;
	Get_super_alignment_vert_parts(elt,items);
	for(Integer i = items ; i >= 1 ; i--){
		Super_alignment_vert_part_delete(elt,i);
	}
	return 0;
}

Integer sa_clear_hg(Element &elt){
	Integer items;
	Get_super_alignment_horz_parts(elt,items);
	for(Integer i = items ; i >= 1 ; i--){
		Super_alignment_horz_part_delete(elt,i);
	}
	return 0;
}

Integer sa_clear(Element &e){
	Integer ok = 0;
	ok+=sa_clear_vg(e);
	ok+=sa_clear_hg(e);
	return ok;
}


//--------------------------------------------------------------------------------
//                        SET NEW VG
//--------------------------------------------------------------------------------

Integer set_super_alignment_vips_via_text_method(Element &element,Real &ch[],Real &ht[], Integer &vips){
	sa_clear_vg(element);
	Integer id = 1;
	for(Integer i=1;i<=vips;i++){
		Super_alignment_vert_part_append(element,sa_ip(ch[i],ht[i],id));
	}
	Calc_super_alignment_vert(element);
	return 0;
}

void clear_old_alignemnt_vg(Element &e){
	Integer n;
	Get_vip_points(e,n);
	for(Integer i=n;i>=1;i--){
		Delete_vip(e,i);
	}
}

Integer set_super_alignment_vips_via_old_alignment_method(Element &e, Real &ch[], Real &ht[], Integer &vips){
	//this is a hack which relys on some convert() calls to work - although seems to be reliable
	//it is a much quicker method than the standard calls which pump in a text string
	Integer ok=0;
	//create an old alignment
	Element hg = Get_super_alignment_horizontal_string(e);
	Element alignment;
	ok+=Convert(hg,"Alignment",alignment);
	clear_old_alignemnt_vg(alignment);

	for(Integer i=1;i<=vips;i++){
		Append_vip(alignment,ch[i],ht[i]);	//append vips to new "alignment" string
	}
	Calc_alignment(alignment);
	Element vg;
	ok+=Convert(alignment,"Super_Alignment",vg);	//this new element has the vg we want
	//get hg from other string
	match_super_alignment_horizontal_from_sa(e,vg);	//now the vg string has the correct hg
	Calc_extent(vg);
	if(ok){
		return ok;
		//error and return before try and replace geometry?
	}
	ok+=String_replace(vg,e);	//take vg's geometry and replace e's geometry
	Calc_extent(e);
	Element_delete(alignment);
	Element_delete(hg);
	Element_delete(vg);
	return ok;
}

Integer set_super_alignment_vips(Element &e, Real &ch[], Real &ht[], Integer &vips){
	if(set_super_alignment_vips_via_old_alignment_method(e, ch, ht, vips)){
		return set_super_alignment_vips_via_text_method(e, ch, ht, vips);
	}
	return 0;
}


/*
Integer Super_alignment_vert_part_delete(Element element,Integer position);
Integer Get_super_alignment_vert_part(Element element,Integer position,Text &part);

Integer Calc_super_alignment_vert(Element);

Integer Get_super_alignment_vert_part_id(Element element,Integer position,Integer &id);


Integer Get_super_alignment_vert_parts(Element element,Integer &num_parts);



Integer Super_alignment_vert_part_insert(Element element,Integer position,Text part);



Integer Super_alignment_vert_part_append(Element element,Text part);

	return 0;
}

*/

#endif

