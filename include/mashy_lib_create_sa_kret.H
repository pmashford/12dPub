#ifndef mashy_lib_create_sa_kret_h_included
#define mashy_lib_create_sa_kret_h_included


/*
KERB RETURN DEFINITION
PART 1
computator {
  id 100
  visible false
  attributes {
    integer "OldID"   100
  }
  time_created "28-May-2010 09:55:13"
  time_updated "28-May-2010 09:55:13"
  computator_horz_segment_reference {
    valid true
    offset -3.5
    one_in false
    computator_segment_reference {
      valid true
      direction 1
      reference {
        model_name "working"
        model_id   667409
        name       "MC10"
        id         667410
      }
      chainage {
        chainage 160.63120000000001
      }
    }
  }
}

PART 2
free_arc_radius {
  id 200
  attributes {
    integer "OldID"   200
  }
  time_created "28-May-2010 09:55:13"
  time_updated "28-May-2010 09:55:13"
  r -15
}

PART 3
computator {
  id 300
  visible false
  attributes {
    integer "OldID"   300
  }
  time_created "28-May-2010 09:55:13"
  time_updated "28-May-2010 09:55:13"
  computator_horz_segment_reference {
    valid true
    offset -3.5
    one_in false
    computator_segment_reference {
      valid true
      direction 1
      reference {
        model_name "base"
        model_id   6148
        name       "MC20"
        id         668811
      }
      point {
        xcoord -105.58799999999999
        ycoord 0.248
      }
    }
  }
}
*/
/*
Number of HG Parts = 3
computator {
  id 19921400
  visible false
  attributes {
    integer "OldID"   100
  }
  time_created "15-Jul-2010 11:43:57"
  time_updated "15-Jul-2010 11:43:57"
  computator_horz_segment_reference {
    valid true
    offset -3.5
    one_in false
    computator_segment_reference {
      valid true
      direction -1
      reference {
        model_name "PRO"
        model_id   3809853
        name       "MC50"
        id         4673032
      }
      point {
        xcoord 525045.3280000001
        ycoord 6905882.2060000002
      }
    }
  }
}
free_arc_radius {
  id 19930400
  attributes {
    integer "OldID"   200
  }
  time_created "15-Jul-2010 11:43:57"
  time_updated "15-Jul-2010 11:43:57"
  r -15
}
computator {
  id 19939400
  visible false
  attributes {
    integer "OldID"   300
  }
  time_created "15-Jul-2010 11:43:57"
  time_updated "15-Jul-2010 11:43:57"
  computator_horz_segment_reference {
    valid true
    offset -3.5
    one_in false
    computator_segment_reference {
      valid true
      direction -1
      reference {
        model_name "PRO"
        model_id   3809853
        name       "MC60"
        id         4673049
      }
      point {
        xcoord 525026.54300000006
        ycoord 6905888.9770000009
      }
    }
  }
}


Number of VG Parts = 3
DEBUG : No_Type
DEBUG : 400
fixed_line_grade {
  id 400
  time_created "08-Dec-2009 00:33:18"
  time_updated "08-Dec-2009 00:33:18"
  x 0
  y 61.29701257076491
  grade -0.05454618661034
  relative_start -5
  relative_end   0
}
DEBUG : No_Type
DEBUG : 500
free_parabola_compound {
  id 500
  time_created "08-Dec-2009 00:33:18"
  time_updated "08-Dec-2009 00:33:18"
  ratio 0.5
  length 0
}
DEBUG : No_Type
DEBUG : 600
fixed_line_grade {
  id 600
  time_created "08-Dec-2009 00:33:18"
  time_updated "08-Dec-2009 00:33:18"
  x 10.02232663517328
  y 61.52249425250827
  grade -0.64349104266792
  relative_start 0
  relative_end   5
}
*/

#include "mashy_lib_geometry.H"
#include "mashy_lib_element_functions.H"
#include "mashy_lib_create_super_alignment.H"

#ifndef PARTS_DECIMALS
#define PARTS_DECIMALS 8
#endif

Integer grade_kerb_return(Element &kr, Real grade1, Real grade2, Real height1, Real height2 ){
	super_alignment_vert_clear(kr);
	Real chainages[2];
	get_chainages(kr,chainages[1],chainages[2]);
	Real relative_start = -5, relative_end = 0;
	Dynamic_Text parts;
	Append(fixed_line_grade(chainages[1],height1,grade1,relative_start,relative_end,get_next_uid()),parts);
	Append(free_parabola_compound(0.5, 0, get_next_uid()),parts);
	relative_start = 0;	relative_end = 5;
	Append(fixed_line_grade(chainages[2],height2,grade2,relative_start,relative_end,get_next_uid()),parts);

	super_alignment_vert_parts_append  (kr,parts);

	return 0;
}


Integer grade_kerb_return(Element &kr, Element &string1, Element &string2, Element &ht_str_1, Element &ht_str_2){
// drop start of kr onto string1 to get a grade
// drop end of kr onto string2 to get a grade
	Element strings[2];	strings[1] = string1;	strings[2] = string2;
	Element ht_strings[2];	ht_strings[1] = ht_str_1;	ht_strings[2] = ht_str_2;
	Real chainages[2];
	get_chainages(kr,chainages[1],chainages[2]);

	Real grade[2];
	Real height[2];
	Integer string_dir[2];

	for(Integer i = 1 ; i <= 2 ; i++){
		Real x,y,z,inst_dir;
		Get_position(kr,chainages[i],x,y,z,inst_dir);
		Real xf,yf,zf,chf,inst_dirf,off,radiusf;
		Drop_point(strings[i],x,y,0,xf,xf,height[i],chf,inst_dirf,off);
		Get_position(strings[i],chf,xf,yf,zf,inst_dirf,radiusf,grade[i]);
		grade[i] = grade[i] * 100;
		//p(To_text(inst_dir,3) + " vs " + To_text(inst_dirf,3));
		if(check_inst_dirs_within_tolerance(inst_dir,inst_dirf,Half_pi())!=0){
			grade[i] = grade[i]*(-1);
		}
		Drop_point(ht_strings[i],x,y,0,xf,xf,height[i],chf,inst_dirf,off);
	}

	return grade_kerb_return(kr,grade[1],grade[2],height[1],height[2]);
}

//
#if 1
Integer grade_kerb_return(Element &kr, Element &string1, Element &string2){
	return grade_kerb_return(kr, string1, string2, string1, string2);
}
#else
Integer grade_kerb_return(Element &kr, Element &string1, Element &string2){
// drop start of kr onto string1 to get a grade
// drop end of kr onto string2 to get a grade
	Element strings[2];	strings[1] = string1;	strings[2] = string2;
	Real chainages[2];
	get_chainages(kr,chainages[1],chainages[2]);

	Real grade[2];
	Real height[2];
	Integer string_dir[2];

	for(Integer i = 1 ; i <= 2 ; i++){
		Real x,y,z,inst_dir;
		Get_position(kr,chainages[i],x,y,z,inst_dir);
		Real xf,yf,zf,chf,inst_dirf,off,radiusf;
		Drop_point(strings[i],x,y,0.0,xf,xf,height[i],chf,inst_dirf,off);
		Get_position(strings[i],chf,xf,yf,zf,inst_dirf,radiusf,grade[i]);
		grade[i] = grade[i] * 100.0;
		//p(To_text(inst_dir,3) + " vs " + To_text(inst_dirf,3));
		if(check_inst_dirs_within_tolerance(inst_dir,inst_dirf,Half_pi())!=0){
			grade[i] = grade[i]*(-1);
		}
		//inst_dir
		//inst_dirf

	}
	return grade_kerb_return(kr,grade[1],grade[2],height[1],height[2]);
}
#endif


Element create_sa_kret(Element &ref, Element &hinge, Real ref_ch, Real radius, Real ref_offset, Integer ref_dir, Real hinge_ch, Real hinge_offset, Integer hinge_dir ){
//create a Super_Alignment kerb return
	Integer ierr;
	Text model_name, string_name;
	Integer model_id, string_id;
	get_element_names_ids(ref,model_name,model_id,string_name,string_id);
	Element elt = Create_super_alignment();
	//Element Create_super_alignment(Element seed);
	//ierr = Super_alignment_horz_part_insert(Element element,Integer position,Text part);
	//Print(To_text(ref_offset,PARTS_DECIMALS));Print(); // TEST DECIMALS
	Text part;
	part =
	 "computator { "
	+"  id 10"
	+"  visible false"
	+"  computator_horz_segment_reference {"
	+"    valid true"
	+"    offset \""+ To_text(ref_offset,PARTS_DECIMALS) +"\""
	+"    one_in false"
	+"    computator_segment_reference {"
	+"      valid true"
	+"      direction " + To_text(ref_dir)
	+"      reference {"
	+"        model_name \""+model_name+"\""
	+"        model_id   \""+To_text(model_id)+"\""
	+"        name       \""+string_name+"\""
	+"        id         \""+To_text(string_id)+"\""
	+"      }"
	+"      chainage {"
	+"        chainage " + To_text(ref_ch,PARTS_DECIMALS)
	+"      }"
	+"    }"
	+"  }"
	+"}";
	ierr = Super_alignment_horz_part_append(elt,part);
	part =
	 "free_arc_radius {"
	+"  id 20"
	//+"  attributes {"
	//+"    integer \"OldID\"   200"
	//+"  }"
	//+"  time_created \"28-May-2010 09:55:13\""
	//+"  time_updated \"28-May-2010 09:55:13\""
	+"  r " + To_text(radius,PARTS_DECIMALS)
	+"}";
	ierr = Super_alignment_horz_part_append(elt,part);

	get_element_names_ids(hinge,model_name,model_id,string_name,string_id);
	part =
	 "computator {"
	+"  id 30"
	+"  visible false"
	//+"  attributes {"
	//+"    integer \"OldID\"   300"
	//+"  }"
	//+"  time_created \"28-May-2010 09:55:13\""
	//+"  time_updated \"28-May-2010 09:55:13\""
	+"  computator_horz_segment_reference {"
	+"    valid true"
	+"    offset " + To_text(hinge_offset,PARTS_DECIMALS)
	+"    one_in false"
	+"    computator_segment_reference {"
	+"      valid true"
	+"      direction " + To_text(hinge_dir)
	+"      reference {"
	+"        model_name \"" + model_name + "\""
	+"        model_id   "+To_text(model_id)
	+"        name       \"" + string_name + "\""
	+"        id         "+To_text(string_id)
	+"      }"
	+"      chainage {"
	+"        chainage " + To_text(hinge_ch,PARTS_DECIMALS)
	+"      }"
	+"    }"
	+"  }"
	+"}";
	ierr = Super_alignment_horz_part_append(elt,part);
	ierr = Calc_super_alignment_horz(elt);
	return(elt);
}

Integer get_string_directions_for_kerb_return(Element &reference,Element &hinge, Real ref_ch, Real hinge_ch, Integer &dir1, Integer &dir2){
// give me 2 strings and a chainage on each, and I'll tell you the each string direction so that the kerb return string flows from string1 to string2
// drop the chainage onto the end points if > or <
//

	Element string[2];	string[1] = reference;	string[2] = hinge;
	Real ch[2];		ch[1]=ref_ch;		ch[2]=hinge_ch;
	Real inst_dir[2],x[2],y[2],z[2];
	Real x2[2],y2[2];
	Real x3[2],y3[2];

	for (Integer i = 1 ; i <= 2 ; i++){ //get some data from the strings (x,y,z,dir at the point of interest)
		Real start_ch, end_ch;
		Get_chainage(string[i],start_ch);
		if(start_ch > ch[i])ch[i] = start_ch;
		Get_end_chainage(string[i],end_ch);
		if(end_ch < ch[i])ch[i] = end_ch;
		Get_position(string[i],ch[i],x[i],y[i],z[i],inst_dir[i]);
		locate_point(x[i], y[i], inst_dir[i], 10000, x2[i], y2[i]); //calculate a point in the positive string direction
		locate_point(x[i], y[i], inst_dir[i], -10000, x3[i], y3[i]); //calculate a point in the negative string direction
	}

	Real xf,yf;
	Integer ierr;
	if(get_segments_intersection(x[1],y[1],x2[1],y2[1],x[2],y[2],x2[2],y2[2],xf,yf)==0){ //string 1 forward, string2 forward
		dir1 = 1;	dir2 = -1;	return 0;
	}else if(get_segments_intersection(x[1],y[1],x3[1],y3[1],x[2],y[2],x2[2],y2[2],xf,yf)==0){ //string 1 reverse, string2 forward
		dir1 = -1;	dir2 = -1;	return 0;
	}else if(get_segments_intersection(x[1],y[1],x2[1],y2[1],x[2],y[2],x3[2],y3[2],xf,yf)==0){//string 1 forward, string2 reverse
		dir1 = 1; 	dir2 = 1;	return 0;
	}else if(get_segments_intersection(x[1],y[1],x3[1],y3[1],x[2],y[2],x3[2],y3[2],xf,yf)==0){//string 1 reverse, string2 reverse
		dir1 = -1;	dir2 = 1;	return 0;
	}else{
		dir1 = 0;	dir2 = 0;	return -1;	//error
	}
	return -2; //no intersection found

}


//--------------------------------------------------------------------------------
//                        new method
//--------------------------------------------------------------------------------

Integer grade_kerb_return_no_function(Element &kerb_return, Dynamic_Element &cls, Dynamic_Element &kerb_lines){

	Text type;
	Get_type(kerb_return,type);
	Integer clean_up = 0;

	if(type!="Super_Alignment"){
		Element e;
		if(Convert(kerb_return,"Super_Alignment",e)){
			//there was an error converting to a SA!
			Print(get_full_name(kerb_return)+ " of type <" + type + "> failed to convert to a Super_Alignment, thus failed to grade kerb return!\n");
			Print(" - To fix please try and convert the string to a Super_Alignment manually and then run the option again\n");
			Show_console(1);
			return 1;
		}
		string_match(kerb_return,e);
		Element_delete(kerb_return);
		kerb_return = e;
		//Calc_extent(kerb_return);
	}

	Element centre_line_1;
	Element centre_line_2;
	Element kerb_line_1;
	Element kerb_line_2;

	Real ch_centre_line_1,ch_centre_line_2;

	Real x[2],y[3],z[2];
	if(get_end_points(kerb_return,x,y,z))return(1);

	Real off_start,off_end;

	Integer i, items;

	if( Get_number_of_items(cls,items) ) return(2);
	if(items < 1) return (3);
	for(i=1;i<=items;i++){
		Element e;
		Get_item(cls,i,e);
		Real xf,yf,zf,ch,inst_dir,off;

		Drop_point(e,x[1],y[1],z[1],xf,yf,zf,ch,inst_dir,off);
		off = Absolute(off);
		if(i==1 || off < off_start){
			centre_line_1 = e;
			ch_centre_line_1 = ch;
			off_start = off;
		}

		Drop_point(e,x[2],y[2],z[2],xf,yf,zf,ch,inst_dir,off);
		off = Absolute(off);
		if(i==1 || off < off_end){
			centre_line_2 = e;
			ch_centre_line_2 = ch;
			off_end = off;
		}
	}

	if( Get_number_of_items(kerb_lines,items) ) return(12);
	if(items < 1) return (13);
	for(i=1;i<=items;i++){
		Element e;
		Get_item(kerb_lines,i,e);
		Real xf,yf,zf,ch,inst_dir,off;

		Drop_point(e,x[1],y[1],z[1],xf,yf,zf,ch,inst_dir,off);
		off = Absolute(off);
		if(i==1 || off < off_start){
			kerb_line_1 = e;
			off_start = off;
		}

		Drop_point(e,x[2],y[2],z[2],xf,yf,zf,ch,inst_dir,off);
		off = Absolute(off);
		if(i==1 || off < off_end){
			kerb_line_2 = e;
			off_end = off;
		}
	}
	//get to here we've got cl strings and height strings
	//Integer dir_centre_line_1,dir_centre_line_2;
	//get_string_directions_for_kerb_return(centre_line_1,centre_line_2,ch_centre_line_1,ch_centre_line_2,dir_centre_line_1,dir_centre_line_2);

	Integer ok = grade_kerb_return(kerb_return, centre_line_1, centre_line_2, kerb_line_1, kerb_line_2);
	Print("completed!\n");

	return(ok);
}

Integer grade_kerb_return_no_function(Dynamic_Element &kerb_returns, Dynamic_Element &cls, Dynamic_Element &kerb_lines){
	Integer ok=0;
	Integer items;
	Get_number_of_items(kerb_returns,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(kerb_returns,i,e);
		ok+=grade_kerb_return_no_function(e,cls,kerb_lines);
		//Print("ierr=");Print(ok);Print();
	}

	return ok;
}

#endif

