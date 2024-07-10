#ifndef mashy_lib_interface_h_included
#define mashy_lib_interface_h_included

#include "X:\12d\MACROS\_INCLUDES\set_ups.h"
#include "X:\12d\MACROS\_INCLUDES\standard_library.h"
#include "X:\12d\MACROS\_INCLUDES\mashy_lib_element_functions.H"
#include "X:\12d\MACROS\_INCLUDES\mashy_lib_geometry.H"

Element create_string_to_interface_from(Real x, Real y, Real z, Real dir){
// auxillary function
	Point start, end;
	Set_x(start,x);		Set_y(start,y);	Set_z(start,z);
	Real distance = 1.0; // changed from 1
	Locate_point(start,dir,distance,end);
	Element section = Create_2d(2);
	Set_2d_data(section,1,Get_x(start),Get_y(start));
	Set_2d_data(section,2,Get_x(end),Get_y(end));
	Set_2d_data(section,z);
	Calc_extent(section);
	return(section);
}

Element create_interface_section(Real x, Real y, Real z, Real xf, Real yf, Real zf){
// auxillary function
	Integer flags = String_Super_Bit(ZCoord_Array) | String_Super_Bit(ZCoord_Value);
	Element string = Create_super(flags,2);
	Real r = 0.0;
	Integer f = 0;
	Set_super_data  (string,1,x,y,z,r,f);
	Set_super_data  (string,2,xf,yf,zf,f,f);
	Calc_extent(string);
	return(string);
}

Element interface(Real x, Real y, Real z, Real dir, Tin tin, Real cut_slope, Real fill_slope, Real search_dist, Real &xf, Real &yf, Real &zf, Integer &cut_or_fill){
// Interface from a point with a direction => return the interface section, and some info (x,y,z) about the point
// cut_or_fill not yet implemented, but cut will be -1, fill will be 1, 0 ther was an error
// + creating a bit of junk so hoping for a BER => http://beta.12dmodel.com/forums/viewtopic.php?t=2728&sid=31329ef6593cc68facf9aa724ebbb732
// + could maybe say if it was cut or fill
// + atts?
	Element string = create_string_to_interface_from(x,y,z,dir + Half_pi());	// dummy string
	Real separation = 10.0;
	Integer side = -1;
	Element int_string;	// dummy interface string
	Dynamic_Element ticks;
	Integer ierr = Interface(tin,string,cut_slope,fill_slope,separation,search_dist,side,int_string,ticks);
	Element section;
	cut_or_fill = 0;
	Null(xf);Null(yf);Null(zf);
	Element check_tick;
	Get_item(ticks,1,check_tick);
	Real x1,y1,z1;
	Get_data(check_tick,1,x1,y1,z1);
	if(xeqy(x,x1) && xeqy(y,y1)){
		Get_data(check_tick,2,xf,yf,zf);
		if(z - zf > 0){
			cut_or_fill = 1;// in fill
		}else {
			cut_or_fill = -1;// in cut
		}
	}else{
		cut_or_fill = 0;
	}
	Element_delete(string);	Element_delete(int_string);
	Null_item(ticks,1); //we want to null this item before the rest is deleted
	elements_delete(ticks);
	Null(ticks);
	return(check_tick);
}

Element interface(Real x, Real y, Real z, Real dir, Tin tin, Real xfall, Real search_dist, Real &xf, Real &yf, Real &zf){

	Element string = create_string_to_interface_from(x,y,z,dir + Half_pi());	// dummy string
	//p(To_text(x,3) + " " +To_text(y,3) + " " +To_text(z,3) + " " +To_text(dir,3));
	Real separation = 10;
	Integer side = -1;
	Element int_string;	// dummy interface string
	Dynamic_Element ticks;
	xfall = 1 / xfall ;
	Integer neg = -1;
	Real cut_slope,fill_slope;
	cut_slope = xfall ;// inverse the sign
	fill_slope = xfall*neg;
	//p(xfall);p(cut_slope);p(fill_slope);
	//Text name; Get_name(tin,name); p("tin " + name);
	Integer ierr = Interface(tin,string,cut_slope,fill_slope,separation,search_dist,side,int_string,ticks);
	//p("SEARCH DIST",search_dist);
	//Integer ierr = Interface(tin,string,4,-4,separation,search_dist,side,int_string,ticks);

	Integer items;
	Get_number_of_items(ticks,items);
	//p("Number of ticks",items);

	Null(xf);Null(yf);Null(zf);
	Element check_tick;
	Get_item(ticks,1,check_tick);

	Real x1,y1,z1;
	Get_data(check_tick,1,x1,y1,z1);
	if(xeqy(x,x1) && xeqy(y,y1)){
		Get_data(check_tick,2,xf,yf,zf);
		//print("__________________________________Calculated interface " + To_text(xf,3)  + " " + To_text(yf,3)  + " " + To_text(zf,3) );

	}else{
		locate_point(x, y, dir + Pi(), search_dist, xf, yf);
		zf = z + 1/xfall * search_dist;
		check_tick = Create_3d(2);
		Set_3d_data(check_tick,1, x,y,z);
		Set_3d_data(check_tick,2, xf,yf,zf);
		Calc_extent(check_tick);
		Set_name(check_tick,"batter");
	}

	Element_delete(string);	Element_delete(int_string);
	Null_item(ticks,1); //we want to null this item before the rest is deleted
	elements_delete(ticks);
	Null(ticks);
	return(check_tick);

}

Integer interface(Real x, Real y, Real z, Real dir, Real to_rl, Real xfall, Real &xf, Real &yf, Real &zf){
	Real dz = to_rl - z; // dz is the change in height for the interface
	Real dist = dz / xfall;
	zf = to_rl;
	return locate_point(x, y, dir, dist, xf, yf);
}

// Interface from a string to a rl returning a string
Integer interface(Element &e, Real interval, Integer do_specials, Real to_rl, Real xfall, Element &e_interface){
	Print("NOT YET IMPLEMENTED!: interface(Element &e, Real interval, Integer do_specials, Real to_rl, Real xfall, Element &e_interface)\n");
	// TODO: do it
	
	// build the string
	return 0;
}

#endif
