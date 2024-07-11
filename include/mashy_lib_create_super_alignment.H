#ifndef mashy_lib_create_super_alignment_h_included
#define mashy_lib_create_super_alignment_h_included

#include "mashy_lib_element_functions.H"

#ifndef PARTS_DECIMALS
#define PARTS_DECIMALS 8
#endif

Text convert_which_part(Integer which_part){
// 0 means previous part, any other value means next part
// return the text string
	Text attach_to;
	if(which_part != 0){
		attach_to = "next_part";
	}else{
		attach_to = "previous_part";
	}
	return attach_to;
}

Text convert_sa_command_true_or_false(Integer true_false){
// 0 false, 1 = true
	Text value;
	if(true_false == 0){
		value = "false";
	}else{
		value = "true";
	}
	return value;
}

Integer super_alignment_horz_parts_append(Element &element, Dynamic_Text &parts){
	Integer items;
	Get_number_of_items(parts,items);
	for(Integer i = 1 ; i <= items ; i++){
		Text part;
		Get_item(parts,i,part);
		Super_alignment_horz_part_append(element,part);
	}
	Calc_super_alignment_horz(element);
	return 0;
}

Integer super_alignment_vert_parts_append(Element &element, Dynamic_Text &parts){
	Integer items;
	Get_number_of_items(parts,items);
	for(Integer i = 1 ; i <= items ; i++){
		Text part;
		Get_item(parts,i,part);
		Super_alignment_vert_part_append(element,part);
	}
	Calc_super_alignment_vert(element);
	return 0;
}

Integer super_alignment_horz_clear(Element &element){
// clear the vg - should check for locks?
	Integer num_parts;
	Get_super_alignment_horz_parts(element,num_parts);
	for(Integer i = num_parts ; i >= 1 ; i--){
		Super_alignment_horz_part_delete(element,i);
	}
	return 0;
}

Integer super_alignment_vert_clear(Element &element){
// clear the vg - should check for locks?
	Integer num_parts;
	Get_super_alignment_vert_parts(element,num_parts);
	for(Integer i = num_parts ; i >= 1 ; i--){
		Super_alignment_vert_part_delete(element,i);
	}
	return 0;
}

/// HORIZONTAL

Text sa_fixed_line(Real &x1, Real &y1, Real &x2, Real &y2, Integer id){
	Text part =
	"fixed_line { "
	+ " id " + To_text(id)
	+ " x1 " + To_text(x1,PARTS_DECIMALS)
	+ " y1 " + To_text(y1,PARTS_DECIMALS)
	+ " x2 " + To_text(x2,PARTS_DECIMALS)
	+ " y2 " + To_text(y2,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_floating_line_point(Real &x1, Real &y1, Integer &which_part, Integer id){
	Text part =
	"floating_line_point { "
	+ " id " + To_text(id)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " attach_to " + convert_which_part(which_part)
	+ " }";
	return part;
}

Text sa_fixed_arc_3_points(Real &x1, Real &y1, Real &x2, Real &y2, Real &x3, Real &y3, Integer id){
	Text part =
	"fixed_arc_3_points { "
	+ " id " + To_text(id)
	+ " x1 " + To_text(x1,PARTS_DECIMALS)
	+ " y1 " + To_text(y1,PARTS_DECIMALS)
	+ " x2 " + To_text(x2,PARTS_DECIMALS)
	+ " y2 " + To_text(y2,PARTS_DECIMALS)
	+ " x3 " + To_text(x3,PARTS_DECIMALS)
	+ " y3 " + To_text(y3,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_fixed_arc(Real &x1, Real &y1, Real &x2, Real &y2, Real &xc, Real &yc, Real &radius, Integer id){
	Text part =
	"fixed_arc { "
	+ " id " + To_text(id)
	+ " xc " + To_text(xc,PARTS_DECIMALS)
	+ " yc " + To_text(yc,PARTS_DECIMALS)
	+ " r "  + To_text(radius,PARTS_DECIMALS)
	+ " x1 " + To_text(x1,PARTS_DECIMALS)
	+ " y1 " + To_text(y1,PARTS_DECIMALS)
	+ " x2 " + To_text(x2,PARTS_DECIMALS)
	+ " y2 " + To_text(y2,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_floating_arc_radius(Real &x1, Real &y1, Real &radius, Integer &which_part, Integer id){
	Text part =
	"floating_arc_radius { "
	+ " id " + To_text(id)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " r " + To_text(radius,PARTS_DECIMALS)
	+ " attach_to " + convert_which_part(which_part)
	+ " }";
	return part;
}

Text sa_floating_arc_radius_alternative(Real &x1, Real &y1, Real &radius, Integer &which_part, Integer &alterative_solution, Integer id){
	Text part =
	"floating_arc_radius { "
	+ " id " + To_text(id)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " r " + To_text(radius,PARTS_DECIMALS)
	+ " attach_to " + convert_which_part(which_part)
	+ " alternative " +convert_sa_command_true_or_false(alterative_solution)
	+ " }";
	return part;
}

Text sa_floating_arc_end_and_point(Real &x1, Real &y1, Integer &which_part, Integer id){
	Text part =
	"floating_arc_end_and_point { "
	+ " id " + To_text(id)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " attach_to " + convert_which_part(which_part)
	+ " }";
	return part;
}

Text sa_free_arc_radius (Real &radius, Integer id){
	Text part =
	"free_arc_radius { "
	+ " id " + To_text(id)
	+ " r " + To_text(radius,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_computator_horz_string_reference(Element &reference, Integer &direction, Real &offset){
	Text model_name[3], string_name[3];
	Integer model_id[3], string_id[3];
	get_element_names_ids(reference,model_name[1],model_id[1],string_name[1],string_id[1]);

	Text part =
	"computator {"
	+"  id 100"
	//time_created not required
	//time_updated not required
	+"  computator_horz_string_reference {"
	+"    valid true"
	+"    offset "+To_text(offset,PARTS_DECIMALS)
	+"    one_in false"
	+"    direction " + To_text(direction)
	+"    reference { "
	+"      model_name \""+model_name[1]+"\""
	+"      model_id   " + To_text(model_id[1])
	+"      name       \""+string_name[1]+"\""
	+"      id         " + To_text(string_id[1])
	+"    }"
	+"  }"
	+"}";
	return part;

}

Text sa_computator_vert_string_reference_ex(Element &reference, Integer &direction){
	Text model_name[1], string_name[1];
	Integer model_id[1], string_id[1];
	get_element_names_ids(reference,model_name[1],model_id[1],string_name[1],string_id[1]);
	Text part =
	"computator {"
	+"  id 200"
	+"  computator_vert_string_reference_ex {"
	+"    valid true"
	+"    chainage_direction " + To_text(direction)
	+"    reference { "
	+"      model_name \""+model_name[1]+"\""
	+"      model_id   " + To_text(model_id[1])
	+"      name       \""+string_name[1]+"\""
	+"      id         " + To_text(string_id[1])
	+"    }"
	+"    start_ch {"
	+"      computator_start_horz_chainage {"
	+"        valid true"
	+"        start_hg {"
	+"        }"
	+"      }"
	+"    }"
	+"    end_ch {"
	+"      computator_end_horz_chainage {"
	+"        valid true"
	+"        end_hg {"
	+"        }"
	+"      }"
	+"    }"
	+"  }"
	+"}";
	return part;
}



Text sa_computator_horz_string_reference_ex(Element &reference, Integer &direction, Real &offset, Element &start_element, Real &start_ch, Element &end_element, Real &end_ch){
	Text model_name[3], string_name[3];
	Integer model_id[3], string_id[3];
	get_element_names_ids(reference,model_name[1],model_id[1],string_name[1],string_id[1]);
	get_element_names_ids(start_element,model_name[2],model_id[2],string_name[2],string_id[2]);
	get_element_names_ids(end_element,model_name[3],model_id[3],string_name[3],string_id[3]);

	Text part =
	"computator {"
	+"  id 100"
	+"  computator_horz_string_reference_ex {"
	+"    valid true"
	+"    offset " + To_text(offset,PARTS_DECIMALS)
	+"    one_in false"
	+"    direction " + To_text(direction)
	+"    reference { "
	+"      model_name \""+model_name[1]+"\""
	+"      model_id   " + To_text(model_id[1])
	+"      name       \""+string_name[1]+"\""
	+"      id         " + To_text(string_id[1])
	+"    }"
	+"  start {"
	+"    computator_horz_point_reference {"
	+"      valid true"
	+"      one_in false"
	+"      computator_horz_reference {"
	+"        valid true"
	+"        direction " + To_text(direction)
	+"        reference {"
	+"          model_name \""+model_name[2]+"\""
	+"          model_id   " + To_text(model_id[2])
	+"          name       \""+string_name[2]+"\""
	+"          id         " + To_text(string_id[2])
	+"        }"
	+"        chainage {"
	+"          chainage " + To_text(start_ch,PARTS_DECIMALS)
	+"        }"
	+"      }"
	+"    }"
	+"  }"
	+"  end {"
	+"    computator_horz_point_reference {"
	+"      valid true"
	+"      one_in false"
	+"      computator_horz_reference {"
	+"        valid true"
	+"        direction  "+ To_text(direction)
	+"        reference {"
	+"          model_name \""+model_name[3]+"\""
	+"          model_id   " + To_text(model_id[3])
	+"          name       \""+string_name[3]+"\""
	+"          id         " + To_text(string_id[3])
	+"        }"
	+"        chainage {"
	+"          chainage " + To_text(end_ch,PARTS_DECIMALS)
	+"        }"
	+"      }"
	+"    }"
	+"  }"
	+"  }"
	+"}";
	return part;
}

/// BOTH HG & VG

Text sa_ip(Real &x1, Real &y1, Integer id){
	Text part =
	"ip { "
	+ " id " + To_text(id)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " }";
	return part;
}

/// VERTICAL

Text sa_radius(Real &x1, Real &y1, Real &radius, Integer id){
	Text part =
	"radius { "
	+ " id " + To_text(id)
	+ " r " + To_text(radius,PARTS_DECIMALS)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_length(Real &x1, Real &y1, Real &length, Integer id){
	Text part =
	"length { "
	+ " id " + To_text(id)
	+ " l " + To_text(length,PARTS_DECIMALS)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_asymmetric(Real &x1, Real &y1, Real &length1, Real &length2, Integer id){
 // this is circular, not parabolic
	Text part =
	"asymmetric { "
	+ " id " + To_text(id)
	+ " l1 " + To_text(length1,PARTS_DECIMALS)
	+ " l2 " + To_text(length2,PARTS_DECIMALS)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_arc(Real &x1, Real &y1, Real &radius, Integer id){
 // this is circular, not parabolic
	Text part =
	"arc { "
	+ " id " + To_text(id)
	+ " r " + To_text(radius,PARTS_DECIMALS)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_max_length(Real &x1, Real &y1, Integer id){
	Text part =
	"max_length { "
	+ " id " + To_text(id)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text sa_kvalue(Real &x1, Real &y1, Real &k, Integer id){
	Text part =
	"kvalue { "
	+ "\n id " + To_text(id)
	+ "\n k " + To_text(k,PARTS_DECIMALS)
	+ "\n x " + To_text(x1,PARTS_DECIMALS)
	+ "\n y " + To_text(y1,PARTS_DECIMALS)
	+ "\n }";
	return part;
}


Text sa_speed(Real &x1, Real &y1, Real &s, Integer id){
	Text part =
	"speed { "
	+ " id " + To_text(id)
	+ " s " + To_text(s,PARTS_DECIMALS)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text fixed_line(Real &x1, Real &y1, Real &x2, Real &y2, Integer id){
	Text part =
	"fixed_line_grade { "
	+ " id " + To_text(id)
	+ " x1 " + To_text(x1,PARTS_DECIMALS)
	+ " y1 " + To_text(y1,PARTS_DECIMALS)
	+ " x2 " + To_text(x2,PARTS_DECIMALS)
	+ " y2 " + To_text(y2,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text fixed_line_grade(Real &x1, Real &y1, Real &grade, Real &relative_start, Real &relative_end, Integer id){
	Text part =
	"fixed_line_grade { "
	+ " id " + To_text(id)
	+ " x " + To_text(x1,PARTS_DECIMALS)
	+ " y " + To_text(y1,PARTS_DECIMALS)
	+ " grade " + To_text(grade,PARTS_DECIMALS)
	+ " relative_start " + To_text(relative_start,PARTS_DECIMALS)
	+ " relative_end " + To_text(relative_end,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text free_parabola_compound(Real ratio, Real length, Integer id){
	Text part =
	"free_parabola_compound { "
	+ " id " + To_text(id)
	+ " ratio " + To_text(ratio,PARTS_DECIMALS)
	+ " length " + To_text(length,PARTS_DECIMALS)
	+ " }";
	return part;
}

Text free_parabola_length(Real &length, Integer id){
	Text part =
	"free_parabola_length { "
	+ " id " + To_text(id)
	+ " l " + To_text(length,PARTS_DECIMALS)
	+ " }";
	return part;
}

#endif

