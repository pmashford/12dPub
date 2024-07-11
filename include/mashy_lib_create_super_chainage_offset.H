#ifndef mashy_lib_create_super_chainage_offset_included
#define mashy_lib_create_super_chainage_offset_included

//get_item conflicts with Qsort.H.... changed the name of the get_item functions locale to this file

Real get_item_local(Dynamic_Real d, Integer i){
	//return the specified real from a Dynamic_Real
	Real r;
	Get_item(d,i,r);
	return r;
}

Element create_super(Dynamic_Real dx, Dynamic_Real dy, Dynamic_Real dz, Text elt_name){
	Integer no_pts;
	Get_number_of_items(dx,no_pts);
	Element elt= Create_super(0,no_pts);
	Set_name(elt,elt_name);
	Real x,y,z;
	for (Integer i=1; i<=no_pts; i++) {
		Set_super_data(elt,i,get_item_local(dx,i),get_item_local(dy,i),get_item_local(dz,i),0,0);
	}
	Calc_extent(elt);
	return elt;
}

Element create_super(Dynamic_Real dx, Dynamic_Real dy, Dynamic_Real dz){
	Element elt = create_super(dx,dy,dz,"");
	return elt;
}

Real get_angle_at_chainage(Element elt,Real ch){
	//the the instanteous direction at chainage ch
	Real x,y,z,inst_dir;
	Get_position(elt,ch,x,y,z,inst_dir);
	return inst_dir;
}

Real get_middle_chainage(Element elt){
	//get the middle chainage of an element
	Real ch,start,end;
	Get_chainage(elt,start);
	Get_end_chainage(elt,end);
	ch = (start + end) / 2;
	return ch;
}

Real get_angle(Element elt){
	//returns the instaneous angle at the middle chainage of the Element elt
	Real a = get_angle_at_chainage(elt,get_middle_chainage(elt));
	return a;
}

Integer label_chainage_offset(Element elt,Real ch,Integer ch_dec, Real off, Integer off_dec){
	//label vertex 1 with an offset
	//label vertex 2 with a chaiange

	//Set_super_use_segment_annotation_array(elt,1);
	//Set_super_use_segment_text_array(elt,1);
	Set_super_use_vertex_annotation_array(elt,1);
	Set_super_use_vertex_text_array(elt,1);

	Textstyle_Data textstyle_data;
	//Set_angle(textstyle_data,Real angle);
	Set_colour(textstyle_data,2);
	//Set_data(textstyle_data,"DATA");
	Set_justify(textstyle_data,4);
	//Set_name(textstyle_data,"NAME");
	//Set_offset(textstyle_data,0);
	Set_raise(textstyle_data,.2);
	Set_size(textstyle_data,0.7);
	//Set_slant(textstyle_data,Real slant);
	Set_textstyle(textstyle_data,"ISOCP");
	//Set_ttf_italic(textstyle_data,Integer italic);
	//Set_ttf_strikeout(textstyle_data,Integer strikeout);
	//Set_ttf_underline(textstyle_data,Integer underline);
	//Set_ttf_weight(textstyle_data,Integer weight);
	//Set_x_factor(textstyle_data,Real xfactor);

	Integer seg = 1,vert = 1;
	//Set_super_segment_textstyle_data(elt,seg,textstyle_data);
	Set_super_vertex_textstyle_data(elt,vert,textstyle_data);
	vert =2;
	Set_super_vertex_textstyle_data(elt,vert,textstyle_data);

	//Set_super_segment_text(elt,1,"TESTING");
	Set_super_vertex_text(elt,1,To_text(off,off_dec));
	Set_super_vertex_text(elt,2,To_text(ch,ch_dec));
	Set_super_vertex_text_angle(elt,1,get_angle(elt));
	Set_super_vertex_text_angle(elt,2,get_angle(elt));
	//Set_super_segment_text_angle(elt,2,get_angle(elt));

	return 0;

}

Element create_super_ch_off(Real x1, Real y1, Real x2, Real y2,Real ch, Real off){
	//returns an element from point x1y1 to x2y2 with text on vertext 1 and 2 rotated to match middle chaiange of element
	Element elt;
	Dynamic_Real dx,dy,dz;
	Append(x1,dx);Append(y1,dy);Append(0,dz);
	Append(x2,dx);Append(y2,dy);Append(0,dz);
	elt = create_super(dx,dy,dz);
	Integer ch_dec = 3, off_dec = 3;
	label_chainage_offset(elt,ch,ch_dec,off,off_dec);
	return elt;
}

#endif