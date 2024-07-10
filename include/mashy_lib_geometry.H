#ifndef mashy_lib_geometry_h_included
#define mashy_lib_geometry_h_included

#include "12d/standard_library.H"
#include "mashy_lib_geometry_vectors.H"

// http://forums.12dmodel.com/viewtopic.php?t=7610
// chord to arc tolerance is the maximum deviation from the arc when chorded
Real get_chord_length(Real radius, Real tolerance){
	return ( 2.0 * Sqrt( 2.0 * radius * tolerance - tolerance * tolerance ) );
}

//https://math.stackexchange.com/questions/1990698/intersection-of-two-lines-each-defined-by-a-point-and-an-angle
//Integer find_intersect_degrees(Real x1, Real y1, Real degrees1, Real x2, Real y2, Real degrees2, Real &xf, Real &yf) {
Integer get_intersection_degrees(Real x1, Real y1, Real degrees1, Real x2, Real y2, Real degrees2, Real &xf, Real &yf) {
// uses degrees (anti-clockwise, 0 degrees == +ve x-axis)
// retuns
// 0 intersection point found
// -3 lines are paralell (..or colinear)
    Real toRadians = Pi() / 180.0;
    Null(xf);
    Null(yf);
#if VERSION_4D < 1500
    if ( xeqy( Mod(  Mod((degrees1-degrees2),180.0)  + 180.0 , 180.0) , 0.0 )) return (-3); // parallel!
    if ( xeqy( Mod( (Mod(degrees1,180.0) + 180.0) , 180.0) , 90.0)) { // vertical at x1?
#else
    if ( Xeqy( Mod(  Mod((degrees1-degrees2),180.0)  + 180.0 , 180.0) , 0.0 )) return (-3); // parallel!
    if ( Xeqy( Mod( (Mod(degrees1,180.0) + 180.0) , 180.0) , 90.0)) { // vertical at x1?
#endif
        xf = x1;
        yf = Tan(degrees2*toRadians) * (x2-x1) + y2;
        return (0);
#if VERSION_4D < 1500
    }else if (  xeqy( Mod(Mod(degrees2,180.0) + 180.0 , 180.0) , 90.0)) { // vertical at x2?
#else
    }else if (  Xeqy( Mod(Mod(degrees2,180.0) + 180.0 , 180.0) , 90.0)) { // vertical at x2?
#endif
        xf = x2;
        yf = Tan(degrees1*toRadians)*(x2-x1) + y1;
        return (0);
    }
    Real m1 = Tan(degrees1*toRadians);
    Real m2 = Tan(degrees2*toRadians);
    xf = ((m1 * x1 - m2 * x2) - (y1 - y2)) / (m1 - m2);
    yf = m1 * (xf - x1) + y1;
    return (0);
}

Integer is_zero_fuzzy(Real x){
		Real mindif = 1.0e-6;
		if(Absolute(x) < mindif) return(1);
		return(0);
}

Integer get_intersection_radians(Real x1, Real y1, Real radians1, Real x2, Real y2, Real radians2, Real &xf, Real &yf) {
// uses radians (anti-clockwise, 0 radians == +ve x-axis)
// retuns
// 0 intersection point found
// -3 lines are paralell (..or colinear)
    Real toRadians = Pi() / 180.0;
    Null(xf);
    Null(yf);
#if VERSION_4D < 1500
    if ( xeqy( Mod(  Mod((radians1-radians2),Pi())  + Pi() , Pi()) , 0.0 )) return (-3); // parallel!
    if ( xeqy( Mod( (Mod(radians1,Pi()) + Pi()) , Pi()) , Half_pi())) { // vertical at x1?
#else
    if ( Xeqy( Mod(  Mod((radians1-radians2),Pi())  + Pi() , Pi()) , 0.0 )) return (-3); // parallel!
    if ( Xeqy( Mod( (Mod(radians1,Pi()) + Pi()) , Pi()) , Half_pi())) { // vertical at x1?
#endif
        xf = x1;
        yf = Tan(radians2) * (x2-x1) + y2;
        return (0);
#if VERSION_4D < 1500
    }else if (  xeqy( Mod(Mod(radians2,Pi()) + Pi() , Pi()) , Half_pi())) { // vertical at x2?
#else
    }else if (  Xeqy( Mod(Mod(radians2,Pi()) + Pi() , Pi()) , Half_pi())) { // vertical at x2?
#endif
        xf = x2;
        yf = Tan(radians1)*(x2-x1) + y1;
        return (0);
    }
    Real m1 = Tan(radians1);
    Real m2 = Tan(radians2);
    xf = ((m1 * x1 - m2 * x2) - (y1 - y2)) / (m1 - m2);
    yf = m1 * (xf - x1) + y1;
    return (0);
}
/*
Integer is_zero_fuzzy(Real x){
		Real mindif = 1.0e-6;
		if(Absolute(x) < mindif) return(1);
		return(0);
}
*/

#if 1
// find the intersection point... based on http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/linesegments.lisp
Integer get_segments_intersection(Real x1,Real y1,Real x2,Real y2,Real x3,Real y3,Real x4,Real y4,Real &xf,Real &yf){
// retuns
// 0 intersection point found
// 1 extended intersection point found
// -1 no intersection
// -2 lines are co-incidental
// -3 lines are paralell
	Real denom  = ( (y4 - y3) * (x2 - x1) ) - ( (x4 - x3) * (y2 - y1) );
	Real ua_num = ( (x4 - x3) * (y1 - y3) ) - ( (y4 - y3) * (x1 - x3) );
	Real ub_num = ( (x2 - x1) * (y1 - y3) ) - ( (y2 - y1) * (x1 - x3) );
	Real mindif = 1.0e-6;
	if(Absolute(denom) < mindif){
		Null(xf);Null(yf);
		if(Absolute(ua_num) < mindif && Absolute(ub_num) < mindif){
			return (-2);	//lines are co-incidental
		}
		return (-3);		//lines are paralell
	} else {
		Real ua = ua_num / denom;
		Real ub = ub_num / denom;

		xf= x1 + (ua * ( x2 - x1));
		yf= y1 + (ua * ( y2 - y1));
		if(ua > 0.0 && ua <=1.0 && ub > 0.0 && ub <=1.0){
			return (0);	// lines intersect
		} else return (1);	// lines intersect if extended
	}
	Null(xf);Null(yf);
	return (-1);			// unknown error
}
#else
// this has no tolerance checking!
// find the intersection point... based on http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/linesegments.lisp
Integer get_segments_intersection(Real x1,Real y1,Real x2,Real y2,Real x3,Real y3,Real x4,Real y4,Real &xf,Real &yf){
// retuns
// 0 point found
// 1 extended point found
// -1 no intersection
// -2 lines are co-incidental
// -3 lines are paralell
	Real denom  = ( (y4 - y3) * (x2 - x1) ) - ( (x4 - x3) * (y2 - y1) );
	Real ua_num = ( (x4 - x3) * (y1 - y3) ) - ( (y4 - y3) * (x1 - x3) );
	Real ub_num = ( (x2 - x1) * (y1 - y3) ) - ( (y2 - y1) * (x1 - x3) );
	Real mindif = 1.0e-6;
	if(denom == 0.0 ){
		Null(xf);Null(yf);
		if(ua_num == 0.0 && ub_num == 0.0){
			return -2;	//lines are co-incidental
		}
		return -3;		//lines are paralell
	} else {
		Real ua = ua_num / denom;
		Real ub = ub_num / denom;

		xf= x1 + (ua * ( x2 - x1));
		yf= y1 + (ua * ( y2 - y1));
		if(ua > 0.0 && ua <=1.0 && ub > 0.0 && ub <=1.0){
			return 0;	// lines intersect
		} else return 1;	// lines intersect if extended
	}
	Null(xf);Null(yf);
	return -1;			// unknown error
}
#endif

Integer get_grades_intersect(Real offset1, Real z1, Real grade1, Real offset2, Real z2, Real grade2, Real &offset, Real &z){
// calculate the intersection of 2 grades
// from point1 with a grade
// and point2 with a grade
// same returns as get_segments_intersection
	Integer ierr = get_segments_intersection(offset1,z1,offset1+100,z1+(100*grade1),offset2,z2,offset2+100,z2+(100*grade2),offset,z);
	return ierr;
}

//NOTE : THIS HAS NOT REALLY BEEN TESTED, BUT _SHOULD_ BE OK
Integer get_intersection(Real x[], Real y[], Integer points, Real x3, Real y3, Real x4, Real y4, Real &xf, Real &yf){
//returns 0 for success or -1 for failure
//used find the intersection of a string (x[],y[] points) with a line (x3,y3,x4,y4)
//the line MUST pass through the string... no intersect extend is used
//will return the first intersection only
	Real return_x, return_y;
	for(Integer i=1;i<points;i++){ //note: i<points
		if(!get_segments_intersection(x[i],y[i],x[i+1],y[i+1],x3,y3,x4,y4,return_x,return_y)){ //if intersection found
			xf = return_x;
			yf = return_y;
			return(0);
		}
	}
	return(-1);//no intersection found!

}

Real get_distance(Real &x1, Real &y1, Real &x2, Real &y2){
	return Sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

Real get_grade(Real length, Real z1, Real z2){
	return ((z2 - z1) / length);
}

Point get_mid_point(Point p1, Point p2){
	Point point;
	Set_x(point,((Get_x(p2) + Get_x(p1) ) / 2 ));
	Set_y(point,((Get_y(p2) + Get_y(p1) ) / 2 ));
	Set_z(point,((Get_z(p2) + Get_z(p1) ) / 2 ));
	return point;
}

Integer get_mid_point(Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Real &x, Real &y, Real &z){
	x = (x2 + x1) / 2.0;
	y = (y2 + y1) / 2.0;
	z = (z2 + z1) / 2.0;
	return 0;
}

#if 1
// http://stackoverflow.com/questions/2187657/calculate-second-point-knowing-the-starting-point-and-distance
// have pretty well tested this, works well :)
Integer locate_point(Real x1, Real y1, Real dir, Real dist, Real &x2, Real &y2){
	x2 = x1 + dist*Cos(dir); // dir measured counterclockwise from due east
	y2 = y1 + dist*Sin(dir);
	return 1;
}
#else
Integer locate_point(Real x1, Real y1, Real dir, Real dist, Real &x2, Real &y2){
	Point from,to;
	Set_x(from,x1);	Set_y(from,y1);	Set_z(from,0);
	Integer ierr = Locate_point(from,dir,dist,to);
	x2 = Get_x(to);	y2 = Get_y(to);
	return ierr;
}
#endif
Integer locate_point(Real x1, Real y1, Real dir, Real dist, Real offset, Real &x2, Real &y2){
// a twist on the locate point, this will do an offset
	Point from,to;
	Set_x(from,x1);	Set_y(from,y1);	Set_z(from,0);
	Integer ierr = Locate_point(from,dir,dist,to);
	from = to;
	ierr+= Locate_point(from,dir-Half_pi(),offset,to);
	x2 = Get_x(to);	y2 = Get_y(to);
	return ierr;
}

Integer locate_point(Real x1, Real y1, Real dir1, Real dist1, Real dir2, Real dist2, Real &x2, Real &y2){
// a twist on the locate point, this will locate a point based on 2 lots of dir & dist
	Point from,to;
	Set_x(from,x1);	Set_y(from,y1);	Set_z(from,0);
	Integer ierr = Locate_point(from,dir1,dist1,to);
	from = to;
	ierr+= Locate_point(from,dir2,dist2,to);
	x2 = Get_x(to);	y2 = Get_y(to);
	return ierr;
}


Integer get_dir_intersect(Real x1, Real y1, Real dir1, Real x2, Real y2, Real dir2, Real &xf, Real &yf){
	Real x3,y3,x4,y4;
	locate_point(x1,y1,dir1,9999999.9,x3,y3);
	locate_point(x2,y2,dir2,9999999.9,x4,y4);
	return(get_segments_intersection(x1,y1,x3,y3,x2,y2,x4,y4,xf,yf));
}

#if 1
// http://forums.12dmodel.com/viewtopic.php?t=5045
/*
	TESTING: dx <0.000> dy <0.000>
	TESTING: angle = 0.000

	TESTING: dx <10.000> dy <0.000>
	TESTING: angle = 0.000

	TESTING: dx <0.000> dy <10.000>
	TESTING: angle = 1.571

	TESTING: dx <-10.000> dy <0.000>
	TESTING: angle = 3.142

	TESTING: dx <0.000> dy <-10.000>
	TESTING: angle = -1.571
*/
Real get_direction(Real x1, Real y1, Real x2, Real y2){
	return Atan2(y2-y1,x2-x1);
}
#else
Real get_direction(Real x1,Real y1,Real x2,Real y2){ // this is measured from the horizontal axis
// returns radians
	Real x3,y3;
	locate_point( x1, y1, 0, 10, x3, y3);
	return change_of_angle(x3, y3, x1, y1, x2, y2); // vector 1 is p1 => p2, vector 2 is p2 => p3

}
#endif

Real get_bearing(Real x1,Real y1,Real x2,Real y2){ // this is measured from north clockwise
// returns radians
	//Real dx = x2 - x1;
	//Real dy = y2 - y1;
	//Real bearing = Half_pi()-Mod( Atan2(dy,dx),Pi()*2) ;
	return( Half_pi()-Mod(Atan2(y2-y1,x2-x1),Pi()*2) );
}

Real radians_to_slope_percent(Real rad){
	Real deg;
	Radians_to_degrees(rad,deg );
	return Tan( deg );
}

Real radians_to_degrees(Real rad){
	Real deg;
	Radians_to_degrees(rad,deg);
	return deg;
}


Real radians_to_radians(Real rad){
// convert radians to a positive value starting from 0 to 4*Pi()
	// make sure radians are not greater than 2*Pi()
	rad = Mod(rad,Pi()*2.0);
	if(rad < 0.0){
		rad = rad + (Pi() * 2.0);
	}
	return rad;
}

Real change_of_radians(Real dir1, Real dir2){
// calculate the diference between 2 directions
	dir1 = radians_to_radians(dir1);
	dir2 = radians_to_radians(dir2);
	Real delta = Mod(dir2 - dir1,Pi()*2.0);// make sure the change of angle is not greater than
	return delta;
}

Real sweep_in_radians(Real xc, Real yc, Real xs, Real ys, Real xe, Real ye,Integer dir){
// calculate the sweep angle in radians for an arc
// dir is +ve = clockwise -ve = anticlockwise (be careful of passing a real because 0 to -0.5 wont get converted properly to an int)
// THIS ROUTINE IS CRAP!
// radians = arclength / radius
// may need to Absolute it
	//p("SWEEP IN RADIANS >>>");
	Real start_dir = radians_to_radians(get_direction(xc,yc,xs,ys));
	Real end_dir   = radians_to_radians(get_direction(xc,yc,xe,ye));
	Real sweep = Absolute(change_of_radians(start_dir, end_dir));
	//p("sweep",sweep);
	//p("dir",dir);
	if(dir >= 0.0 && start_dir < end_dir){
	//p(1);
		sweep+=Pi();
	//}else if (dir < 0 && start_dir > end_dir ){
	}else if (dir < 0.0 && start_dir > end_dir && start_dir < Pi()){
	//p(2);
		sweep+=Pi();
	}
	//p(start_dir);p(end_dir);p(sweep);
	//p("<<< SWEEP IN RADIANS");
	return sweep;
}

Real radians_to_bearing(Real inst_dir){
// bearing is measured from north in a clockwise direction
        Real bearing;
        Radians_to_degrees(inst_dir,bearing);
        if(bearing <= 90.0 && bearing >= 0.0){
		bearing -= 90.0;
        } else if(bearing == 360.0){
		bearing -= 360.0;
        } else {
		bearing -= 450.0;
        }
        return bearing;
}


Integer check_inst_dirs_within_tolerance(Real dir1, Real dir2, Real tolerance){
// remember! Half_pi() = 90 degrees, Pi() = 180 degress
	Real delta = change_of_radians(dir1,dir2);
	delta = Absolute(delta);
	//p("delta",delta);
	if(delta <= tolerance){
		//p("within");
		return 0;
	}
	//p("outside");
	return -1;
}

Integer drop_point_onto_segment_extend(Real &x1, Real &y1, Real &z1, Real &x2, Real &y2, Real &z2, Real &px, Real &py, Real &dropped_z, Real &distance){

	Real dx = x2 - x1;
	Real dy = y2 - y1;
	Real dz = z2 - z1;

	Real length = Sqrt(dx * dx + dy * dy);

	Real u =	( ( ( px - x1 ) * dx ) +
			( ( py - y1 ) * dy ) )/
			( length * length );
	Real dropped_x =  ( x1 + u * dx );
	Real dropped_y =  ( y1 + u * dy );
	dropped_z =  ( z1 + u * dz );
	distance =  Sqrt((dropped_x - px) * (dropped_x - px) + (dropped_y - py) * (dropped_y - py));
	return 0;
}


Integer drop_point_onto_segment(Real x3, Real y3, Real x1, Real y1, Real x2, Real y2, Real &xf,Real &yf){
// p1 & p2 are the segment, p3 is the point to drop, pf is the result, based on http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/ (java example)
// return
// 0 = all good
// 1 = points are all co-incidental
// 2 = dropped point is before start
// 3 = dropped point is after start
// -1= bad
// -2=points are co-incidental, ie a segemnt was not defined
	Real xDelta = x2 - x1;
	Real yDelta = y2 - y1;
	if(xDelta == 0 && yDelta == 0){
		if(x3 == x1 && y3 == y1){
			xf = x1; yf = y1;
			return 1;	// points are all equal, kind of get a result
		}
		return -2; // points are not allowed to be co-incidental
	}
	Real u = ( ( x3 - x1 ) * xDelta + ( y3 - y1 ) * yDelta ) / (xDelta * xDelta + yDelta * yDelta);

	if (u < 0 ){
		xf = x1; yf = y1;
		return 2;		// droped point is before start of segment
	} else if (u > 1 ){
		xf = x2; yf = y2;
		return 3;		// droped point is after end of segment
	} else {
		xf = x1 + ( u * xDelta );
		yf = y1 + ( u * yDelta );
		return 0;
	}
	Null(xf);Null(yf);
	return -1;
}

Integer drop_point_onto_segment_extend(Real x3, Real y3, Real x1, Real y1, Real x2, Real y2, Real &xf,Real &yf){
// p1 & p2 are the segment, p3 is the point to drop, pf is the result, based on http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/ (java example)
// return
// 0 = all good
// 1 = points are all co-incidental
// 2 = dropped point is before start
// 3 = dropped point is after start
// -1= bad
// -2=points are co-incidental, ie a segemnt was not defined
	Real xDelta = x2 - x1;
	Real yDelta = y2 - y1;
	if(xDelta == 0 && yDelta == 0){
		if(x3 == x1 && y3 == y1){
			xf = x1; yf = y1;
			return 1;	// points are all equal, kind of get a result
		}
		return -2; // points are not allowed to be co-incidental
	}
	Real u = ( ( x3 - x1 ) * xDelta + ( y3 - y1 ) * yDelta ) / (xDelta * xDelta + yDelta * yDelta);

	if (u < 0 ){
		xf = x1 + ( u * xDelta );
		yf = y1 + ( u * yDelta );
		return 2;		// droped point is before start of segment
	} else if (u > 1 ){
		xf = x1 + ( u * xDelta );
		yf = y1 + ( u * yDelta );
		return 3;		// droped point is after end of segment
	} else {
		xf = x1 + ( u * xDelta );
		yf = y1 + ( u * yDelta );
		return 0;
	}
	Null(xf);Null(yf);
	return -1;
}


/*
	// Should give:
	// Distance from 5,5 to (10,10)-(20,20): 7.071068
	// Distance from 15,15 to (10,10)-(20,20): 0.000000
	// Distance from 15,15 to (20,10)-(20,20): 5.000000
	// Distance from 0,15 to (20,10)-(20,20): 20.000000
	// Distance from 0,25 to (20,10)-(20,20): 20.615528
	// Distance from -13,-25 to (-50,10)-(20,20): 39.880822
*/
Integer drop_point(Real x3, Real y3, Real x1, Real y1, Real x2, Real y2, Real &xf,Real &yf, Real &dist){
	Integer ierr;
	ierr = drop_point_onto_segment( x3,  y3,  x1,  y1,  x2,  y2, xf, yf);
	if(ierr >= 0){
		dist = get_distance(x3,y3,xf,yf);
		return ierr;
	}else{
		Null(dist);
		return ierr;
	}
	return ierr;
}

//--------------------------------------------------------------------------------
//                        PLANES,  THERE ARE 2 WAYS TO DEFINE A PLANE (A,B,C,D) or (vector_normal,point_on_plane)
//--------------------------------------------------------------------------------


#if 0
// work in progress to find the intesection of 2 planes
Integer get_intersection_of_2_lines_and_2_slopes(
		Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Real s1,
		Real x4, Real y4, Real z4, Real x5, Real y5, Real z5, Real s2,
		Real &x7, Real &y7, Real &z7, Real &x8, Real &y8, Real &z8){

	Real x3,y3,z3,x6,y6,z6;

	//find closest end points;

// same as below
//http://local.wasp.uwa.edu.au/~pbourke/geometry/planeeq/
// note d is negative?
	Real A1 = y1*(z2 - z3) + y2*(z3 - z1) + y3 * (z1 - z2);
	Real B1 = z1*(x2 - x3) + z2*(x3 - x1) + z3 * (x1 - x2);
	Real C1 = x1*(y2 - y3) + x2*(y3 - y1) + x3 * (y1 - y2);
	Real D1 = x1*(y2 * z3 - y3 * z2) + x2*(y3 * z1 - y1 * z3) + x3*(y1 * z2 - y2 * z1);
	D1 = D1*(-1.0);
	if(A1 == 0.0 && B1 == 0.0 && C1 == 0.0){
		return 1; //points are colinear (ie points form a line!)
	}

	return 0;
}
#endif

Integer define_plane(Real x1,Real y1,Real z1,Real x2,Real y2,Real z2,Real x3,Real y3,Real z3,Real &A,Real &B,Real &C,Real &D){
//http://local.wasp.uwa.edu.au/~pbourke/geometry/planeeq/
// note d is negative?
	A = y1*(z2 - z3) + y2*(z3 - z1) + y3 * (z1 - z2);
	B = z1*(x2 - x3) + z2*(x3 - x1) + z3 * (x1 - x2);
	C = x1*(y2 - y3) + x2*(y3 - y1) + x3 * (y1 - y2);
	D = x1*(y2 * z3 - y3 * z2) + x2*(y3 * z1 - y1 * z3) + x3*(y1 * z2 - y2 * z1);
	D = D*(-1.0);
	if(A == 0.0 && B == 0.0 && C == 0.0){
		return -1; //points are colinear (ie points form a line!)
	}
	return 0;
}


Real get_z_on_plane(Real A, Real B, Real C, Real D,Real x, Real y){
//A*x + B*y +C*z = -D (note: D is already negative?)
	return ( (0.0 - D - A*x - B*y ) / C);
}


Integer get_point_on_plane(Real A, Real B, Real C, Real D,Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Real &x, Real &y, Real &z){
// project a line defined by P1 & P2 onto a plane defined by ABCD
	Real u = (A*x1 + B * y1 + C * z1 + D) / ( A * (x1 - x2) + B * (y1 - y2) + C * (z1 - z2) );
	if(u == 0)return -1; // is parallel to plane
	x = x1 * u;
	y = y1 * u;
	z = z1 * u;
	if(u >= 0 && u <= 1){
		return 0; // intersects between p1 and p2
	}
	return 1; // intersect extended (outside the extents of p1 and p2
}

//NORMAL plus POINT


Integer get_plane(Real &x[], Real &y[], Real &z[], Vector3 &plane_normal, Point &plane_point){
	get_normal_vector(x,y,z,plane_normal);
	Set_x(plane_point,x[1]);	Set_y(plane_point,y[1]);	Set_z(plane_point,z[1]);
	return 0;
}

Integer get_plane_two_points(Real &x[], Real &y[], Real &z[], Vector3 &plane_normal, Point &plane_point){
	get_normal_vector_from_two_points(x,y,z,plane_normal);
	Set_x(plane_point,x[1]);	Set_y(plane_point,y[1]);	Set_z(plane_point,z[1]);
	return 0;
}

Integer get_plane_one_point(Real &x, Real &y, Real &z, Vector3 &plane_normal, Point &plane_point){
	Set_vector(plane_normal,0.0,0.0,1.0);
	Set_x(plane_point,x);	Set_y(plane_point,y);	Set_z(plane_point,z);
	return 0;
}

Integer get_plane_two_points_slope(Real &x[], Real &y[], Real &z[], Real &slope, Vector3 &plane_normal, Point &plane_point){
	Vector3 vector;	Set_vector(vector,x[2]-x[1],y[2]-y[1],z[2]-z[1]);
	vector = get_perpendicular_vector_add_slope(vector, slope);
	Real xf[3],yf[3],zf[3];
	xf[1] = x[1];		yf[1] = y[1];		zf[1] = z[1];
	xf[2] = x[2];		yf[2] = y[2];		zf[2] = z[2];
	Get_vector(vector,xf[3],yf[3],zf[3]);
	xf[3]+=x[1];		yf[3]+=y[1];		zf[3]+=z[1];
	return get_plane(xf, yf, zf, plane_normal, plane_point);
}


Real get_z_on_plane(Real &vx, Real &vy, Real &vz, Real &px, Real &py, Real &pz, Real &x, Real &y){
// v are the vector points
// known are a known point
// x and y are passed in and the z is calculated
//	v . ( unknownP - knownP) = 0;
	Real z = ( ( (-1.0)*(vx * (x - px)) - (vy *(y-py) ) ) / vz ) + pz;
	return z;
}

Real get_z_on_plane(Vector3 &plane_normal, Point &plane_point, Real &x, Real &y){
	Real vx,vy,vz,px,py,pz;
	Get_vector(plane_normal,vx,vy,vz);
	px = Get_x(plane_point);
	py = Get_y(plane_point);
	pz = Get_z(plane_point);
	return get_z_on_plane(vx,vy,vz,px,py,pz,x,y);
}

Real get_x_on_plane(Real &vx, Real &vy, Real &vz, Real &px, Real &py, Real &pz, Real &y, Real &z){
// v are the vector points
// known are a known point
// y and z are passed in and the z is calculated
//	v . ( unknownP - knownP) = 0;
	Real x = ( ( (-1.0)*(vz * (z - pz)) - (vy *(y-py) ) ) / vx ) + px;
	return x;
}

Real get_y_on_plane(Real &vx, Real &vy, Real &vz, Real &px, Real &py, Real &pz, Real &x, Real &z){
// v are the vector points
// known are a known point
// x and z are passed in and the z is calculated
//	v . ( unknownP - knownP) = 0;
	Real y = ( ( (-1.0)*(vx * (x - px)) - (vz *(z-pz) ) ) / vy ) + py;
	return y;
}


//--------------------------------------------------------------------------------
//                        GEOMETRY WITH SHAPES
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//                        POLYLINE DIRECTION
//--------------------------------------------------------------------------------
//http://paulbourke.net/geometry/clockwise/index.html#clockwise
#if 0
/*
   Return whether a polygon in 2D is concave or convex
   return 0 for incomputables eg: colinear points
          CONVEX == 1
          CONCAVE == -1
   It is assumed that the polygon is simple
   (does not intersect itself or have holes)
*/
int Convex(XY *p,int n)
{
   int i,j,k;
   int flag = 0;
   double z;

   if (n < 3)
      return(0);

   for (i=0;i<n;i++) {
      j = (i + 1) % n;
      k = (i + 2) % n;
      z  = (p[j].x - p[i].x) * (p[k].y - p[j].y);
      z -= (p[j].y - p[i].y) * (p[k].x - p[j].x);
      if (z < 0)
         flag |= 1;
      else if (z > 0)
         flag |= 2;
      if (flag == 3)
         return(CONCAVE);
   }
   if (flag != 0)
      return(CONVEX);
   else
      return(0);
}
#endif

#define CONVEX 1
#define CONCAVE -1
Integer test_convex(Real x[], Real y[],Integer n)
{
   Integer i,j,k;
   Integer flag = 0;
   Real z;

   if (n < 3)
      return(0);
   //Print("n"+To_text(n) );Print();
   for (i=1;i<=n;i++) {
      //j = (i + 1) % n;	//
      j = 1+ (i % n);
      k = 1+ (j % n);
      //Print("i"+To_text(i) + ", j"+To_text(j) + ", k"+To_text(k) );Print();
      z  = ( x[j] - x[i] ) *  ( y[k] - y[j] );
      z -= (y[j] - y[i]) * (x[k] - x[j] );
      if (z < 0)
         flag |= 1;
      else if (z > 0)
         flag |= 2;
      if (flag == 3)
         return(CONCAVE);
   }
   if (flag != 0)
      return(CONVEX);
   else
      return(0);
   return (0);
}

Real get_area(Real x[], Real y[], Integer n){
//+ve area means the coords are counter clockwise
//-ve area means the coords are clockwise
	Real area = 0.0;
	Integer i,j;
	//Print("n"+To_text(n) );Print();
	if(n <3)
		return(0.0);	//should this be an error?
	for(i=1;i<=n;i++){
		j = 1 + (i  % n );
		//Print("i"+To_text(i) + ", j"+To_text(j) );Print();
		area+= x[i] * y[j];
		area-= y[i] * x[j];
	}
	area/=2.0;
	return(area);
}

Integer polyCentroid(Real x[], Real y[], Integer n, Real &xCentroid, Real &yCentroid, Real &area){
	Real i, j;
	Real ai, atmp = 0.0, xtmp = 0.0, ytmp = 0.0;
	if (n < 3) return 1;
	i = n;
	for (j = 1; j <= n; j++){
		ai = x[i] * y[j] - x[j] * y[i];
		atmp += ai;
		xtmp += (x[j] + x[i]) * ai;
		ytmp += (y[j] + y[i]) * ai;
		i=j;
	}
	area = atmp / 2.0;
	if (atmp != 0.0){
		xCentroid =	xtmp / (3.0 * atmp);
		yCentroid =	ytmp / (3.0 * atmp);
		return 0;
	}
	return 2;
}

#if 0
Contour.prototype.centroid = function() {
   var pts = this. pts;
   var nPts = pts.length;
   var x=0; var y=0;
   var f;
   var j=nPts-1;
   var p1; var p2;

   for (var i=0;i<nPts;j=i++) {
      p1=pts[i]; p2=pts[j];
      f=p1.x*p2.y-p2.x*p1.y;
      x+=(p1.x+p2.x)*f;
      y+=(p1.y+p2.y)*f;
   }

   f=this.area()*6;
   return new Point({x: x/f,y:y/f});
};
#endif

Integer get_centroid(Real x[], Real y[], Integer n, Real &xf, Real &yf){
	xf=0.0;yf=0.0;
	Real f;
	Integer j;
	for(Integer i=1;i<=n;i++){
		j = 1 + (i  % n );
		f = x[j] * y[i] - x[j] * y[i];
		xf+=(x[j]+x[i])*f;
		yf+=(y[j]+y[i])*f;
	}
	f = get_area(x,y,n) * 6.0;
	xf/=f;
	yf/=f;
	return (0);
}



// medial axis?
Integer calc_centroid(Element element,Real tol_c2a,Real &Cx,Real &Cy, Real &A)
// --------------------------------------------------------------------------
// Owen T - http://forums.12dmodel.com/viewtopic.php?t=5446&start=15
// --------------------------------------------------------------------------
{
  Real length;
  if(Get_length(element,length) != 0) return(-3);

  if(xeqy(length,0.0)) return(-4);

  Text type;
  Get_type(element,type);

  Element el_super;
  Integer do_holes = 0;

  if(type != "Super") {
    if(Convert(element,"Super",el_super) != 0) return(-5);
  }
  else {
    if(Element_duplicate(element,el_super) != 0) return(-6);
    Get_super_use_hole(element,do_holes);
  }

  Set_super_use_interval(el_super,1);
  Set_super_interval_chord_arc(el_super,tol_c2a);

  Element el_2d;

  if(Convert(el_super,"2d",el_2d) != 0) return(-8);
  Element_delete(el_super);

  Integer numpts;
  if(Get_points(el_2d,numpts) != 0) return(-9);

  if(numpts < 3) return(-10);

  Real A6,xl,yl,x,y,Cterm;

  for(Integer i=1; i<=(numpts+1); i++) {

    Integer ii = (i > numpts) ? 1 : i;

    Get_2d_data(el_2d,ii,x,y);

    if(i==1) {

      A  = 0.0;
      Cx = 0.0;
      Cy = 0.0;
    }
    else {

      Cterm = xl*y - x*yl;
      A  += Cterm;
      Cx += (xl + x)*Cterm;
      Cy += (yl + y)*Cterm;
    }

    xl = x;
    yl = y;
  }

  Element_delete(el_2d);

  A *= 0.5;
  A6 = 6.0*A;

  if(xeqy(A,0.0)) return(-11);

  Cx /= A6;
  Cy /= A6;

  if(do_holes) {

    Real CxA  = Cx*A;
    Real CyA  = Cy*A;
    Real Atot = A;

    Real Ah;

    Element hole;

    Integer num_holes = 0;
    Get_super_holes(element,num_holes);

    for(Integer k=1; k<=num_holes; k++) {

      if(Super_get_hole(element,k,hole) != 0) continue;

      if(calc_centroid(hole,tol_c2a,Cx,Cy,Ah) != 0) continue; //recursive

      if(A*Ah < 0.0) Ah = -Ah; //make sure holes are subtractive

      CxA  -= Cx*Ah;
      CyA  -= Cy*Ah;
      Atot -= Ah;
    }

    if(xley(A*Atot,0.0)) return(-12); //sum of holes is bigger than parent

    Cx = CxA/Atot;
    Cy = CyA/Atot;
    A  = Atot;
  }

  return(0);
}

#endif