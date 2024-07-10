#ifndef mashy_lib_element_functions_included
#define mashy_lib_element_functions_included

Text get_full_name(Element &e);
Integer get_uid(Element &e,Uid &uid_elt,Uid &uid_mdl);

#include "mashy_lib_get_item.H"

#include "12d/standard_library.H"
#include "mashy_lib_debug.H"
#include "mashy_lib_sort_qsort.H"
#include "mashy_lib_geometry.H"
#include "mashy_lib_log_lines.H"

// WIP
Integer filter_3d(Element &e, Real pipe_dia){


	Integer npts;
	Get_points(e,npts);
	Integer last_p = npts - 2;
	for(Integer i=1;i<=last_p;i++){
		Real x[3],y[3],z[3];
		Get_data(e,i,  x[1],y[1],z[1]);
		Get_data(e,i+1,x[2],y[2],z[2]);
		Get_data(e,i+2,x[3],y[3],z[3]);
	}

	return 0;
}

Text get_type(Element &e){
	Text type="";
	Get_type(e,type);
	return type;
}

Integer get_points(Element &e){
	Integer npts;
	if(Get_points(e,npts))return 0;
	return npts;
}

// Match standard element header stuff, form "source" to "target"
Integer match_header(Element &source, Element &target){
   Integer colour;
   Get_colour(source,colour);
   Text name;
   Get_name(source,name);
   Text style;
   Get_style(source,style);
   Real chainage;
   Get_chainage(source,chainage);
   Integer time_u;
   Get_time_updated(source,time_u);
   Real weight;
   Get_weight(source,weight);

   Integer ok=0;
   ok+=Set_colour(target,colour);
   ok+=Set_name(target,name);
   ok+=Set_style(target,style);
   ok+=Set_chainage(target,chainage);
   ok+=Set_time_updated(target,time_u);
   ok+=Set_weight(target,weight);

   return ok;
}

Integer string_match(Element &source, Element &target){
	Integer ok=0;
	ok+=match_header(source, target);
	Model m;
	ok+=Get_model(source,m);
	ok+=Set_model(target,m);
	return ok;
}


Integer string_replace(Element &source, Element &target){
	Integer ok=0;
	ok+=match_header(source, target);
	//Model m;
	//Get_model(source,m);
	ok+=String_replace(target,source);	// takes target strings geometry and puts on source
	//Calc_extent(source);
	return ok;
}

Integer count_points(Dynamic_Element &delt, Integer &npts){
	Integer nelt;
	Get_number_of_items(delt,nelt);
	npts = 0;
	for(Integer i=1;i<=nelt;i++){
		Element e;
		Get_item(delt,i,e);
		Integer this_npts;
		Get_points(e,this_npts);
		npts+=this_npts;
	}
	return 0;
}

Integer count_points(Model &m, Integer &npts){
	Dynamic_Element delt;
	Append(m,delt);
	return count_points(delt,npts);
}

Integer get_end_points(Element e, Real x[], Real y[], Real z[]){
	Text type;
	Integer clean_up = 0;
	Get_type(e,type);
	if(type=="Super_Alignment"){
		e = Get_super_alignment_horizontal_string(e);
		clean_up++;
	}else if (type=="Alignment"){
		Convert(e,"Super",e);
		clean_up++;
	}
	Integer points;
	Get_points(e,points);
	if(points<1)return(1);
	Get_data(e,1,x[1],y[1],z[1]);
	Get_data(e,points,x[2],y[2],z[2]);
	if(clean_up)
		Element_delete(e);
	return(0);
}

Integer is_null(Element &e){
	Uid uid;
	Get_id(e,uid);
	return(Is_null(uid));
}

Integer is_element_clockwise(Element &e){
//1 = clockwise
// 0 = anti
	Integer points;
	Get_points(e,points);
	if(points < 1) return -1;
	Real x[points],y[points],z;
	for(Integer i=1;i<=points;i++){
		Get_data(e,i,x[i],y[i],z);
	}
	Real area = get_area(x, y, points);
	if(area < 0)return 1;
	return 0;
}

Integer is_e_closed(Element &e){
	Integer is_closed;
	String_closed(e,is_closed);
	return is_closed;
}
Integer is_inside(Real x, Real y, Element &element){
	Integer is_closed;
	String_closed(element,is_closed);
	if(!is_closed)return 0;

	Real xmin,xmax,ymin,ymax;
	Get_extent_x  ( element,xmin,xmax);
	Get_extent_y  ( element,ymin,ymax);
	if(x < xmin || x > xmax)return 0;
	if(y < ymin || y > ymax)return 0;

	Line seg_2;
	Point p1,p2;
	Set_x(p1,x);	Set_y(p1,y);
	Set_x(p2,xmax+1);	Set_y(p2,y);
	Set_start(seg_2,p1);	Set_end(seg_2,p2);

	Integer nsegs;
	Get_segments(element,nsegs);
	Integer count = 0;
	for(Integer i = 1;i<=nsegs;i++){
		Segment seg_1;
		Get_segment(element,i,seg_1);
		Integer no_intersects = 0;
		if(!Intersect       ( seg_1, seg_2,no_intersects,p1,p2)){
			count +=no_intersects;
		}
	}
	if(count % 2)return 1;// if there is a remainder then the count is odd
	return 0;
}

Integer set_function_id(Dynamic_Element &delt, Macro_Function &macro_function){
	Integer items;	Get_number_of_items(delt,items);
	Uid fid;
	Get_id(macro_function,fid);
	for(Integer i=1;i<=items;i++){
		Element e;	Get_item(delt,i,e);
		Set_function_id(e,fid);
	}
	return(0);
}

Integer check_point_is_within_extents(Real &x, Real &y, Element &elt){
//return 1 if its with the extents
	Real xmin,ymin,xmax,ymax;
 	Get_extent_x  ( elt, xmin, xmax);
 	Get_extent_y  ( elt, ymin, ymax);
#if VERSION_4D > 1400
 	if( Xgey(x,xmin) ){
 		if( Xley(x,xmax) ){
 			if( Xgey(y,ymin) ){
 				if( Xley(y,ymax) ){
 					return (1);
 				}
 			}
 		}
 	}
#else
 	if( xgey(x,xmin) ){
 		if( xley(x,xmax) ){
 			if( xgey(y,ymin) ){
 				if( xley(y,ymax) ){
 					return (1);
 				}
 			}
 		}
 	}
#endif
 	return (0);
}

Integer get_extents(Dynamic_Element &elts, Real &min_x, Real &max_x, Real &min_y, Real &max_y){
	Integer items;	Get_number_of_items(elts,items);
	Null(min_x);Null(max_x);Null(min_y);Null(max_y);
	for(Integer i=1;i<=items;i++){
		Element elt;	Get_item(elts,i,elt);
		Real minx,maxx,miny,maxy;
		Get_extent_x(elt,minx,maxx);
		Get_extent_y(elt,miny,maxy);
		if(i==1){
			min_x=minx;	max_x=maxx;	min_y=miny;	max_y=maxy;
		}else{
			if(minx < min_x)min_x=minx;
			if(maxx > max_x)max_x=maxx;
			if(miny < min_y)min_y=miny;
			if(maxy > max_y)max_y=maxy;
		}

	}
	return(0);
}

Integer get_extents(Element &e, Real &xmin, Real &xmax, Real &ymin, Real &ymax){
	Integer ok=0;
	ok+=Get_extent_x(e,xmin,xmax);
	ok+=Get_extent_y(e,ymin,ymax);
	return(ok);
}

Integer check_overlap_extents(Element &e, Element &b){
	Integer overlap=0;
	Real xmin1,xmax1,ymin1,ymax1,xmin2,xmax2,ymin2,ymax2;
	get_extents(e, xmin1, xmax1, ymin1, ymax1);
	get_extents(b, xmin2, xmax2, ymin2, ymax2);

	// oldschool way -> check it if there are bugz! if(xmax1 >= xmin2 && xmin1 <= xmax2 && ymin1 <= ymax2 && ymax1 >=ymin2 )
#if VERSION_4D > 1400
	if( Xgey(xmax1,xmin2) && Xley(xmin1,xmax2) && Xley(ymin1,ymax2) && Xgey(ymax1,ymin2) )
#else
	if( xgey(xmax1,xmin2) && xley(xmin1,xmax2) && xley(ymin1,ymax2) && xgey(ymax1,ymin2) )
#endif
	{ // colission found! :) !
		return 1;
	}else
	{ // no collision :(
		return 0;
	}
	return(0);
}

Integer set_name(Dynamic_Element &elts, Text name){
// set the name of every element in elts
	Integer items;	Get_number_of_items(elts,items);
	for(Integer i=1;i<=items;i++){
		Element elt;	Get_item(elts,i,elt);
		Valid_string_name(name,name);
		Set_name(elt,name);
		Calc_extent(elt);
	}
	return(0);
}

Text valid_string_name(Text t){
	Text t2;
	Valid_string_name(t,t2);
	return (t2);
}


Integer check_for_locks(Element &elt){
// check an element for locks, return 0 if clear, -ve if lock exists
	Integer no_locks;
	Get_read_locks(elt,no_locks); // check for read locks
	if (no_locks != 1) return -1; // I reckon this is 1 is because we need to read the element to check if there are any locks!
	Get_write_locks(elt,no_locks); // check for write locks
	if (no_locks != 0) return -2;
	return 0; // no lock
}

Integer check_for_locks(Dynamic_Element &delt){
//check each element in model for locks
	Integer no_locks = 0;
	Integer items;
	Get_number_of_items(delt,items);
	for (Integer i = 1 ; i <= items ; i++){
		Element elt;
		Get_item(delt,i,elt);
		no_locks += check_for_locks(elt);
	}
	return no_locks;

}


Integer check_for_locks(Model &mdl){
//check each element in model for locks
	Integer no_locks = 0;
	Dynamic_Element delt;
	Append(mdl,delt);
	Integer items;
	Get_number_of_items(delt,items);
	for (Integer i = 1 ; i <= items ; i++){
		Element elt;
		Get_item(delt,i,elt);
		no_locks += check_for_locks(elt);
	}
	return no_locks;

}

Dynamic_Element get_elements(Text model_name){
// get all the elements in the project which match model_name & string_name as wildcards
	Dynamic_Element de_models;
	Integer ierr;
	Dynamic_Text dt_mdls;
	ierr =Get_project_models(dt_mdls);
	Integer count_items;
	ierr = Get_number_of_items(dt_mdls,count_items);
	for(Integer i = 1;i<=count_items;i++){
		Text t_mdl;
		Get_item(dt_mdls,i,t_mdl);
		if( Match_name(t_mdl,model_name) !=0){
			Append(Get_model(t_mdl),de_models);// append all strings in the model to de_models
			#if DEBUG
				p(model_name + " MATCHES " + t_mdl);
			#endif
		}
	}
	return de_models;

}

Dynamic_Element get_elements(Text model_name, Text string_name){
// get all the elements in the project which match model_name & string_name as wildcards
	Dynamic_Element de_models,de_return;
	Integer ierr;
	Dynamic_Text dt_mdls;
	ierr =Get_project_models(dt_mdls);
	Integer count_items;
	ierr = Get_number_of_items(dt_mdls,count_items);
	for(Integer i = 1;i<=count_items;i++){
		Text t_mdl;
		Get_item(dt_mdls,i,t_mdl);
		if( Match_name(t_mdl,model_name) ){
			Append(Get_model(t_mdl),de_models);// append all strings in the model to de_models
			#if DEBUG
				p(model_name + " MATCHES " + t_mdl);
			#endif
		}
	}
	Match_name(de_models,string_name,de_return);
	Null(dt_mdls);
	Null(de_models);
	return de_return;
}

Integer get_element(Dynamic_Element &delt, Text string_name, Element &first_found_e){
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element this_e;
		Get_item(delt,i,this_e);
		Text name;
		Get_name(this_e,name);
		if( Match_name(name, string_name) ){
			first_found_e = this_e;
			return 0;
		}
	}
	return 1;
}

Integer get_element(Text model_name, Uid &model_id, Text element_name, Uid &element_id, Element &e){
//return 0 is good
//return 1 fails
//return > 1 means more than 1 string was found, e is set to the first string found!
// if(!get_element) for only 1 match
// if(get_element != 1) for an number of matches, but will only return the first string
	Integer ok =0;
	if(Get_element(model_id,element_id,e)){
		Dynamic_Element delt = get_elements(model_name,element_name);
		Integer items;
		Get_number_of_items(delt,items);
		if(items >= 1){
			Get_item(delt,1,e);
		}
		if(items == 1)
			ok = 0;
		else if (items == 0)
			ok=1;
		else
			ok = items;
	}
	if(ok!=0){
		if(ok == 1){
			Print("Model ID + Element ID does not exist, search for Model Name->Element Name found more than 1 element! <"+To_text(ok)+"> elements found");Print();
			return ok;
		}else{
			Print("Model ID + Element ID does not exist, search for Model Name->Element Name found no elements!");Print();
			return ok;
		}
	}
	return ok;
}


Integer get_next_uid(){
// get next uid as an integer
	Integer id;
	Uid uid = Get_next_uid();
	Convert_uid(uid,id);
	return id;
}

Text get_full_name(Element &e){
//return the text "MODEL NAME->STRING NAME"
	Text t,tm,te;
	Model m;
	Get_model(e,m);
	Get_name(m,tm);
	Get_name(e,te);
	t = tm + "->" + te;
	return t;
}

Text get_element_name(Element &e){
//return the name of the element as text
	Text t;
	Get_name(e,t);
	return t;
}

Text get_element_model(Element &e){
//return the name of the model which element is in
	Text tm;
	Model m;
	Get_model(e,m);
	Get_name(m,tm);
	return tm;
}
//this should be depreciated cause I fucked up the order of the variables.. use get_id() in future to match 12d order (mdl/elt)
Integer get_uid(Element &e,Uid &uid_elt,Uid &uid_mdl){
	Integer ierr;
	ierr = Get_id(e,uid_elt);
	Model m;
	ierr += Get_model(e,m);
	ierr += Get_id(m,uid_mdl);
	return ierr;
}

Integer is_same(Element &e, Element &e2){
	// Uid uid1,uid11,uid2,uid22;
	// get_uid(e,uid1,uid11);
	// get_uid(e2,uid2,uid22);
	Uid uid1,uid2;
	Get_id(e,uid1);
	Get_id(e2,uid2);

	if(uid1 == uid2){
		return 1;
	}
	return 0;
}

Integer get_id(Element &e,Uid &uid_mdl,Uid &uid_elt){
	Integer ierr;
	ierr = Get_id(e,uid_elt);
	Model m;
	ierr += Get_model(e,m);
	ierr += Get_id(m,uid_mdl);
	return ierr;
}

Integer get_id(Element &e){
//return the id as an integer
	Uid uid;
	Integer i;
	Get_id(e,uid);
	Convert_uid(uid,i);
	return i;

}

Integer get_elements_model_id(Element e){
//return the id as an integer
	Uid uid;
	Model m;
	Get_model(e,m);
	Get_id(m,uid);
	Integer i;
	Convert_uid(uid,i);
	return i;

}

Integer get_id(Model m){
//return the id as an integer
	Uid uid;
	Integer i;
	Get_id(m,uid);
	Convert_uid(uid,i);
	return i;

}

Integer get_element_names_ids(Element &element, Text &model_name, Integer &model_id, Text &string_name, Integer &string_id){
//return the above info
	model_name  = get_element_model(element);
	model_id    = get_id(Get_model(get_element_model(element)));
	string_name = get_element_name(element);
	string_id   = get_id(element);
	return(0);
}

Integer get_element_name_ids(Element &e,Text &string){
// string is in the \t seperated format
	Text	model_name,string_name;
	Integer	model_id,string_id;
	Integer ierr = get_element_names_ids(e,model_name,model_id,string_name,string_id);
	string = model_name + "\t" + To_text(model_id) + "\t" + string_name + "\t" + To_text(string_id);
	return(0);
}

Integer get_element_names_ids(Element &element, Text &model_name, Uid &model_id, Text &element_name, Uid &element_id){
//return the above info
	Model m;
	Integer ok=0;
	ok+=Get_model(element,m);
	ok+=Get_name(m,model_name);
	ok+=Get_id(m,model_id);
	element_name = get_element_name(element);
	ok+=Get_id(element,element_id);
	return(ok);
}

//ALIGNMENTS	{6E0DA271-896C-48cd-ABE5-D58FD68E27C2-00000000012E152A}	ROAD B	{6E0DA271-896C-48cd-ABE5-D58FD68E27C2-00000000012E152C}
//ALIGNMENTS\t{6E0DA271-896C-48cd-ABE5-D58FD68E27C2-00000000012E152A}\tROAD B\t{6E0DA271-896C-48cd-ABE5-D58FD68E27C2-00000000012E152C}
Text get_element_string(Element &element){
	Text model_name,  element_name;
	Uid model_id, element_id;
	get_element_names_ids(element, model_name, model_id, element_name, element_id);
	return model_name + "\t" + To_text(model_id) + "\t" + element_name + "\t" + To_text(element_id);
}


///THIS IS NOT YET TESTED !

// input a text string and find the matching element
// model name\tmodel id\tstring name\tstring id
// NOTE - the return value is the nubmer of strings found.
// because it is possible that if the UID's are not part of the string name, or the ID's are potentially invalid, the string
// is searched for using the model name + string name. Is this case it is possible for > 1 string to match.
// Therefore a return of 1 means the funciton successfully returned the correct string
// > 1 means it is returning the first found string, but there are others which match
// 0 means that the a string was not returned!
Integer get_element(Text string,Element &e){
	Integer tab = '\t';
	Text model_name="",model_id="",name="",id="";
	Integer len = Text_length(string);

	Integer pos = 1;
	Integer last_pos = 1;
	Integer count = 0;
	while(1){
		if(pos > len){
			break;
		}

		Integer c1;
		Get_char(string,pos,c1);
		if(c1 == tab || pos == len){
			count++;
			Text this_part = "";
			if(pos != len){
				if( (pos - last_pos) > 0 ){
					this_part = Get_subtext(string,last_pos,pos-1);
				}
			}else{
				if( (pos - last_pos) >= 0){
					this_part = Get_subtext(string,last_pos,pos);
				}
			}
			last_pos = pos + 1; // where to start to break up the next text string
			if(count == 1){
				model_name = this_part;
			}else if(count == 2){
				model_id = this_part;
			}else if(count == 3){
				name = this_part;
			}else if(count == 4){
				id = this_part;
			}else{

			}
		}

		pos++;
	}

	Uid uid_model,uid_e;
	Integer uid_error=0;
	if(!From_text(model_id,uid_model)){
		if(From_text(id,uid_e)){
			if(!Get_element(uid_model,uid_e,e)){
				return (1);
			}
		}
	}

	//get id failed, try looking for element by model name and string name
	Dynamic_Element delt = get_elements(model_name,name);
	Integer items;
	Get_number_of_items(delt,items);
	if(items > 0){
		Get_item(delt,1,e); // get the first element only
		return items; //
	}

	return 0;
}

Integer append_unique(Element &e, Dynamic_Element &delt){
// only add unique elements to the list
 	Uid id,mid;
	Get_id(e,id);
 	//get_id(e,mid,id);
 	Integer items;
 	Get_number_of_items(delt,items);
 	//Integer match_found = 0;
 	for(Integer i = 1;i<=items;i++){
 		Element this_e;
 		Get_item(delt,i,this_e);
 		Uid this_id,this_mid;
		Get_id(this_e,this_id);
 		// get_id(this_e,this_mid,this_id);
 		if(this_id == id){
 			//if(this_mid == mid){
 			//	match_found = 1;
 				return 0;
 			//}
 		}
 	}
 	return Append(e,delt);
 }

Integer append_unique_elements_from_mask(Text model_mask, Text element_mask, Dynamic_Element &delt){
	Dynamic_Element delt1 = get_elements(model_mask, element_mask);
	Integer items;
	Get_number_of_items(delt1,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(delt1,i,e);
		ok+=append_unique(e,delt);
	}
	return ok;
}

Integer append_unique_elements_from_mask(Dynamic_Text &model_masks, Dynamic_Text &element_masks, Dynamic_Element &delt){
	Integer items=0;
	Get_number_of_items(model_masks,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text model_mask="",element_mask="";
		Get_item(model_masks,i,model_mask);
		Get_item(element_masks,i,element_mask);
		ok+=append_unique_elements_from_mask( model_mask, element_mask, delt);
	}
	return ok;
}



// IS ELEMENT VALID 
Integer is_valid(Element &e, Integer do_output_messages){
	Uid uid;
	Get_id(e,uid);
	if(Is_null(uid)){
		if(do_output_messages)printe("ERROR: Element uid does not exist!\n");
		return 0;	//if model handle is dud fails
	}
	if(Is_global(uid)){

		if(do_output_messages)printe("ERROR: element <"+get_full_name(e)+"> is a shared element!\n");
		return 0; //if model handle is shared fails
	}
	return 1;
}
// some function overlodes for is_valid()
Integer is_valid(Element &e){
	return (is_valid(e,0));
}

Integer has_write(Element &e){
	return (is_valid(e,1));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Integer get_chainages(Element &elt,Real &start_ch, Real &end_ch){

	Integer ierr1 = Get_chainage(elt,start_ch);
	Integer ierr2 = Get_end_chainage(elt,end_ch);
	if(ierr1 != 0)return -1;
	if(ierr2 != 0)return -2;
	return 0;
}

Integer debug_dynamic_element(Dynamic_Element &de){
//dump out a list of all elements in de
	Integer count_items,ierr;
	ierr = Get_number_of_items(de,count_items);
	for(Integer i = 1;i<=count_items;i++){
		Element elt;
		Get_item(de,i,elt);
		Print(get_full_name(elt)+"\n");
	}
	return 1;
}


Element find_closest_string(Point &pt1,Dynamic_Element &de){
// return the element (from de) which is closest to pt1
	Real x,y,z,x1,y1,z1,d1,d2,off,off2;
	x = Get_x(pt1); y = Get_y(pt1); z=0;

	//Cut_strings_with_nulls(str,mdl,elt);
// *** NEED TO FIND THE STRING WHICH IS CUT
// for each string in mdl drop point and check the offset to work out the closest string
	Integer no_elt,no_elt_cut;
	Get_number_of_items(de,no_elt);
	off2 = 9999999;
	for(Integer i=1;i<=no_elt;i++) {
		Element str1;
		Get_item(de,i,str1);
		Drop_point(str1,x,y,z,x1,y1,z1,d1,d2,off);
		Real tmp = -1.0;
		if(off < 0 ){
			off = off * tmp;
		}
		if(off < off2){
			off2 = off;
			no_elt_cut = i;
		}
	}
	// MAYBE CHECK IF off2 is over a certain tolerance there is an error
	Element str1;
	Get_item(de,no_elt_cut,str1); // get the item number in the model array of the closest string
#if DEBUG
	Text txt;
	Get_name(str1,txt);
	Print("\nfind_closest_string()->STRING NAME CUT -> "+ txt +"  Offset " + To_text(off2,3) +"\n");
#endif
	return str1;
}
Integer find_closest_string(Real &x, Real &y, Dynamic_Element &de, Element &element, Real &xf, Real &yf, Real &zf, Real &distance, Real &dir, Real &chainage){

//this is a newer function than the one above
	Real z=0.0,x1,y1,z1,d1,d2,off;
	Null(distance);
// for each string in mdl drop point and check the offset to work out the closest string
	Integer no_elt,no_elt_cut;
	Get_number_of_items(de,no_elt);
	if(no_elt < 1)return 1;
	for(Integer i=1;i<=no_elt;i++) {
		Element str1;
		Get_item(de,i,str1);
		Drop_point(str1,x,y,z,x1,y1,z1,d1,d2,off);
		if(off < 0.0 ){
			off = off * (-1.0);
		}
		if(i==1){
			distance = off;
			dir = d2;
			chainage = d1;
			no_elt_cut = i;
			xf = x1;yf = y1;zf = z1;
			continue;
		}
		if(off < distance){
			distance = off;
			dir = d2;
			chainage = d1;
			no_elt_cut = i;
			xf = x1;yf = y1;zf = z1;
		}
	}
	// MAYBE CHECK IF off2 is over a certain tolerance there is an error
	Get_item(de,no_elt_cut,element); // get the item number in the model array of the closest string
#if DEBUG
	Text txt;
	Get_name(element,txt);
	Print("\n->STRING NAME CUT -> "+ txt +"  Offset " + To_text(off,3) +"\n");
#endif
	return 0;
}

Integer find_closest_string(Real &x, Real &y, Dynamic_Element &de, Element &element, Real &xf, Real &yf, Real &zf, Real &distance, Real &dir){
//this is a newer function than the one above
	Real z=0.0,x1,y1,z1,d1,d2,off;
	Null(distance);
// for each string in mdl drop point and check the offset to work out the closest string
	Integer no_elt,no_elt_cut;
	Get_number_of_items(de,no_elt);
	if(no_elt < 1)return 1;
	for(Integer i=1;i<=no_elt;i++) {
		Element str1;
		Get_item(de,i,str1);
		Drop_point(str1,x,y,z,x1,y1,z1,d1,d2,off);
		if(off < 0.0 ){
			off = off * (-1.0);
		}
		if(i==1){
			distance = off;
			dir = d2;
			no_elt_cut = i;
			xf = x1;yf = y1;zf = z1;
			continue;
		}
		if(off < distance){
			distance = off;
			dir = d2;
			no_elt_cut = i;
			xf = x1;yf = y1;zf = z1;
		}
	}
	// MAYBE CHECK IF off2 is over a certain tolerance there is an error
	Get_item(de,no_elt_cut,element); // get the item number in the model array of the closest string
#if DEBUG
	Text txt;
	Get_name(element,txt);
	Print("\n->STRING NAME CUT -> "+ txt +"  Offset " + To_text(off,3) +"\n");
#endif
	return 0;
}

Integer find_closest_string(Real &x, Real &y, Dynamic_Element &de, Element &element, Real &xf, Real &yf, Real &zf, Real &distance){
//this is a newer function than the one above
	Real z=0.0,x1,y1,z1,d1,d2,off;
	Null(distance);
// for each string in mdl drop point and check the offset to work out the closest string
	Integer no_elt,no_elt_cut;
	Get_number_of_items(de,no_elt);
	if(no_elt < 1)return 1;
	for(Integer i=1;i<=no_elt;i++) {
		Element str1;
		Get_item(de,i,str1);
		Drop_point(str1,x,y,z,x1,y1,z1,d1,d2,off);
		if(off < 0.0 ){
			off = off * (-1.0);
		}
		if(i==1){
			distance = off;
			no_elt_cut = i;
			xf = x1;yf = y1;zf = z1;
			continue;
		}
		if(off < distance){
			distance = off;
			no_elt_cut = i;
			xf = x1;yf = y1;zf = z1;
		}
	}
	// MAYBE CHECK IF off2 is over a certain tolerance there is an error
	Get_item(de,no_elt_cut,element); // get the item number in the model array of the closest string
#if DEBUG
	Text txt;
	Get_name(element,txt);
	Print("\n->STRING NAME CUT -> "+ txt +"  Offset " + To_text(off,3) +"\n");
#endif
	return 0;
}

Integer find_closest_string(Real &x, Real &y, Dynamic_Element &de, Element &element){
//this is a newer function than the one above
	Real z=0,x1,y1,z1,d1,d2,off,off2;
// for each string in mdl drop point and check the offset to work out the closest string
	Integer no_elt,no_elt_cut;
	Get_number_of_items(de,no_elt);
	if(no_elt < 1)return 1;
	for(Integer i=1;i<=no_elt;i++) {
		Element str1;
		Get_item(de,i,str1);
		Drop_point(str1,x,y,z,x1,y1,z1,d1,d2,off);
		if(off < 0 ){
			off = off * (-1.0);
		}
		if(i==1){
			off2 = off;
			no_elt_cut = i;
			continue;
		}
		if(off < off2){
			off2 = off;
			no_elt_cut = i;
		}
	}
	// MAYBE CHECK IF off2 is over a certain tolerance there is an error
	Get_item(de,no_elt_cut,element); // get the item number in the model array of the closest string
#if DEBUG
	Text txt;
	Get_name(element,txt);
	Print("\n->STRING NAME CUT -> "+ txt +"  Offset " + To_text(off2,3) +"\n");
#endif
	return 0;
}


Integer elements_duplicate(Dynamic_Element &from, Dynamic_Element &to){
	Integer items;	Get_number_of_items(from,items);
	Integer ierr=0;
	for(Integer i=1;i<=items;i++){
		Element elt_from,elt_to;
		Get_item(from,i,elt_from);
		if(Element_duplicate(elt_from,elt_to));
		else ierr++;
		Append(elt_to,to);
	}
	return(ierr);

}

Integer elements_delete_slow_method(Dynamic_Element &delt){
// delete all the elements in delts
	Integer items;
	Get_number_of_items(delt,items);
	for (Integer i = items ; i >= 1 ; i--){
		Element elt;
		Get_item(delt,i,elt);
		if( Element_delete(elt) )return -1;
	}
	return 0;
}


Integer elements_delete(Dynamic_Element &delt){
// this is the faster way to delete a bunch of elements, set them to a model, then delete the model
	Integer do_slow_method = 1; // this only executes if the other method fails
	for(Integer i=1;i<=100;i++){	//try upto 20 times to create a unique model
		Model m = Create_model("12d macro delete "+To_text(i));
		Uid uid;
		Get_id(m,uid);
		if(Is_null(uid))continue;
		else{
			Set_model(delt,m);
			Null(delt);
			Model_delete(m);
			return 0;
		}
	}
	if(do_slow_method) return elements_delete_slow_method(delt);
	return 10;
}


Integer elements_delete_check_locks(Dynamic_Element &delts){
// delete all the elements in delts
	Integer items;
	Get_number_of_items(delts,items);
	Integer ok=0;
	Dynamic_Element new_list;
	for (Integer i = items ; i >= 1 ; i--){
		Element elt;
		Get_item(delts,i,elt);
		if(check_for_locks(elt)){
			ok++;
		}else{
			Append(elt,new_list);// Element_delete(elt);
		}
	}
	elements_delete(new_list);
	return ok;
}

Integer elements_draw(Dynamic_Element &delts,Integer colour){
// draw the eleement
	Integer items;
	Get_number_of_items(delts, items);

	for (Integer i = 1; i <= items; i++) {
		Element string;
		Get_item (delts,i,string);
		Element_draw(string,colour);
	}
	return 0;
}

Integer elements_draw(Dynamic_Element &delts){
// draw the eleement
	Integer items;
	Get_number_of_items(delts, items);

	for (Integer i = 1; i <= items; i++) {
		Element string;
		Get_item (delts,i,string);
		Element_draw(string);
	}
	return 0;
}


Integer reorder_elements_in_model(Model model,Integer mode){
// reorder the elements in a model so that they appear alphabetical in the 12d database
// mode 1 - alphabetical
// mode -1 - reverse alphabetical
	Dynamic_Element elts;
	Append(model,elts);
	Integer items;
	Get_number_of_items(elts,items);
	if(items<1)return(-1);
	Integer index[items];
	Dynamic_Text list;
	for(Integer i=1;i<=items;i++){ //make a list of the element names
		Element elt;
		Get_item(elts,i,elt);
		Text name;
		Get_name(elt,name);
		Append(name,list);
	}
	Qsort(list,index,items);
	if(mode==1){ //alphabetical
		for(i=1;i<=items;i++){ //reorder the model by setting the model for each element from index[]
			Element elt;
			Get_item(elts,index[i],elt);
			Set_model(elt,model);
		}
	}else if(mode==-1){ // reverse alphabetical
		for(i=items;i>=1;i--){ //reorder the model by setting the model for each element from index[]
			Element elt;
			Get_item(elts,index[i],elt);
			Set_model(elt,model);
		}
	}else{
		return(-2); //unknown mode
	}
	return(0);
}

Integer reorder_elements_in_model(Model &model){
	return(reorder_elements_in_model(model,1));
}

Integer debug_element(Element &e){
	Integer points;	Get_points(e,points);
	Print("Dumping <" + get_full_name(e) + ">, points <" + To_text(points) +">");Print();
	for(Integer i=1;i<=points;i++){
		Real x,y,z;
		Get_data(e,i,x,y,z);
		Print("<"+To_text(i)+">"+To_text(x,3)+","+To_text(y,3)+","+To_text(z,3));Print();
	}
	return(0);
}

Integer debug_element(Dynamic_Element &de){
	Integer items;	Get_number_of_items(de,items);
	for(Integer i=1;i<=items;i++){
		Element e;	Get_item(de,i,e);
		debug_element(e);
	}
	return (0);
}

//--------------------------------------------------------------------------------
//                        4d Strings
//--------------------------------------------------------------------------------

Integer get_4d_data(Element &e, Real x[], Real y[], Real z[], Text t[], Real ch[], Integer max_pts, Integer &num_pts){
//tis expands 12d's native call to also get the chainage of each point
	Integer ok=0;
	ok+=Get_4d_data(e,x,y,z,t,max_pts,num_pts);
	for(Integer i=1;i<=max_pts;i++){
		Real xf,yf,zf,inst_dir,off;
		ok+=Drop_point(e,x[i],y[i],z[i],xf,yf,zf,ch[i],inst_dir,off);
	}
	return (0);
}

Integer get_4d_point_chainage(Element &e, Text pt_name, Real &chainage){
// 0 means the point name was found and a chaiange was set
	Null(chainage);
	Integer ok=0;
	Integer max_pts;
	Get_points(e,max_pts);
	Real x[max_pts], y[max_pts], z[max_pts], ch[max_pts];
	Text t[max_pts];
	Integer num_pts;
	get_4d_data(e,x,y,z,t,ch,max_pts,num_pts);
	for(Integer i=1;i<=num_pts;i++){
		if(pt_name == t[i] ){
			chainage = ch[i];
			return (0);
		}
	}

	return (1);
}

Integer get_4d_zero_chainage(Element &e, Integer &point_number){
	Integer points=0;
	Get_points(e,points);
	if(points < 1) return 1;
	Real x[points],y[points],z[points],ch[points];
	Text t[points];
	Integer num_pts;
	get_4d_data(e, x, y, z, t, ch, points, num_pts);
	for(Integer i=1;i<=num_pts;i++){
		if(ch[i]==0.0){
			point_number = i;
			return (0);
		}
	}
	return 2;
}

Integer get_4d_lowest_non_zero_chainage(Element &e, Integer &point_number){
	Integer points=0;
	Get_points(e,points);
	if(points < 1) return 1;
	Real x[points],y[points],z[points],ch[points];
	Text t[points];
	Integer num_pts;
	get_4d_data(e, x, y, z, t, ch, points, num_pts);
	Real lowest_chainage;
	for(Integer i=1;i<=num_pts;i++){
		if(i==1){
			point_number = 1;
			lowest_chainage = Absolute(ch[i]);
			continue;
		}
		if(Absolute(ch[i])<lowest_chainage){
			point_number = i;
			lowest_chainage = Absolute(ch[i]);
		}
	}
	//Model cad_model = Get_model_create("macro debug");
	//Set_model(e,cad_model);
	//Print(To_text(lowest_chainage,3));Print();
	return 0;
}

Integer xeqy_2d(Point &p1, Point &p2){
	Real mindif = 1.0e-6;
	Real x1,x2,y1,y2;
	x1 = Get_x(p1);	x2 = Get_x(p2);
	y1 = Get_y(p1);	y2 = Get_y(p2);
	if(xeqy(x1,x2)){
		if(xeqy(y1,y2)){
			return 1;
		}
	}
	return 0;
}


Integer super_insert_vertex(Element &e, Real ch_new[], Integer count){
// insert new vertexes for a chainages in ch_new[]
	//get a list of existing chainages for each vertex on the element
	Integer npts;
	Get_points(e,npts);

	if(npts<1)return 1;

	Integer closed=0;
	String_closed(e,closed);
	if(closed)npts++;

	Real ch_exist[npts];

	if(closed){
		Get_end_chainage(e,ch_exist[npts]);
		npts--;
	}
	Get_chainage(e,ch_exist[1]);
	for(Integer i=2;i<=npts;i++){
		Real x,y,z;
		Get_super_vertex_coord(e,i,x,y,z);
		Real xf,yf,zf,dir,off;
		Drop_point(e,x,y,z,xf,yf,zf,ch_exist[i],dir,off);
		if(i==npts){
			Real x1,y1,z1;
			Get_super_vertex_coord(e,1,x1,y1,z1);
			if(xeqy(x,x1)){
				if(xeqy(y,y1)){
					Get_end_chainage(e,ch_exist[i]);	//could be dropping onto the start chainage
				}
			}
		}
	}
	if(closed)npts++;
	Integer last_ch_point_found = npts;  //keep a track of the last found point


	//POSSIBLE BUG - ASSUMES THE CHAINAGES (ch_new[i]) are in ascending order
	//work in reverse vertex order so that inserted points dont stuff up tracking of point numbers
	for(i=count;i>=1;i--){
		for(Integer j=last_ch_point_found;j>1;j--){	//(start at the last found chainage as everything should be in ascending order) ****note: j<npts as we use [j+1] later
								//loop thru the existing chainages to see if there is a match, only need to go so far as >
			//Print("i<"+To_text(i) +"> j<"+ To_text(j) +"> ch_new[i]"+To_text(ch_new[i],3) +"> ch_exist[j]"+To_text(ch_exist[j],3) +"> ch_exist[j-1]"+To_text(ch_exist[j-1],3) + "\n");
			Real min_diff = 0.00001;

			Real diff1 = ch_new[i] - ch_exist[j];
			if( (Absolute(diff1) < min_diff)){	//if a chainage already exists skip
				last_ch_point_found = j;
				//Print("i<"+To_text(i) +"> j<"+ To_text(j) +"> " +"Absolute(diff1) < min_diff" +"\n");
				break; //look for another unique chainage
			}

			Real diff2 = ch_new[i] - ch_exist[j-1];
			if( (Absolute(diff2) < min_diff)){	//if a chainage already exists skip
				last_ch_point_found = j-1;
				//Print("i<"+To_text(i) +"> j<"+ To_text(j) +"> " +"Absolute(diff2) < min_diff" +"\n");
				break; //look for another unique chainage
			}

			if(ch_new[i] > ch_exist[j])continue;
			//Print("i<"+To_text(i) +"> j<"+ To_text(j) +"> " +"should be trying to insert a point" +"\n");

			if( (ch_new[i] > ch_exist[j-1]) && (ch_new[i] < ch_exist[j]) ){
				last_ch_point_found = j;
				//insert a vertex
				Real x,y,z,dir;
				Get_position(e,ch_new[i],x,y,z,dir);
				Real rad;
				Get_super_segment_radius(e,j-1,rad);
				Integer major;
				Get_super_segment_major(e,j-1,major);		//THIS NEEDS SOME BEEFING UP
				Arc arc;
				Point p_centre;
				if(major){
					Segment seg;
					Get_segment (e,j-1,seg);
					Get_arc(seg,arc);
					p_centre = Get_centre(arc);
				}
				//Print("radius ");Print(rad);Print();
				Integer num = 1;
				if(closed){
					//Print("i<"+To_text(i) +"> j<"+ To_text(j) +"> " +"string is closed" +"\n");
					if(j==npts){
						Real x1,y1,z1;
						Get_super_vertex_coord(e,j-1,x1,y1,z1);	//this is the last point before the closed segment
						//Get_super_segment_radius(e,j-1,rad);	//need a different segment radius, cause j isn't synced
						Super_insert_vertex(e,j-1,num);		//insert a point before the last vertex
						Set_super_segment_radius(e,j-1,rad);
						Set_super_segment_radius(e,j,rad);
						Set_super_vertex_coord(e,j-1,x1,y1,z1);
						Set_super_vertex_coord(e,j,x,y,z);

						//Set_super_segment_radius(e,j-2,rad);
						//j++;
					}else{
						Super_insert_vertex(e,j,num);
						Set_super_vertex_coord(e,j,x,y,z);
						Set_super_segment_radius(e,j,rad);
					}
				}else{
					//Print("i<"+To_text(i) +"> j<"+ To_text(j) +"> " +"string is not closed" +"\n");
					Super_insert_vertex(e,j,num);
					Set_super_vertex_coord(e,j,x,y,z);
					Set_super_segment_radius(e,j,rad);
				}

				if(major){
					Segment seg;
					Get_segment (e,j,seg);
					Get_arc(seg,arc);
					Point p2 = Get_centre(arc);
					Get_segment(e,j-1,seg);
					Get_arc(seg,arc);
					Point p1 = Get_centre(arc);
					if(!xeqy_2d(p1,p_centre)){
						Set_super_segment_major(e,j-1,0);
					}
					if(!xeqy_2d(p2,p_centre)){
						Set_super_segment_major(e,j,0);
					}

				}

				ch_exist[j] = ch_new[i];//there is a new existing chainage for j
				break;
			}
		}
	}
	Calc_extent(e);
	return 0;
}

void set(Real my_real_array[], Integer max_pts, Real value){
	for(Integer i=1;i<=max_pts;i++){
		my_real_array[i] = value;
	}
}

Integer get_data(Element &e, Real x[], Real y[], Real z[], Integer max_pts, Integer &num_pts){
//  basically pull out the x y z from any element, element agnostic call
	Text type;
	Get_type(e,type);

	switch (type){
		case ("String_Cloud") : {
			Get_points(e,num_pts);
			if(max_pts<num_pts)num_pts = max_pts;
			for(Integer i=1;i<=max_pts;i++){
				//Get_point_cloud_coord(e,i,x[i],y[i],z[i]);
				Get_data(e,i,x[i],y[i],z[i]);
			}
		}break;
		case ("2d") : {
			Real this_z;
			Integer ierr = Get_2d_data(e,x,y,this_z,max_pts,num_pts);
			set(z,max_pts,this_z);
		} break;
		case ("3d") : {
			return Get_3d_data(e,x,y,z,max_pts,num_pts);
		} break;
		case ("4d") : {
			Text t[max_pts];
			return Get_4d_data(e,x,y,z,t,max_pts,num_pts);
		} break;
		case ("Interface") : {
			Integer f[max_pts];
			return Get_interface_data(e,x,y,z,f,max_pts,num_pts);
		} break;
		case ("Super_Alignment") : {
			Element horiz = Get_super_alignment_horizontal_string(e);
			Real r[max_pts];
			Integer f[max_pts];
			Get_super_data(e,x,y,z,r,f,max_pts,num_pts);
			for(Integer i=1;i<=max_pts;i++){
				Real xf,yf,zf,ch,dir,off;
				Drop_point(e,x[i],y[i],zf,xf,yf,z[i],ch,dir,off);
			}
		} break;
		case ("Alignment") : {
// hmmm
			Print("! I'm not currently supported :( <"+type+"> [FILE:mashy_lib_element_functions.H, CALL:get_data() LINE:"+To_text(__LINE__)+"]\n");
		} break;
		case ("Arc") : {
			Integer ok = 0;
			ok+=Get_arc_start(e,x[1],y[1],z[1]);
			num_pts = 1;
			if(max_pts>=2){
				ok+=Get_arc_start(e,x[2],y[2],z[2]);
				num_pts = 2;
			}
			return ok;
		} break;
		case ("Circle") : {
			Real r;
			num_pts = 1;
			return Get_circle_data(e,x[1],y[1],z[1],r);

		} break;
		case ("Text") : {
			num_pts = 1;
			Null(z[1]);
			return Get_text_xy(e,x[1],y[1]);
		} break;
		case ("Pipeline") : {
// hmmm
			Print("! I'm not currently supported :( <"+type+"> [FILE:mashy_lib_element_functions.H, CALL:get_data() LINE:"+To_text(__LINE__)+"]\n");
		} break;

		case ("Polyline") : {
			Real r[max_pts];
			Integer f[max_pts];
			return Get_polyline_data(e,x,y,z,r,f,max_pts,num_pts);
		} break;
		case ("Drainage") : {
			Real r[max_pts];
			Integer f[max_pts];
			return Get_drainage_data(e,x,y,z,r,f,max_pts,num_pts);
		} break;

		case ("Pipe") : {
			return Get_pipe_data(e,x,y,z,max_pts,num_pts);
		} break;
		case ("Face") : {
			return Get_face_data(e,x,y,z,max_pts,num_pts);
		} break;
		case ("Feature") : {
			num_pts = 1;
			Get_feature_centre(e,x[1],y[1],z[1]);
		} break;
		case ("Super") : {
			Real r[max_pts];
			Integer f[max_pts];
			return Get_super_data(e,x,y,z,r,f,max_pts,num_pts);
		} break;


		default : {
			Print("! internal routine does not recogise string type [cry face] <"+type+"> [FILE:mashy_lib_element_functions.H, CALL:get_data() LINE:"+To_text(__LINE__)+"]\n");
		} break;
	}
	return 1;
}

Integer get_picked_uids(Element &e, Uid &uid_m, Uid &uid_e){
	return get_id(e,uid_m,uid_e);
}

Integer find_element(Dynamic_Element &delt, Element &e_to_find, Integer &index){
	Uid uid_to_find,uid_m;
	Get_id(e_to_find,uid_to_find);
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(delt,i,e);
		Uid uid;
		Get_id(e,uid);
		if(uid == uid_to_find){
			index = i;
			return 1;
		}
	}
	return 0;
}

Integer get_next(Uid uid_m, Uid uid_e, Element &next_e){
	Model m;
	Element this_e;
	Get_model(uid_m,m);
	Get_element(uid_m,uid_e,this_e);
	Dynamic_Element delt;
	Append(m,delt);
	Integer items;
	Get_number_of_items(delt,items);
	Integer index;
	if(find_element(delt,this_e,index)){
		if(index<items){
			return Get_item(delt,index+1,next_e);
		}
	}
	return 1;
}


Integer get_previous(Uid uid_m, Uid uid_e, Element &previous_e){
	Model m;
	Element this_e;
	Get_model(uid_m,m);
	Get_element(uid_m,uid_e,this_e);
	Dynamic_Element delt;
	Append(m,delt);
	Integer items;
	Get_number_of_items(delt,items);
	Integer index;
	if(find_element(delt,this_e,index)){
		if(index>1){
			return Get_item(delt,index-1,previous_e);
		}
	}
	return 1;
}

#endif