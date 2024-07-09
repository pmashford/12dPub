#ifndef mashy_lib_widgets_select_button_h_included
#define mashy_lib_widgets_select_button_h_included

#include "set_ups.H"
#include "12d/standard_library.H"

void get_select_button_message(Text msg, Real &x, Real &y, Real &z, Real &ch, Real &z2, Text &view){
// process the select button message
	Dynamic_Text dt_msg;
	From_text(msg,34,32,dt_msg);
	Text t;
	Get_item(dt_msg,1,t);From_text(t,x);
	Get_item(dt_msg,2,t);From_text(t,y);
	Get_item(dt_msg,3,t);From_text(t,z);
	Get_item(dt_msg,4,t);From_text(t,ch);
	Get_item(dt_msg,5,t);From_text(t,z2);
	Text view_name;
	Get_item(dt_msg,6,view_name);
	Null(dt_msg);
}

void get_select_button_message(Text msg, Real &x, Real &y, Text &view){
// process the select button message, when we wnat less info
	Dynamic_Text dt_msg;
	From_text(msg,34,32,dt_msg);
	Text t;
	Get_item(dt_msg,1,t);From_text(t,x);
	Get_item(dt_msg,2,t);From_text(t,y);
	Get_item(dt_msg,6,view);
	Null(dt_msg);
}

void get_select_button_message(Text msg, Real &x, Real &y, View &view){
// process the select button message, when we wnat less info
	Text view_name;
	get_select_button_message(msg, x, y, view_name);
	view = Get_view(view_name);
}

Integer get_select_button_selected_vertex(Select_Button &select_button,Integer &vert){
//http://forums.12dmodel.com/viewtopic.php?t=3306
// tweaked it a little
	Element element;
	if(Validate(select_button, element) != OK){
		return(1);
	}else{
		Real  xd,yd,zd,chd,z2d;
		Get_select_coordinate(select_button,xd,yd,zd,chd,z2d);

		Integer  numpts;
		Get_points(element,numpts);
		for (Integer i=1 ; i <= numpts ; i++){
			Real xf,yf,zf;
			Get_data(element,i,xf,yf,zf);
			if (xeqy(xf,xd)){
				if (xeqy(yf,yd)){
					if (xeqy(zf,zd)){
						vert = i; return(0);
					}
				}
			}
		}
	}
	return(-1);
}

Integer get_select_button_selected_segment(Select_Button &select_button,Integer &segment, Integer &dir, Integer &vertex, Integer &vertex_from){
// vertex is the selected vertex taking into account direction
// vertex from is the previous vertex taking into account string direction
	Element element;
	if(Validate(select_button, element) != OK) {
		return(1);
	} else {
		Real  xd,yd,zd,chd,z2d;
		Get_select_coordinate(select_button,xd,yd,zd,chd,z2d);
		Get_select_direction (select_button,dir);

		Real xf,yf,zf,chf,inst_dirf,off;
		Segment seg;
		Drop_point(element,xd,yd,zd,xf,yf,zf,chf,inst_dirf,off,seg);
		Point point;
		Get_start(seg,point);
		Real xs = Get_x(point);	Real ys = Get_y(point);	Real zs = Get_z(point);

		Integer  numpts;
		Get_points(element,numpts);
		//p("numpts",numpts);
		//p("dir",dir);
		for (Integer i=1 ; i <= numpts ; i++){
			//p(i);
			Get_data(element,i,xf,yf,zf);
			if (xeqy(xf,xs)){
				if (xeqy(yf,ys)){
					if (xeqy(zf,zs)){
						if(dir != -1){ // -1 == reverse
							if(i==numpts){
								vertex = 1;// if we're at the last point then the string is closed and we actually need the 1st point
							}else{
								vertex = i+1;
							}
							vertex_from = i;
						}else{
							vertex = i;
							if(i == numpts){
								vertex_from = 1;
							}else{
								vertex_from = i + 1;
							}
							//p("start reverse dir",vertex_from);
						}
						//p("start",vertex_from);
						segment = i; return(0);
					}
				}
			}
		}
	}
	return(-1);
}



Integer select_button_get_segment_or_point(Select_Button &select_button, Integer &vertex, Integer &segment, Integer &dir, Integer &vertex_from){
// try and work out if a point or segment was selected
//  if a point was selected what point number?
//  if a segment was selected what segment number and direction?
// RETURN
// 1 = point selected
// 2 = segment selected

	if(get_select_button_selected_vertex(select_button,vertex) == 0){
		//point selected!
		dir = 0;
		segment = 0;
		vertex_from = 0;
		return 1; // 1 matches segment type point
	}else{
		if(get_select_button_selected_segment(select_button,segment, dir, vertex, vertex_from)== 0){ // pos is the segment number we are interested in
			return 2; //2 matches segment type line
		}
		return -1; // error
	}
	return -2; // really bad error!
}



#endif