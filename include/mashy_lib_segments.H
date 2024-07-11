#ifndef mashy_lib_segments_h_included
#define mashy_lib_segments_h_included

void get_point(Point &p1, Real &x, Real &y){
	x = Get_x(p1);
	y = Get_y(p1);
}

void get_arc_data(Arc &arc, Real &xc, Real &yc, Real &radius){
	Point p1;
	p1 = Get_centre(arc);
	get_point(p1,xc,yc);
	radius = Get_radius(arc);
}

void get_start(Segment &segment, Real &x, Real &y){
	Point p1;
	Get_start(segment,p1);
	get_point(p1,x,y);
}

void get_end(Segment &segment, Real &x, Real &y){
	Point p1;
	Get_end(segment,p1);
	get_point(p1,x,y);
}

void get_start_check_spiral(Segment &segment, Real &x, Real &y){
	Point p1;
	Integer segtype = Get_type(segment);
	if(segtype == 4){
		Spiral spiral;	Get_spiral(segment,spiral);
		if(!Get_leading(spiral)){
			Get_end(segment,p1);
			get_point(p1,x,y);
			return;
		}
	}
	Get_start(segment,p1);
	get_point(p1,x,y);
}

void get_end_check_spiral(Segment &segment, Real &x, Real &y){
	Point p1;
	Integer segtype = Get_type(segment);
	if(segtype == 4){
		Spiral spiral;	Get_spiral(segment,spiral);
		if(!Get_leading(spiral)){
			Get_start(segment,p1);
			get_point(p1,x,y);
			return;
		}
	}
	Point point;
	Get_end(segment,point);
	get_point(point,x,y);
}

#endif