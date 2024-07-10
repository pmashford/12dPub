#ifndef mashy_lib_geometry_parabola_h_included
#define mashy_lib_geometry_parabola_h_included

#include "X:\12d\MACROS\_INCLUDES\set_ups.H"

//http://forums.12dmodel.com/viewtopic.php?t=4619

Integer solve3x3(Real u[], Real v1[], Real v2[], Real v3[], Real &a, Real &b, Real &c){
	Real det;

  det = v1[1] * (v2[2] * v3[3] - v2[3] * v3[2])
      + v1[2] * (v2[3] * v3[1] - v2[1] * v3[3])
      + v1[3] * (v2[1] * v3[2] - v2[2] * v3[1]);

  if(det == 0.0) return TRUE; //if(det == 0.0) return FALSE; //c++

  Real w1[3];
  Real w2[3];
  Real w3[3];

  w1[1] =  (v2[2] * v3[3] - v2[3] * v3[2]) / det;
  w1[2] = -(v1[2] * v3[3] - v1[3] * v3[2]) / det;
  w1[3] =  (v1[2] * v2[3] - v1[3] * v2[2]) / det;

  w2[1] = -(v2[1] * v3[3] - v2[3] * v3[1]) / det;
  w2[2] =  (v1[1] * v3[3] - v1[3] * v3[1]) / det;
  w2[3] = -(v1[1] * v2[3] - v1[3] * v2[1]) / det;

  w3[1] =  (v2[1] * v3[2] - v2[2] * v3[1]) / det;
  w3[2] = -(v1[1] * v3[2] - v1[2] * v3[1]) / det;
  w3[3] =  (v1[1] * v2[2] - v1[2] * v2[1]) / det;

  c = w1[1] * u[1] + w1[2] * u[2] + w1[3] * u[3];
  b = w2[1] * u[1] + w2[2] * u[2] + w2[3] * u[3];
  a = w3[1] * u[1] + w3[2] * u[2] + w3[3] * u[3];

  return FALSE; //return TRUE;  //c++
}

// given 3 points a parabola return a b c
void get_abc(Point &s, Point &m, Point &e, Real &a, Real &b, Real &c)
{
	Real u[3];

	u[1] = Get_y(s); //u[1] = s.ycoord();
	u[2] = Get_y(m); //u[2] = m.ycoord();
	u[3] = Get_y(e); //u[3] = e.ycoord();

	//double v[3][3];
	Real v1[3];
	Real v2[3];
	Real v3[3];

	v1[1] = 1.0; //v[1][1] = 1.0;
	v1[2] = Get_x(s); //v[1][2] = s.xcoord();
	v1[3] = Get_x(s) * Get_x(s); //v[1][3] = s.xcoord()*s.xcoord();

	v2[1] = 1.0; //v[2][1] = 1.0;
	v2[2] = Get_x(m); //v[2][2] = m.xcoord();
	v2[3] = Get_x(m) * Get_x(m); //v[2][3] = m.xcoord()*m.xcoord();

	v3[1] = 1.0; //v[3][1] = 1.0;
	v3[2] = Get_x(e); //v[3][2] = e.xcoord();
	v3[3] = Get_x(e) * Get_x(e); //v[3][3] = e.xcoord()*e.xcoord();

	solve3x3(u,v1,v2,v3,a,b,c);
}

// get the ip
// cs - start chainage
// ce - end chainage
// ci - ip chainage
// hi - ip height

void get_ip(Real a, Real b, Real c, Real cs, Real ce, Real &ci, Real &hi){
	Real m = 2.0*a*cs + b;
	Real n  = a*cs*cs + b*cs + c;

	ci = (cs + ce)/2.0;
	hi = m*c + n;
}

void get_ip(Real ch1, Real ht1, Real ch2, Real ht2, Real ch3, Real ht3, Real cs, Real ce, Real &ip_ch, Real &ip_ht){

	Point s,m,e;
	Set_x(s,ch1);	Set_y(s,ht1);
	Set_x(m,ch2);	Set_y(m,ht2);
	Set_x(e,ch3);	Set_y(e,ht3);

	Real a,b,c;
	get_abc(s, m, e, a, b, c);

	get_ip(a,b,c,cs,ce,ip_ch,ip_ht);
}

#endif