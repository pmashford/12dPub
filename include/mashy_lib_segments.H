#ifndef mashy_lib_segments_h_included
#define mashy_lib_segments_h_included

#include "mashy_lib_element_functions.H"

void get_point(Point &p1, Real &x, Real &y){
	x = Get_x(p1);
	y = Get_y(p1);
}

void get_point(Point &p1, Real &x, Real &y, Real &z){
	x = Get_x(p1);
	y = Get_y(p1);
	z = Get_z(p1);
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

void get_start_check_spiral(Segment &segment, Real &x, Real &y, Real &z){
	Point p1;
	Integer segtype = Get_type(segment);
	if(segtype == 4){
		Spiral spiral;	Get_spiral(segment,spiral);
		if(!Get_leading(spiral)){
			Get_end(segment,p1);
			get_point(p1,x,y,z);
			return;
		}
	}
	Get_start(segment,p1);
	get_point(p1,x,y,z);
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

void get_end_check_spiral(Segment &segment, Real &x, Real &y, Real &z){
	Point p1;
	Integer segtype = Get_type(segment);
	if(segtype == 4){
		Spiral spiral;	Get_spiral(segment,spiral);
		if(!Get_leading(spiral)){
			Get_start(segment,p1);
			get_point(p1,x,y,z);
			return;
		}
	}
	Point point;
	Get_end(segment,point);
	get_point(point,x,y,z);
}


Integer get_segment_or_vertex(Element e, Real x, Real y, Real z, Integer &segment_or_vertex_index){
	// returns 0 for a fail/error
	// returns 1 if x,y is a vertex (or dropped to a vertex)
	// returns 2 if x,y is a segment (or dropped to a segment)
	Real xf,yf,zf,chf,dirf,off,distance;
	Segment segment;
	if( Drop_point(e,x,y,z,xf,yf,zf,chf,dirf,off,segment) || Get_position(e,chf,xf,yf,zf,dirf,segment_or_vertex_index,distance)){ // drop point gets a chainage, then get_position gets a vertex
		segment_or_vertex_index = 0;
		return 0; // error, 12d built in call(s) failed, this handles bad input of element,x,y
	}
	segment_or_vertex_index++; // convert to 1 based vertex index (see https://forums.12dmodel.com/viewtopic.php?t=16047)
	if(Xeqy(distance,0.0)){
		return(1); // dropped onto vertex (at start of segment)
	}
	Real length;
	Get_length(segment,length);
	if(Xeqy(distance,length)){
		segment_or_vertex_index++;
		return(1); // dropped onto vertex (and end of segment)
	}
	return(2); // dropped onto segment
}
Integer get_segment_or_vertex(Element e, Real x, Real y, Integer &segment_or_vertex_index){
	return get_segment_or_vertex(e, x, y, 0., segment_or_vertex_index);
}

Integer get_segment_and_vertex_ids(Element element,Real xd, Real yd, Real zd, Integer direction, Integer &segment_n, Integer &vertex_from, Integer &vertex_to){
	// returns 0 for a fail/error
	// returns 1 if x,y is a vertex (or dropped to a vertex)
	// returns 2 if x,y is a segment (or dropped to a segment)
	// direction is 1 for with chainage, -1 for reverse. (effects vertex from and to)
	Integer ierr =get_segment_or_vertex(element, xd, yd, zd,segment_n);
	if(0 == ierr){
		segment_n = 0;	vertex_from = 0;	vertex_to = 0;
		return(0); // error, no points in the element, or unhandled error
	}
	if(1 == ierr){
		vertex_from = segment_n;	vertex_to = segment_n;
		return(1); // point found (segment_n = vertex_from = vertex_to) - that is dropped point is bang on a vertex
	}
	Integer npts;
	Get_points(element,npts);
	vertex_from = segment_n;
	vertex_to = vertex_from + 1;
	if(vertex_to > npts){
		vertex_to = 1;
	}
	if(direction < 0){
		Integer temp = vertex_from;
		vertex_from = vertex_to;
		vertex_to = temp;
	}
	return(2);
}

#endif