#ifndef mashy_lib_geometry_points_H_included
#define mashy_lib_geometry_points_H_included

// Given a directed line pq, determine whether qr turns CW or CCW.
Integer which_side(Real px, Real py, Real qx, Real qy, Real rx, Real ry){
	Real result = (px - qx) * (qy - ry) - (py - qy) * (qx - rx);
	if (result < 0.0) return -1;	// q lies to the left  (qr turns CW)
	if (result > 0.0) return  1;	// q lies to the right (qr turns CCW)
	return 0;						// q lies on the line from p to r
}

Integer compare(Real px, Real py, Real qx, Real qy)		/* Lexicographic comparison of p and q*/
{
    if (px < qx) return -1;	/* p is less than q.			*/
    if (px > qx) return  1;	/* p is greater than q.		*/
    if (py < qy) return -1;	/* p is less than q.			*/
    if (py > qy) return  1;	/* p is greater than q.		*/
    return 0;				/* p is equal to q.			*/
}


#endif