#ifndef mashy_lib_geometry_vectors_H_included
#define mashy_lib_geometry_vectors_H_included


Vector2 radians_to_vector(Real dir){
	Vector2 vector;
	Set_vector(vector,Cos(dir),Sin(dir));
	return Get_vector_normalize(vector);
}

Real change_of_angle(Vector2 v1, Vector2 v2){
	return Acos(v1*v2);
}

Real change_of_angle(Real dir1, Real dir2){
	return change_of_angle(radians_to_vector(dir1),radians_to_vector(dir2));
}


Vector3 get_vector_between_points(Real x[], Real y[], Real z[]){
	Vector3 vector;
	Set_vector(vector,x[2] - x[1], y[2] - y[1], z[2] - z[1]);
	return vector;
}

Vector2 get_vector_between_points(Real x[], Real y[]){
	Vector2 vector;
	Set_vector(vector,x[2] - x[1], y[2] - y[1]);
	return vector;
}


void normalise(Real &x,Real &y,Real &z,Real &nx,Real &ny,Real &nz)
// --------------------------------------------------------
// Normalise a vector, for vector(x,y,z) this produces a vector with the length of 1
// --------------------------------------------------------
{
	Real length = Sqrt((x*x) + (y*y) + (z*z));
	if (length != 0.0){
		nx = x / length;
		ny = y / length;
		nz = z / length;
	}else{
		nx = 0.0;
		ny = 0.0;
		nz = 0.0;
	}
}

void normalise(Real &x,Real &y,Real &nx,Real &ny)
// --------------------------------------------------------
// Normalise a vector, for vector(x,y) this produces a vector with the length of 1
// --------------------------------------------------------
{
	Real length = Sqrt((x*x) + (y*y));
	if (length != 0.0){
		nx = x / length;
		ny = y / length;
	}else{
		nx = 0.0;
		ny = 0.0;
	}
}

// 3 points define a plane, take the points and construct a normal vector
Integer get_normal_vector(Real x[], Real y[], Real z[], Vector3 &vector){
#if 0
	//This is the normal vector, nx,ny,nz
	//vectors do not need to be normalized
	Real nx = (y[2] - y[1])*(z[3] - z[1]) - (y[3] - y[1])*(z[2] - z[1]);
	Real ny = (z[2] - z[1])*(x[3] - x[1]) - (z[3] - z[1])*(x[2] - x[1]);
	Real nz = (x[2] - x[1])*(y[3] - y[1]) - (x[3] - x[1])*(y[2] - y[1]);
	if(nz < 0){ // the vector must point up
		nx*=(-1.0);
		ny*=(-1.0);
		nz*=(-1.0);
	}
	return(Set_vector(vector,nx,ny,nz));
#else
	//Vector3 AB;	Set_vector(AB,x[2] - x[1],y[2] - y[1],z[2] - z[1]);
	//Vector3 BC;	Set_vector(BC,x[3] - x[1],y[3] - y[1],z[3] - z[1]);
	Real nx = ( (y[2] - y[1]) * (z[3] - z[1]) ) - ( (z[2] - z[1]) * (y[3] - y[1]) );
	Real ny = ( (z[2] - z[1]) * (x[3] - x[1]) ) - ( (x[2] - x[1]) * (z[3] - z[1]) );
	Real nz = ( (x[2] - x[1]) * (y[3] - y[1]) ) - ( (y[2] - y[1]) * (x[3] - x[1]) );
	if(nz < 0){ // the vector must point up
		nx*=(-1.0);
		ny*=(-1.0);
		nz*=(-1.0);
	}
	return(Set_vector(vector,nx,ny,nz));
#endif
}

Integer get_normal_vector(Real x[], Real y[], Real z[], Real &nx, Real &ny, Real &nz){
	//This is the normal vector, nx,ny,nz
	//vectors do not need to be normalized
	nx = (y[2] - y[1])*(z[3] - z[1]) - (y[3] - y[1])*(z[2] - z[1]);
	ny = (z[2] - z[1])*(x[3] - x[1]) - (z[3] - z[1])*(x[2] - x[1]);
	nz = (x[2] - x[1])*(y[3] - y[1]) - (x[3] - x[1])*(y[2] - y[1]);
	if(nz < 0){ // the vector must point up
		nx*=(-1.0);
		ny*=(-1.0);
		nz*=(-1.0);
	}

	return(0);
}

Integer get_normal_vector_from_two_points(Real x[], Real y[], Real z[], Vector3 &vector){
	//first find point 3 which is perpendicular to point [1] ... it has a z of z[1].
	Real x3 = x[1] + y[2] - y[1];
	Real y3 = y[1] + (x[2] - x[1])*(-1.0);
	Real z3 = z[1];
	//now calc the normal vector for the 3 points [1],[2],3
	Real nx = ( (y[2] - y[1]) * (z3 - z[1]) ) - ( (z[2] - z[1]) * (y3 - y[1]) );
	Real ny = ( (z[2] - z[1]) * (x3 - x[1]) ) - ( (x[2] - x[1]) * (z3 - z[1]) );
	Real nz = ( (x[2] - x[1]) * (y3 - y[1]) ) - ( (y[2] - y[1]) * (x3 - x[1]) );
	if(nz < 0){ // the vector must point up
		nx*=(-1.0);
		ny*=(-1.0);
		nz*=(-1.0);
	}
	return(Set_vector(vector,nx,ny,nz));


}

Integer get_normal_vector_from_one_point(Real x, Real y, Real z, Vector3 &vector){
	return(Set_vector(vector,0,0,1));
}

void get_perpendicular_vector(Real &Vx, Real &Vy, Real &Vz, Real &px, Real &py, Real &pz){
	//to get a perpendicular vector simply swap two values (swap the sign of one), and make the 3rd =0
	//now we have 2 vectors which are said to be normal to each other.
	px = Vy * (-1.0);
	py = Vx;
	pz = 0;
}

Vector3 get_perpendicular_vector(Vector3 &vector){
//creates a perpendicular vector on the RHS
	//to get a perpendicular vector simply swap two values (swap the sign of one), and make the 3rd =0
	//now we have 2 vectors which are said to be normal to each other.
	Real vx,vy,vz;
	Get_vector(vector,vx,vy,vz);
	Vector3 new_vector;	Set_vector(new_vector,vy,vx * (-1.0),0.0);
	return new_vector;
}

Vector3 get_perpendicular_vector_add_slope(Vector3 &vector, Real &slope){//NOTE the direction of the vector is important
//creates a perpendicular vector on the RHS
	//to get a perpendicular vector simply swap two values (swap the sign of one), and make the 3rd =0
	//now we have 2 vectors which are said to be normal to each other.
	Real vx,vy,vz;
	Get_vector(vector,vx,vy,vz);
	Vector3 new_vector;	Set_vector(new_vector,vy,vx * (-1.0),0.0);
	Set_vector(new_vector,3,Get_vector_length(new_vector) * slope);	//calculate the rise/fall of the vector from the supplied slope
	return new_vector;
}


Real get_dot_product(Vector2 &v1, Vector2 &v2){
///these vectors should be normalized if you want the radians
	Real x[2],y[2];
	Get_vector(v1,x[1],y[1]);
	Get_vector(v2,x[2],y[2]);
	return ( x[1] * x[2] + y[1] * y[2]);
}

Real get_dot_product(Real x1,Real y1, Real x2, Real y2){
///these vectors should be normalized if you want the radians
	return (  x1 * x2 + y1 * y2);
}

Real get_angle_between_vectors(Vector2 &v1, Vector2 &v2){
//this returns radians
	v1 = Get_vector_normalize(v1);
	v2 = Get_vector_normalize(v2);
	return (get_dot_product(v1,v2));
}
/*
axis-angle version
sfrotation angleBetween(sfvec3f v1,sfvec3f v2) {
	float angle;
	// turn vectors into unit vectors
	n1 = v1.norm();
	n2 = v2.norm();
	angle = Math.acos( sfvec3f.dot(n1,n2) );
	// if no noticable rotation is available return zero rotation
	// this way we avoid Cross product artifacts
	if( Math.abs(angle) < 0.0001 ) return new sfrotation( 0, 0, 1, 0 );
	// in this case there are 2 lines on the same axis
	if(Math.abs(angle)-Math.pi) < 0.001){
		n1 = n1.Rotx( 0.5f );
		// there are an infinite number of normals
		// in this case. Anyone of these normals will be
		// a valid rotation (180 degrees). so I rotate the curr axis by 0.5 radians this way we get one of these normals
	}
 	sfvec3f axis = n1;
	axis.cross(n2);
	return new sfrotation(axis.x,axis.y,axis.z,angle);
}
*/
//angle between 2 vectors
// the result of a dot product is a number
Real get_dot_product(Vector3 &v1, Vector3 &v2){
///these vectors should be normalized if you want the radians
	Real x[2],y[2],z[2];
	Get_vector(v1,x[1],y[1],z[1]);
	Get_vector(v2,x[2],y[2],z[2]);
	return ( x[1] * x[2] + y[1] * y[2] + z[1] * z[2]);
}

Real get_dot_product(Real x1,Real y1, Real z1, Real x2, Real y2, Real z2){
///these vectors should be normalized if you want the radians
	return ( x1 * x2 + y1 * y2 + z1 * z2);
}

Real get_angle_between_vectors(Vector3 &v1, Vector3 &v2){
//this returns radians
	v1 = Get_vector_normalize(v1);
	v2 = Get_vector_normalize(v2);
	return (get_dot_product(v1,v2));
}


Real get_degrees_between_vectors(Vector3 &v1, Vector3 &v2){
//this returns degrees
	v1 = Get_vector_normalize(v1);
	v2 = Get_vector_normalize(v2);
	Real angle = Acos( get_dot_product(v1,v2) );
	return (angle);
}

Real get_slope(Vector3 &v1){
	v1 = Get_vector_normalize(v1);
	Vector3 v2;	Set_vector(v2,0,0,1); //this should already be normalized
	return (get_angle_between_vectors(v1,v2));
}

/*

// intersect3D_SegmentPlane(): intersect a segment and a plane
//    Input:  S = a segment, and Pn = a plane = {Point V0; Vector n;}
//    Output: *I0 = the intersect point (when it exists)
//    Return: 0 = disjoint (no intersection)
//            1 = intersection in the unique point *I0
//            2 = the segment lies in the plane
int
intersect3D_SegmentPlane( Segment S, Plane Pn, Point* I )
{
    Vector    u = S.P1 - S.P0;
    Vector    w = S.P0 - Pn.V0;

    float     D = dot(Pn.n, u);
    float     N = -dot(Pn.n, w);

    if (fabs(D) < SMALL_NUM) {          // segment is parallel to plane
        if (N == 0)                     // segment lies in plane
            return 2;
        else
            return 0;                   // no intersection
    }
    // they are not parallel
    // compute intersect param
    float sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                       // no intersection

    *I = S.P0 + sI * u;                 // compute segment intersect point
    return 1;
}

*/


#define SMALL_NUM 0.0000001
Integer get_segment_intersect_with_plane(Real sx[], Real sy[], Real sz[], Point &known_point_on_plane, Vector3 &normal_vector_to_plane, Real &x, Real &y, Real &z){
//return -ve if segment is parallel or coincidental
//return 0 if segment intersects
//return 1 if extended segment intersects

#if 0
//highjacking here works perfectly - therefore the normal or the point are not getting constructed
	Set_vector(normal_vector_to_plane,0,10,10);
	Set_x(known_point_on_plane,0);Set_y(known_point_on_plane,10);Set_z(known_point_on_plane,2);

	//Print("x "+To_text(x,3)+" y "+To_text(y,3)+" z "+To_text(z,3));Print();
	//Print("done - get_segment_intersect_with_plane");Print();
#endif

	Real nx,ny,nz;
	Get_vector(normal_vector_to_plane,nx,ny,nz);
	//paul burke
	Real u = ( (nx * ( Get_x(known_point_on_plane)-sx[1])) + (ny * ( Get_y(known_point_on_plane)-sy[1]))+ (nz * ( Get_z(known_point_on_plane)-sz[1])) ) / ( (nx * ( sx[2]-sx[1])) + (ny * ( sy[2]-sy[1]))+ (nz * ( sz[2]-sz[1])) );

	x = sx[1] + u * (sx[2] - sx[1]);
	y = sy[1] + u * (sy[2] - sy[1]);
	z = sz[1] + u * (sz[2] - sz[1]);
	if(u < 0 || u > 1) return 1; //return 1 if the segment intersects if extended
	return 0;
}


#endif