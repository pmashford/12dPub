#ifndef mashy_lib_widgets_draw_box_H_included
#define mashy_lib_widgets_draw_box_H_included

#include "mashy_lib_element_functions.h"

Integer draw_draw_box(Draw_Box &draw_box, Dynamic_Element &delt,Integer keep_aspect, Real scale_factor){
	Real x_min, x_max, y_min, y_max;
	Integer x,y;
	Get_size(draw_box,x,y);
	get_extents(delt,x_min,x_max,y_min,y_max);
	Real dx = (x_max - x_min);
	Real dy = (y_max - y_min);

	Real x_scale = dx/x / scale_factor;
	Real y_scale = dy/y / scale_factor;

	if(keep_aspect){
		Real scale = (x_scale > y_scale) ? x_scale : y_scale;
		x_scale = scale;
		y_scale = scale;
	}

	Real origin_factor = (1.0 - scale_factor ) / 2.0;

	Set_origin(draw_box,x_min-(dx*origin_factor),y_min-(dy*origin_factor));
	Set_scale(draw_box,x_scale,y_scale);

	Start_batch_draw(draw_box);
	Clear(draw_box,0,0,0);
	Integer items;	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;	Get_item(delt,i,e);
		Draw_element(draw_box,e);
	}
	End_batch_draw(draw_box);
	return(0);
}

#ifndef DRAW_BOX_FACTOR
#define DRAW_BOX_FACTOR 0.9
#endif

Integer draw_draw_box(Draw_Box &draw_box, Dynamic_Element &delt,Integer keep_aspect){
	return(draw_draw_box(draw_box, delt,keep_aspect,DRAW_BOX_FACTOR));
}




Element create_image(Text image){
//this creates a string with an image at the 1st point, and a bounding box around the image extents (useful for displaying an image in a draw_box when you want to scale the image to fit)
	Element e = Create_super(0,4);
	Integer x,y;
	Get_image_size(image,x,y);
	Set_super_vertex_coord(e,1,0,0,0);
	Set_super_vertex_coord(e,2,0,y,0);
	Set_super_vertex_coord(e,3,x,y,0);
	Set_super_vertex_coord(e,4,x,0,0);
	Set_super_use_vertex_image_array(e,1);
	Super_vertex_add_plan_image(e,1,image,x,y,0,0,0);
	//Set_super_use_visibility(e,1);
	Set_super_use_segment_visibility_value(e,1);
	Set_super_segment_visibility(e,1,1);
	Calc_extent(e);
	return(e);
}

Integer fit_image(Draw_Box &box,Text image){
//clears a draw box and adds an imagage scaled to fit
	Integer box_x,box_y;
	Integer image_x,image_y;
	if(File_exists(image)){
		Element e = create_image(image);
		Dynamic_Element delt;Append(e,delt);
		draw_draw_box(box,delt,1,1.01);
		Element_delete(e);
	}else{
		return 1;
	}
	return 0;
}

#endif