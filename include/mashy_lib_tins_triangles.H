#ifndef mashy_lib_tins_triangles_h_included
#define mashy_lib_tins_triangles_h_included

#include "mashy_lib_geometry.H"

Integer intersect_line_triangle (
                             Real lx0,Real ly0,Real lz0, // first  point on line
                             Real lx1,Real ly1,Real lz1, // second point on line

                             Real tx0,Real ty0,Real tz0, // first  point on triangle
                             Real tx1,Real ty1,Real tz1, // second point on triangle
                             Real tx2,Real ty2,Real tz2, // third  point on triangle

                             Real& ix,Real& iy,Real& iz, // intersection point
                             Real& t, Real& u, Real& v,  // line and plane parameters for intersection point
                             Integer& online, Integer& intri                    // 1 means on/in
                             ){
	Real a11 = lx0 - lx1;
	Real a21 = ly0 - ly1;
	Real a31 = lz0 - lz1;
	Real a12 = tx1 - tx0;
	Real a22 = ty1 - ty0;
	Real a32 = tz1 - tz0;
	Real a13 = tx2 - tx0;
	Real a23 = ty2 - ty0;
	Real a33 = tz2 - tz0;

	Real i11 =  (a33*a22-a32*a23);
	Real i12 = -(a33*a12-a32*a13);
	Real i13 =  (a23*a12-a22*a13);

	Real det = a11*i11+a21*i12+a31*i13;
	if (det == 0.0) return -1;

	i11 /= det;
	i12 /= det;
	i13 /= det;

	Real i21 = -(a33*a21-a31*a23) / det;
	Real i22 =  (a33*a11-a31*a13) / det;
	Real i23 = -(a23*a11-a21*a13) / det;
	Real i31 =  (a32*a21-a31*a22) / det;
	Real i32 = -(a32*a11-a31*a12) / det;
	Real i33 =  (a22*a11-a21*a12) / det;

	Real dx = lx0 - tx0;
	Real dy = ly0 - ty0;
	Real dz = lz0 - tz0;

	t = i11*dx + i12*dy + i13*dz;
	u = i21*dx + i22*dy + i23*dz;
	v = i31*dx + i32*dy + i33*dz;

	online = (0.0 <= t && t <= 1.0) ? 1 : 0;

	intri = 1;
	if (u < 0.0 || u > 1.0)
		intri = 0;
	else if (v < 0.0 || (u + v) > 1.0)
		intri = 0;

	ix = lx0 + (lx1 - lx0) * t;
	iy = ly0 + (ly1 - ly0) * t;
	iz = lz0 + (lz1 - lz0) * t;

	return 0;
}

Integer intersect_line_triangle (
                             Tin tin, Integer triangle,  // tin and triangle

                             Real lx0,Real ly0,Real lz0, // first  point on line
                             Real lx1,Real ly1,Real lz1, // second point on line

                             Real& ix,Real& iy,Real& iz, // intersection point
                             Real& t, Real& u, Real& v,  // line and plane parameters for intersection point
                             Integer& online, Integer& intri                    // 1 means on/in
                             ){
	Real tx0,ty0,tz0, tx1, ty1, tz1, tx2, ty2, tz2;
	Integer ok = 0;
	Integer p1,p2,p3,n1,n2,n3;
	if( Tin_get_triangle(tin,triangle,p1,p2,p3,n1,n2,n3,tx0,ty0,tz0, tx1, ty1, tz1, tx2, ty2, tz2) )
		return 1;

	return 	intersect_line_triangle ( lx0, ly0, lz0, lx1, ly1, lz1, tx0, ty0, tz0, tx1, ty1, tz1, tx2, ty2, tz2, ix, iy, iz, t,  u,  v, online,  intri) ;
}

void append_unique(Integer needle, Dynamic_Integer &haystack){
	Integer items;
	Get_number_of_items(haystack,items);
	for(Integer i=1;i<=items;i++){
		Integer this_i;
		Get_item(haystack,i,this_i);
		if(this_i == needle)return;
	}
	Append(needle,haystack);
}

Integer get_crossing_triangles(Element &seed, Tin &tin, Dynamic_Integer &triangles){
	Dynamic_Element delt, draped_strings;
	Append(seed,delt);
	Drape(tin,delt,draped_strings);

	Integer items;
	Get_number_of_items(draped_strings,items);
	for(Integer i=items;i>0;i--){
		Element e;
		Get_item(draped_strings,i,e);
		// get the triangle at the midpoint of each segment
		// !assume no arcs
		Integer npts;
		Get_points(e,npts);
		for(Integer j=2;j<=npts;j++){
			Real x1,y1,z1,x2,y2,z2;
			Get_data(e,j-1,x1,y1,z1);
			Get_data(e,j,x2,y2,z2);
			Real x = (x1+x2) / 2.0;
			Real y = (y1+y2) / 2.0;
			Integer triangle;
			Tin_get_triangle_from_point(tin,x,y,triangle);
			append_unique(triangle,triangles);
		}
		Element_delete(e);
	}
	return 0;
}

Integer get_crossing_triangles(Real x1, Real y1, Real x2, Real y2, Tin &tin, Dynamic_Integer &triangles){
	Integer npts = 2;
	Real x[npts], y[npts], z[npts],r[npts];
	Integer f[npts];
	for(Integer i=1;i<=npts;i++){
		z[i] = 0.0;
		r[i] = 0.0;
		f[i] = 0;
	}
	x[1] = x1;	y[1] = y1;
	x[2] = x2;	y[2] = y2;
	Integer flag1 = 0;
	Element seed = Create_super(flag1, x, y, z, r, f, npts);
	return get_crossing_triangles(seed, tin, triangles);
}


Element create_triangle(Real x1, Real y1, Real x2, Real y2, Real x3, Real y3){
	Integer npts = 3;
	Real x[npts], y[npts], z[npts],r[npts];
	Integer f[npts];
	for(Integer i=1;i<=npts;i++){
		z[i] = 0.0;
		r[i] = 0.0;
		f[i] = 0;
	}
	x[1] = x1;	y[1] = y1;
	x[2] = x2;	y[2] = y2;
	x[3] = x3;	y[3] = y3;
	Integer flag1 = 0;
	return Create_super(flag1, x, y, z, r, f, npts);
}

void set_triangle_colour(Tin &tin, Integer triangle, Integer colour){
	Integer p1,p2,p3,n1,n2,n3;
	Real x1,y1,z1,x2,y2,z2,x3,y3,z3;
	Tin_get_triangle( tin, triangle,p1,p2,p3,n1,n2,n3,x1,y1,z1,x2,y2,z2,x3,y3,z3);
	Element e = create_triangle(x1,y1,x2,y2,x3,y3);
	Integer mode = 0; // 0 inside, 1 outside
	Colour_triangles(tin,colour, e, mode);
	Element_delete(e);
}

Integer find_intersection_with_line(Tin tin, Real x1, Real y1, Real z1, Real x2, Real y2, Real z2, Integer &triangle, Real &ix, Real &iy, Real &iz){
	triangle = 0;
	Dynamic_Integer crossing_triangles;
	get_crossing_triangles(x1, y1, x2, y2, tin, crossing_triangles);
	Integer ntri;
	Get_number_of_items(crossing_triangles,ntri);
	if(ntri<1)return 1;
	for(Integer i=1;i<=ntri;i++){
		Integer this_tri;
		Get_item(crossing_triangles,i,this_tri);
		Real t,u,v;
		Integer online,intri;
		intersect_line_triangle (tin, this_tri, x1, y1, z1, x2, y2, z2, ix, iy, iz, t, u, v, online, intri);
		if(online && intri){
			triangle = this_tri;
			return 0;
		}
	}
	return 2;
}

Integer find_intersection(Tin tin, Real x1, Real y1, Real z1, Real dir, Real grade_percent, Real max_search, Integer &triangle, Real &ix, Real &iy, Real &iz){
	triangle = 0;
	Real x2,y2,z2=z1+(max_search*grade_percent);
	locate_point(x1, y1, dir, max_search, x2, y2);
	return find_intersection_with_line(tin, x1, y1, z1, x2, y2, z2, triangle, ix, iy, iz);
}

#endif