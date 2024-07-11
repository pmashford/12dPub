#ifndef mashy_lib_create_super_included
#define mashy_lib_create_super_included

#include "12d/set_ups.h"
#include "mashy_lib_get_item.h"
#include "mashy_lib_geometry.h"


//--------------------------------------------------------------------------------
// Append to the end of a super element
//--------------------------------------------------------------------------------
Integer append_super_data(Element &e, Real x, Real y, Real z){
	Integer ok=0;
	Integer npts;
	ok+=Get_points(e,npts);
	Real xe,ye,ze;
	ok+=Get_super_vertex_coord(e,npts,xe,ye,ze);
	ok+=Super_insert_vertex(e,npts,1);
	ok+=Set_super_vertex_coord(e,npts,xe,ye,ze);
	ok+=Set_super_vertex_coord(e,npts+1,x,y,z);
	return ok;
}

Integer append_super_data(Element &e, Real x[], Real y[], Real z[], Integer size){
	Integer ok=0;
	Integer npts;
	ok+=Get_points(e,npts);
	Real xe,ye,ze;
	ok+=Get_super_vertex_coord(e,npts,xe,ye,ze);
	ok+=Super_insert_vertex(e,npts,size);
	ok+=Set_super_vertex_coord(e,npts,xe,ye,ze);
	Real r[size];
	Integer f[size];
	for(Integer i=1;i<=size;i++){
		r[i] = 0.0;
		f[i] = 0;
	}
	ok+=Set_super_data(e,x,y,z,r,f,size,npts+1);
	return ok;
}

//--------------------------------------------------------------------------------
// Insert along a super element
//  - this call handles appending to the end of the element if before_which_vertex > npts
//--------------------------------------------------------------------------------
Integer insert_super_data(Element &e, Integer before_which_vertex, Real x, Real y, Real z){
	Integer ok=0;
	Integer npts;
	Get_points(e,npts);
	if(before_which_vertex > npts){ // handle appending to the end of the super if before_vertex is > npts
		Real xe,ye,ze;
		ok+=Get_super_vertex_coord(e,npts,xe,ye,ze);
		ok+=Super_insert_vertex(e,npts,1);
		ok+=Set_super_vertex_coord(e,npts+1,x,y,z);
		ok+=Set_super_vertex_coord(e,npts,xe,ye,ze);
	}else{
		ok+=Super_insert_vertex(e,before_which_vertex,1);
		ok+=Set_super_vertex_coord(e,before_which_vertex,x,y,z);
	}
	return ok;
}

Integer insert_super_data(Element &e, Integer before_which_vertex, Real x[], Real y[], Real z[], Integer size){
	Real r[size];
	Integer f[size];
	for(Integer i=1;i<=size;i++){
		r[i] = 0.0;
		f[i] = 0;
	}
	Integer ok=0;
	Integer npts;
	Get_points(e,npts);
	if(before_which_vertex > npts){ // handle appending to the end of the super if before_vertex is > npts
		Real xe,ye,ze;
		ok+=Get_super_vertex_coord(e,npts,xe,ye,ze);
		ok+=Super_insert_vertex(e,npts,size);
		ok+=Set_super_data(e,x,y,z,r,f,size,npts+1);
		ok+=Set_super_vertex_coord(e,npts,xe,ye,ze);
	}else{
		ok+=Super_insert_vertex(e,before_which_vertex,size);
		ok+=Set_super_data(e,x,y,z,r,f,size,before_which_vertex);
	}
	return ok;
}

// remove 1 vertex
Integer super_remove(Element &e, Integer vertex){
	return Super_remove_vertex(e,vertex,1);
}

// create a 2pt super string
Element create_super(Real x1,Real y1, Real z1, Real x2, Real y2, Real z2){
	Element e = Create_super(String_Super_Bit(ZCoord_Array),2);
	Set_super_vertex_coord(e,1,x1,y1,z1);
	Set_super_vertex_coord(e,2,x2,y2,z2);
	Calc_extent(e);
	return e;
}

// create a 1pt super string
Element create_super(Real x1,Real y1, Real z1){
	Element e = Create_super(String_Super_Bit(ZCoord_Array),1);
	Set_super_vertex_coord(e,1,x1,y1,z1);
	Calc_extent(e);
	return e;
}


Integer set_super_enable_text(Element &elt){

	//really cant work out how to control world text
	//Set_super_vertex_world_text(elt);
	//Set_super_vertex_text_type(elt,1);
	/*
		Set_super_vertex_world_text(elt);
		for(j=1;j<=num_points;j++){
			//Set_super_vertex_text_type(elt,j,1);
			Set_super_vertex_text_size(elt,j,1.75);
		}
	*/

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

	//Integer seg = 1,vert = 1;
	//Set_super_segment_textstyle_data(elt,seg,textstyle_data);
	//Set_super_vertex_textstyle_data(elt,vert,textstyle_data);
	//vert =2;
	//Set_super_vertex_textstyle_data(elt,vert,textstyle_data);

	//Set_super_segment_text(elt,1,"TESTING");

	//Set_super_vertex_text_angle(elt,1,get_angle(elt));
	//Set_super_vertex_text_angle(elt,2,get_angle(elt));
	//Set_super_segment_text_angle(elt,2,get_angle(elt));

	return 0;

}


Element create_super(Dynamic_Real &dx, Dynamic_Real &dy, Dynamic_Real &dz){
	Integer no_pts;
	Get_number_of_items(dx,no_pts);
	Integer flags = String_Super_Bit(ZCoord_Array);
	Element elt= Create_super(flags,no_pts);
	Real x,y,z;
	for (Integer i=1; i<=no_pts; i++) {
		Set_super_data(elt,i,get_item(dx,i),get_item(dy,i),get_item(dz,i),0.0,0);
	}
	Calc_extent(elt);
	return elt;
}

Element create_super(Real x[], Real y[], Real z[], Integer npts){
	Integer flag1 = String_Super_Bit(ZCoord_Array);
	Real r[npts];
	Integer f[npts];
	for(Integer i=1;i<=npts;i++){
		r[i] = 0.0;
		f[i] = 0;
	}
	return Create_super(flag1, x, y, z, r, f, npts);
}


Element create_super(Real x[], Real y[], Integer npts){
	Real z[npts],r[npts];
	Integer f[npts];
	for(Integer i=1;i<=npts;i++){
		z[i] = 0.0;
		r[i] = 0.0;
		f[i] = 0;
	}
	Integer flag1 = 0;
	return Create_super(flag1, x, y, z, r, f, npts);
}

Element create_super_text(Real &x[], Real &y[], Real &z[], Text &t[], Integer &numpts){
	//returns an element - super string text
	Integer flags = String_Super_Bit(ZCoord_Array);
	Element elt=Create_super(flags,numpts);
	Real r[numpts];
	Integer f[numpts];
	for(Integer i=1;i<=numpts;i++){
		r[i] = 0.0;
		f[i] = 0;
	}
	Set_super_data(elt,x,y,z,r,f,numpts);

	set_super_enable_text(elt);
	Set_style(elt,"0");

	for(i = 1 ; i <= numpts ; i++){
		//Set_super_data(elt,i,x[i],y[i],z[i],0,0);
		Set_super_vertex_text(elt,i,t[i]);
	}
	Set_super_vertex_text_style(elt,1,"Arial Narrow"); // ID = 768
	Calc_extent(elt);
	return elt;
}


Element create_super_text(Dynamic_Real &dx, Dynamic_Real &dy, Dynamic_Real &dz, Dynamic_Text &dt){
	//returns an element - super string text
	Element e = create_super(dx,dy,dz);
	set_super_enable_text(e);
	Set_style(e,"0");
	Integer no_pts;
	Get_number_of_items(dx,no_pts);
	for (Integer i=1; i<=no_pts; i++) {
		Set_super_vertex_text(e,i,get_item(dt,i));
	}
	Set_super_vertex_text_style(e,1,"Arial Narrow"); // ID = 768
	Calc_extent(e);

	return e;
}
Element create_super_text(Real x, Real y, Real z, Text t){

	Dynamic_Real dx; Append(x,dx);
	Dynamic_Real dy; Append(y,dy);
	Dynamic_Real dz; Append(z,dz);
	Dynamic_Text dt; Append(t,dt);
	return create_super_text(dx, dy, dz, dt);
}

Integer create_super_circle(Element &seed_circle_element, Element &super){
	Null(super);
	Text type;
	Get_type(seed_circle_element,type);
	if(type == "Circle"){
		Real x,y,z,r;
		Get_circle_data(seed_circle_element,x,y,z,r);
		Integer size = 2;
		Real sx[size], sy[size], sz[size], sr[size];
		Integer sf[size];
		sf[1] = 1;
		sf[2] = 1;
		sr[1] = r;
		sr[2] = r;
		Integer closed = 1;
		locate_point(x, y, 0.0, r, sx[1], sy[1]);
		locate_point(x, y, Pi(), r, sx[2], sy[2]);
		sz[1] = z;
		sz[2] = z;

		Integer flags = String_Super_Bit(ZCoord_Array) | String_Super_Bit(Radius_Array);
		super=Create_super(flags,size);
		Set_super_data(super,sx,sy,sz,sr,sf,size);
		String_close(super);
		Calc_extent(super);
		//Element e;
		//Element_duplicate(super,e);
		//Model m = ;
		//Set_model(e,Get_model_create("base"));
		return 0;
	}
	return 1;
}


#endif