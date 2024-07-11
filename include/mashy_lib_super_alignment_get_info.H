#ifndef mashy_lib_super_alignment_get_info_h_included
#define mashy_lib_super_alignment_get_info_h_included

#include "mashy_lib_geometry.H"
#include "mashy_lib_segments.H"

Text segment_type_to_text(Segment seg){
	Integer type = Get_type(seg);
	switch( type ) {
		case 1 :{ //ip
			return("IP");
		}break;
		case 2 :{ //line
			return("T");
		}break;
		case 3 :{ //arc
			return("C");
		}break;
		case 4 :{ //spiral
			return("S");
		}break;
		case 5 :{ //parabola
			return("PARABOLA");
		}break;

	}
	return("");
}

Text get_tanget_type(Segment &seg1, Segment &seg2){
	Text type = segment_type_to_text(seg1) + segment_type_to_text(seg2);
	if(type == "TT" || type == "T")type = "IP";
	return type;
}


Integer get_ip(Element &horiz, Integer iseg, Real &ipx, Real &ipy){
//to calc the ip of the curve we need to get the instantaneous direction at the start and end of the curve
// if there is a leading or trailing spiral then need to get the instantaneous direction at the start/end of the spiral
//  ? possible bug - what if there is a leading spiral but it is actually the trailing sprial for the previous curve?

	Integer nsegs;	Get_segments(horiz,nsegs);
	Segment seg, seg_p, seg_n;
	Get_segment(horiz,iseg,seg);
	Point p1,p2;	//p1 is the point on the left, p2 is the point on the right
	if(!Get_segment(horiz,iseg-1,seg_p)){
		Integer type = Get_type(seg_p);
		if(type == 4){ // v9 spiral
			//do an extra check here to see if the spiral is actually attached to the curve?
			Spiral spiral;	Get_spiral(seg_p,spiral);
			p1 = Get_start_point(spiral);
		}else if(type == 6){ // v10 spiral
#if VERSION_4D >= 1000
			//do an extra check here to see if the spiral is actually attached to the curve?
			Curve spiral;	Get_curve(seg_p,spiral);
			p1 = Get_start_point(spiral);
#endif
		}else{
			Get_start(seg,p1);
		}

	}else{
		Get_start(seg,p1);
	}
	Real x1,y1,tmp_x,tmp_y,tmp_z,tmp_ch,tmp_off;
	get_point(p1,x1,y1);
	Print("p1 " + To_text(x1,3) + " " + To_text(y1,3));Print();
	Real dir1;	//dir1 is left, dir2 is right, will need to +Pi() to dir2 for calcing the intersection
	Drop_point(horiz,x1,y1,tmp_z,tmp_x,tmp_y,tmp_z,tmp_ch,dir1,tmp_off);

	if(!Get_segment(horiz,iseg+1,seg_n)){
		Integer type = Get_type(seg_n);
		if(type == 4){ // v 9 spiral
			//do an extra check here to see if the spiral is actually attached to the curve?
			Spiral spiral;	Get_spiral(seg_n,spiral);
			p2 = Get_start_point(spiral);			// actually need the start of the spiral
		}if(type == 6){ // v10 spiral
			//do an extra check here to see if the spiral is actually attached to the curve?
#if VERSION_4D >= 1000
			Curve spiral;	Get_curve(seg_n,spiral);
			p2 = Get_start_point(spiral);			// actually need the start of the spiral
#endif
		}else{
			Get_end(seg,p2);
		}

	}else{
		Get_end(seg,p2);
	}
	Real x2, y2;
	get_point(p2,x2,y2);
	Print("p2 " + To_text(x2,3) + " " + To_text(y2,3));Print();
	Real dir2;	//dir1 is left, dir2 is right, will need to +Pi() to dir2 for calcing the intersection
	Drop_point(horiz,x2,y2,tmp_z,tmp_x,tmp_y,tmp_z,tmp_ch,dir2,tmp_off);
	dir2+=Pi();

	get_dir_intersect(x1, y1, dir1, x2, y2, dir2, ipx, ipy);

	Print("caluclated ip " + To_text(ipx,3) + " " + To_text(ipy,3));Print();

	return 0;

}

#if VERSION_4D >= 1000
// this is for v10
Integer get_segment_curve_data(Element &horiz, Integer i, Integer &curve_type, Real xval[], Real yval[], Real length[]){
	Segment seg;
	Get_segment(horiz,i,seg);

	Curve spiral;	Get_curve(seg,spiral);
	Point p1,p2;

	if(Get_leading(spiral)){
		p1 = Get_start_point(spiral);
		p2 = Get_end_point(spiral);
	}else{
		p1 = Get_end_point(spiral);
		p2 = Get_start_point(spiral);
	}
	get_point(p1,xval[1],yval[1]);
	get_point(p2,xval[2],yval[2]);

	Null(xval[3]);	Null(yval[3]);
	Null(xval[4]);	Null(yval[4]);
	Null(xval[5]);	Null(yval[5]);
	Null(xval[6]);	Null(yval[6]);

	length[2] = Get_length(spiral);
	length[1] = Get_radius(spiral);
	Null(length[3]);
	Null(length[4]);

	curve_type = 0;

	return 0;
}
#endif

Integer get_segment_spiral_data(Element &horiz, Integer i, Integer &curve_type, Real xval[], Real yval[], Real length[]){
	Segment seg;
	Get_segment(horiz,i,seg);

	Spiral spiral;	Get_spiral(seg,spiral);
	Point p1,p2;

	if(Get_leading(spiral)){
		p1 = Get_start_point(spiral);
		p2 = Get_end_point(spiral);
	}else{
		p1 = Get_end_point(spiral);
		p2 = Get_start_point(spiral);
	}
	get_point(p1,xval[1],yval[1]);
	get_point(p2,xval[2],yval[2]);

	Null(xval[3]);	Null(yval[3]);
	Null(xval[4]);	Null(yval[4]);
	Null(xval[5]);	Null(yval[5]);
	Null(xval[6]);	Null(yval[6]);

	length[2] = Get_length(spiral);
	length[1] = Get_radius(spiral);
	Null(length[3]);
	Null(length[4]);

	curve_type = 0;

	return 0;
}

Integer get_segment_line_data(Element &horiz, Integer i, Integer &curve_type, Real xval[], Real yval[], Real length[]){
	Segment seg;
	Get_segment(horiz,i,seg);

	Line line; Get_line(seg,line);
	Point p1=Get_start(line),p2=Get_end(line);
	get_point(p1,xval[1],yval[1]);
	get_point(p2,xval[2],yval[2]);

	Null(xval[3]);	Null(yval[3]);
	Null(xval[4]);	Null(yval[4]);

	xval[5] = xval[1];	yval[5] = yval[1];
	xval[6] = xval[2];	yval[6] = yval[2];

	Null(length[1]);
	Get_length(seg,length[2]);
	Null(length[3]);
	Null(length[4]);

	curve_type = 0;

	return 0;
}
/*
Real deflection_dir_for_ips(Real dir1, Real dir2){
	if(dir2 > dir1){
		return (dir1 + Two_pi()-dir2);
	}else if(dir2 < dir 1){
		dir2 - dir1;
	}
	return 0.0;

}
*/

Integer get_segment_arc_data(Element &horiz, Integer i, Integer &curve_type, Real xval[], Real yval[], Real length[], Real &deflection_angle){
/*
	NOTE : THERE IS A DIFFERENT ORDER THAN 12D's GET_HIP_INFO
	BASICALLY 1 <=> 5, 2 <=> 6.
	THIS WORKS BETTER FOR SA
	xval,yval
		1 = left tangent (TC SC
		2 = right tangent (CS CT)
		3 = centre
		4 = hip
		5 = TS,TC
		6 = ST,CT

	lengths
		1 = radius
		2 = length
		3 = left spiral length
		4 = right spiral length
	curve_spirals

//     type = 0  straight	(should not be 0 ever)
//          1  Curve only
//          2  LH Spiral only
//          3  LH spiral and curve
//          4  RH spiral only
//          5  curve, RH spiral
//          6  LH spiral, RH spiral
//          7  LH spiral, curve, RH spiral

*/
	Segment seg, seg_p, seg_n;
	Get_segment(horiz,i,seg);
	if(Get_type(seg) == 3){	//make sure segment is an arc
		Arc arc;	Get_arc(seg,arc);
		Point arc_ps; Get_start(seg,arc_ps);		get_point(arc_ps,xval[1],yval[1]);	get_point(arc_ps,xval[5],yval[5]);
		Point arc_pe; Get_end  (seg,arc_pe);		get_point(arc_pe,xval[2],yval[2]);	get_point(arc_pe,xval[6],yval[6]);
		Point arc_pc	=	Get_centre(arc);	get_point(arc_pc,xval[3],yval[3]);
		length[1] = Get_radius(arc);
		Integer curve = 0;	//is there a curve
		if(length[1] > 1.0e-6)curve=1;
		Get_length(seg,length[2]);
		Null(length[3]);	Null(length[4]);
		//LEADING SPIRAL
		Integer left_spiral = 0;
		if(!Get_segment(horiz,i-1,seg_p)){
			if(Get_type(seg_p) == 4){	//is the previous segment a spiral?
				Spiral spiral;	Get_spiral(seg_p,spiral);
				if(Get_leading(spiral)){	//is this a leading spiral?
					Point spiral_pe = Get_end_point(spiral);
					Point spiral_ps = Get_start_point(spiral);	get_point(spiral_ps,xval[5],yval[5]);
					length[3] = Get_length(spiral);
					left_spiral = 1;
				}

			}
		}
		//TRAILING SPIRAL - for a trailing spiral the segment is kind of reversed, the start of the sprial touches the straight, the end of the spiral touches the curve
		Integer right_spiral = 0;
		if(!Get_segment(horiz,i+1,seg_n)){
			if(Get_type(seg_n) == 4){	//is the next segment a spiral?
				Spiral spiral;	Get_spiral(seg_n,spiral);
				if(!Get_leading(spiral)){	//is this a trailing spiral? (not a leading spiral)
					Point spiral_pe = Get_end_point(spiral);
					Point spiral_ps = Get_start_point(spiral);	get_point(spiral_ps,xval[6],yval[6]);
					length[4] = Get_length(spiral);
					right_spiral = 1;
				}
			}
		}
		//CALC IP
		Real dir1, dir2;
		Real tmp_x,tmp_y,tmp_z,tmp_ch,tmp_off;
		Drop_point(horiz,xval[5],yval[5],tmp_z,tmp_x,tmp_y,tmp_z,tmp_ch,dir1,tmp_off);
		Drop_point(horiz,xval[6],yval[6],tmp_z,tmp_x,tmp_y,tmp_z,tmp_ch,dir2,tmp_off);
		deflection_angle = Absolute(dir2-dir1);
		Print("DEFLECTION ANGLE:"+To_text(deflection_angle,3)+"\n");
		dir2+=Pi();
		get_dir_intersect(xval[5], yval[5], dir1, xval[6], yval[6], dir2, xval[4], yval[4]);	//there is a possibility that the ip cannot be constructed here!

		curve_type = curve + 2*left_spiral + 4*right_spiral;


		return 0;
	}else{
		return 1;
	}

	return 2;
}

Integer get_hg_info(Element &sa, Integer iseg, Integer &segtype, Integer &curve_type, Real xval[], Real yval[], Real zval[], Real ch[], Real dir[], Text tangent_type[],Real length[], Real &deflection_angle){
/*
	segtype
		1 = ip		(non sensical)
		2 = straight
		3 = curve
		4 = spiral
		5 = parabola	(non sensical)

	xval,yval,ch,dir
		1 = left
		2 = right
		3 = centre
		4 = hip
		5 = TS
		6 = ST

	tangent type
		1 = LHS
		2 = RHS

	lengths
		1 = radius
		2 = length
		3 = left spiral length
		4 = right spiral length

//     type = 0  straight	(shouldn't ever be 0) , type really only makes sense for curves
//          1  Curve only
//          2  LH Spiral only
//          3  LH spiral and curve
//          4  RH spiral only
//          5  curve, RH spiral
//          6  LH spiral, RH spiral
//          7  LH spiral, curve, RH spiral


*/
	Null(deflection_angle);
	Element e;
	Text element_type;
	Get_type(sa,element_type);
	if(element_type == "Super"){
		e = sa;
	}else if (element_type == "Super_Alignment"){
		e = Get_super_alignment_horizontal_string(sa);
	}else{
		return 1; //bad string type
	}

	Integer nsegs;	Get_segments(e,nsegs);
	Segment seg,seg_n, seg_p;
	if(Get_segment(e,iseg,seg))
		return 2; //could not get the segment iseg!

	//these are for the type of tangents, if seg_n or seg_p does not exist it will just fill out the part type for seg
	Get_segment(e,iseg-1,seg_p);
	Get_segment(e,iseg+1,seg_n);
	tangent_type[1] = get_tanget_type(seg_p,seg);
	tangent_type[2] = get_tanget_type(seg,seg_n);

	segtype = Get_type(seg);

	get_start(seg,xval[1],yval[1]);
	get_end  (seg,xval[2],yval[2]);

	for(Integer i=3;i<=6;i++){
		Null(xval[i]);Null(yval[i]);Null(ch[i]);
	}


	switch( segtype ) {
		case 1 :{ //ip
			Print("Segment type <"+To_text(segtype)+"> not allowed : point");Print();
		}break;
		case 2 :{ //line

			get_segment_line_data(e,iseg, curve_type, xval, yval, length);

		}break;
		case 3 :{ //arc
 			/*
 			Real z;
 			Arc arc;	Get_arc(seg,arc);
 			get_arc_data  (arc,xval[3],yval[3],length[1]);
 			Get_length(seg,length[2]);
 			//need to calculate the IP point!
 			get_ip(e, iseg, xval[4],yval[4]);
 			*/
 			//Integer curve_type;
 			get_segment_arc_data(e,iseg, curve_type, xval, yval, length, deflection_angle);

		}break;
		case 4 :{ //spiral (v9, depreciated in v10)

			get_segment_spiral_data(e,iseg, curve_type, xval, yval, length);

			Spiral spiral;	Get_spiral(seg,spiral);
			Point p1,p2;

			if(Get_leading(spiral)){
				p1 = Get_start_point(spiral);
				p2 = Get_end_point(spiral);
			}else{
				p1 = Get_end_point(spiral);
				p2 = Get_start_point(spiral);
			}
			get_point(p1,xval[1],yval[1]);
			get_point(p2,xval[2],yval[2]);

			length[2] = Get_length(spiral);
			length[1] = Get_radius(spiral);

		}break;
		case 5 :{ //parabola
			Print("Segment type <"+To_text(segtype)+"> not allowed : parabola");Print();
		}break;
		case 6 :{ // curve (spiral v10)
#if VERSION_4D >= 1000
			get_segment_curve_data(e,iseg, curve_type, xval, yval, length);

			//Spiral spiral;	Get_spiral(seg,spiral); // rem for v10, which uses Curve see http://beta.12dmodel.com/forums/viewtopic.php?t=4020
			Curve spiral;	Get_curve(seg,spiral);
			Point p1,p2;

			if(Get_leading(spiral)){
				p1 = Get_start_point(spiral);
				p2 = Get_end_point(spiral);
			}else{
				p1 = Get_end_point(spiral);
				p2 = Get_start_point(spiral);
			}
			get_point(p1,xval[1],yval[1]);
			get_point(p2,xval[2],yval[2]);

			length[2] = Get_length(spiral);
			length[1] = Get_radius(spiral);
#else
			Show_console(1);
			Print("NOTE: This macro needs to be recompiled for version 10 (http://beta.12dmodel.com/forums/viewtopic.php?t=4020)\n");
#endif
		}break;
		default :{
			Print("Unknown segment type <"+To_text(segtype)+">");Print();
		}break;
	}

	Real tmp_z,tmp_x,tmp_y,tmp_z2,tmp_off;

	for(i=1;i<=6;i++){
		Drop_point(sa,xval[i],yval[i],tmp_z,tmp_x,tmp_y,zval[i],ch[i],dir[i],tmp_off);
	}

	if (element_type == "Super Alignment"){	//if we extracted the hg string then clean up
		Element_delete(e);
	}

	return 0;
}


Integer get_sa_horiz_report_full(Element &sa,
				Integer columns,
				Integer chainage_type_column,	// THIS IS NEW !
				Integer chainage_column, Integer chainage_decimals, Integer chainage_prefix, Integer chainage_prefix_space,
				Integer x_column, Integer x_decimals,
				Integer y_column, Integer y_decimals,
				Integer z_column, Integer z_decimals,	// THIS IS NEW !
				Integer radius_column, Integer radius_decimals,
				Integer bearing_column, Integer bearing_decimals,
				Integer length_column, Integer length_decimals,
				Integer ip_deflection_column, Integer ip_deflection_decimals,	// THIS IS NEW !
				Integer count_column,
				Integer do_curve_ips,
				Dynamic_Text &report){


	Text element_type;	Get_type(sa,element_type);
	if(element_type!="Super_Alignment")
		return 1;

	Element horiz = Get_super_alignment_horizontal_string(sa);
	Integer nsegs;	Get_segments(horiz,nsegs);

	Integer tangent_count = 0;

	for(Integer count=1;count<=(nsegs+1);count++){

		//this provides a method to get the last point of the last segment
		Integer iseg = count;
		Integer pos = 1;
		if(count >nsegs){
			pos = 2;
			iseg = count-1;
		}

		//define some arrays
		Integer segtype, type;
		Real xval[6], yval[6], zval[6], ch[6], dir[6];
		Text tangent_type[2];
		Real length[4];
		Real ip_deflection_angle;
		//if( get_hg_info(horiz, iseg, segtype, type, xval, yval, zval, ch, dir, tangent_type, length) ){
		if( get_hg_info(sa, iseg, segtype, type, xval, yval, zval, ch, dir, tangent_type, length,ip_deflection_angle) ){
			//error!
		}

		Text t_type[2];		t_type[1]="";		t_type[2]="";
		Text t_chainage[2]; 	t_chainage[1]= "";	t_chainage[2]= "";
		Text t_x[2]; 		t_x[1]= "";		t_x[2]= "";
		Text t_y[2]; 		t_y[1]= "";		t_y[2]= "";
		Text t_z[2]; 		t_z[1]= "";		t_z[2]= "";
		Text t_radius[2]; 	t_radius[1]= "";	t_radius[2]= "";
		Text t_bearing[2]; 	t_bearing[1]= "";	t_bearing[2]= "";
		Text t_length[2]; 	t_length[1] = "";	t_length[2] = "";
		Text t_count[2];	t_count[1] = "";	t_count[2] = "";
		Text t_deflection[2];	t_deflection[1] = "";	t_deflection[2] = "";


		//Integer ii = 1;
		for(Integer ii=1;ii<=2;ii++){
			Integer pos_old = pos;
			//if(segtype != 3)continue; // if it's not an arc then dont worry
			if( (ii == 2) && (!do_curve_ips) )continue;	//skip ii = 2 if we arn't getting IP's
			tangent_count++;
			t_count[ii] = To_text(tangent_count);
			Text tangent_prefix = tangent_type[pos];	//this needs to go here before we re-position the location of pos

			if( (ii == 2) && (segtype == 3) && (do_curve_ips != 0) ){
				pos = 4;				//this breaks the routine!!!!
				tangent_prefix = "IP";
			}
			if(chainage_type_column) t_type[ii] += tangent_prefix;
			if(chainage_column){
				if(chainage_prefix)t_chainage[ii] += tangent_prefix;
				if(chainage_prefix_space)t_chainage[ii] += " ";
				t_chainage[ii] += To_text(ch[pos],chainage_decimals);
			}
			if(x_column)	t_x[ii] += To_text(xval[pos],x_decimals);
			if(y_column)	t_y[ii] += To_text(yval[pos],y_decimals);
			if(z_column){
				if(Is_null(zval[pos]))t_z[ii]="-";
				else t_z[ii] += To_text(zval[pos],z_decimals);
			}
			if(segtype != 2) {if(radius_column)	t_radius[ii]  += To_text(length[1],radius_decimals);	}
			if(bearing_column){
				Integer dd,mm;	Real ss;
				Degrees_to_dms(radians_to_bearing(dir[pos]),dd,mm,ss);
				t_bearing[ii] += To_text(dd)+ Degrees_text + To_text(mm)+"'" + To_text(ss,bearing_decimals) + "\"";
			}
			if(ip_deflection_column && ii == 2){
				Integer dd,mm;	Real ss;
				Real decimal_degrees;
				Print("ip_deflection_angle"+To_text(ip_deflection_angle,3)+"\n");
				Radians_to_degrees(ip_deflection_angle,decimal_degrees);
				Degrees_to_dms(decimal_degrees,dd,mm,ss);
				t_deflection[ii] += To_text(dd)+ Degrees_text + To_text(mm)+"'" + To_text(ss,ip_deflection_decimals) + "\"";
			}
			if(segtype != 2) {if(length_column)	t_length[ii]  += To_text(length[2],length_decimals);}
			pos = pos_old;
		}
		/*
		Print("ROW 1:" + t_chainage[1] + " " +t_x[1]  + " " +t_y[1]  + " " + t_radius[1]  + " " +t_bearing[1]  + " " +t_length[1]);Print();
		Print("ROW 2:" + t_chainage[2] + " " +t_x[2]  + " " +t_y[2]  + " " + t_radius[2]  + " " +t_bearing[2]  + " " +t_length[2]);Print();
		*/
		switch( segtype ) {
			case 1 :{ //ip
				Print("Segment type <"+To_text(segtype)+"> not allowed : point");Print();
			}break;
			case 2 :{ //line

			}break;
			case 3 :{ //arc

			}break;
			case 4 :{ //spiral v9

			}break;
			case 5 :{ //parabola
				Print("Segment type <"+To_text(segtype)+"> not allowed : parabola");Print();
			}break;
			case 6 :{ //spiral v10

			}break;
			default :{
				Print("Unknown segment type <"+To_text(segtype)+">");Print();
			}break;
		}

		Text row[2]; row[1] = ""; row[2] = "";

		for(Integer jj=1;jj<=2;jj++){		//jj is for row[1] and row[2]
			for(Integer j=1;j<=columns;j++){
				if(j!=1)row[jj]+="\t";

				if(j==chainage_type_column)row[jj]+=t_type[jj];
				else if(j==chainage_column)row[jj]+=t_chainage[jj];
				else if(j==x_column)row[jj]+=t_x[jj];
				else if(j==y_column)row[jj]+=t_y[jj];
				else if(j==z_column)row[jj]+=t_z[jj];
				else if(j==radius_column)row[jj]+=t_radius[jj];
				else if(j==bearing_column)row[jj]+=t_bearing[jj];
				else if(j==length_column)row[jj]+=t_length[jj];
				else if(j==count_column)row[jj]+=t_count[jj];
				else if(j==ip_deflection_column)row[jj]+=t_deflection[jj];

			}
		}
		Append(row[1],report);			//always add this row

		if(segtype == 3 && do_curve_ips!=0 ){		//add the ip row?
			Append(row[2],report);
		}else{
			tangent_count--;
		}

	}
	Element_delete(horiz);
	return 0;
}

Integer get_sa_horiz_report(Element &sa,Dynamic_Text &report){
	Integer columns = 9;
	Integer chainage_type_column = 0;
	Integer chainage_column = 1, chainage_decimals = 3, chainage_prefix = 1, chainage_prefix_space = 1;
	Integer x_column = 2, x_decimals = 3;
	Integer y_column = 3, y_decimals = 3;
	Integer z_column = 4, z_decimals = 3;
	Integer radius_column = 5, radius_decimals = 3;
	Integer bearing_column = 6, bearing_decimals = 3;
	Integer length_column = 7, length_decimals = 3;
	Integer ip_deflection_column = 8,  ip_deflection_decimals = 3;
	Integer count_column = 9;
	Integer do_curve_ips = 1;
	Integer ok = get_sa_horiz_report_full(sa,
				columns,
				chainage_type_column,
				chainage_column, chainage_decimals, chainage_prefix, chainage_prefix_space,
				x_column, x_decimals,
				y_column, y_decimals,
				z_column, z_decimals,
				radius_column, radius_decimals,
				bearing_column, bearing_decimals,
				length_column, length_decimals,
				ip_deflection_column, ip_deflection_decimals,
				count_column,
				do_curve_ips,
				report);
	return ok;
}

#endif

#if 0

#include "mashy_lib_get_user_input.H"

void main(){
	Clear_console();
	Dynamic_Element delt;
	get_source_strings_panel("Panel", "Process" , delt);

	Integer items;
	Get_number_of_items(delt,items);

	for(Integer j=1;j<=items;j++){
		Element e;
		Get_item(delt,j,e);
		Integer nsegs;	Get_segments(e,nsegs);
		for(Integer i=1;i<=nsegs;i++){

			Real xval[6],yval[6],ch[6],dir[6];
			Text tangent_type[2];
			Real length[4];
			Integer segtype,segtype2;

			Print("ierr=" + To_text(get_hg_info(e, i, segtype, segtype2,xval, yval, ch, dir, tangent_type, length)));Print();;

			Print(tangent_type[1] + To_text(ch[1]) + " " +To_text(xval[1]) + " " +To_text(yval[1]) + " " +To_text(dir[1]) + " ");Print();
			if(segtype == 3){
				Print(" IP DATA " + To_text(ch[4]) + " " +To_text(xval[4]) + " " +To_text(yval[4]) + " " +To_text(dir[4]) + " ");Print();
			}
			Print(tangent_type[2] + To_text(ch[2]) + " " +To_text(xval[2]) + " " +To_text(yval[2]) + " " +To_text(dir[2]) + " ");Print();
		}
	}

}




void get_hip_info(Element align,Integer hip,Integer &type,
					Real xval[],Real yval[],Real lengths[],Real &deflection,Real ch[])
// ----------------------------------------------------------------------------
//  Get the horizontal info for an horizontal ip
//
//      - the co-ordinates of the special points
//      - the curve radius and curve length
//      - the left and right spiral lengths
//
//
//  Type of HIP is returned as type where
//
//     type = 0  HIP only
//          1  Curve only
//          2  LH Spiral only
//          3  LH spiral and curve
//          4  RH spiral only
//          5  curve, RH spiral
//          6  LH spiral, RH spiral
//          7  LH spiral, curve, RH spiral
//
//   Co-ordinates of special points returned in xval[1...6],yval[1...6]
//   where the array position gives
//
//        position  1  LH tangent, TS or TC
//                  2  RH tangent, ST or CT
//                  3  curve centre
//                  4  SC
//                  5  CS
//                  6  HIP
//
//    NOTE -
//
//       If the IP is an HIP only, 1-5 are all given the HIP co-ords.
//       If the IP has a curve and no spirals, 1 is set equal to 4 (TC=SC),
//           and 2 is set equal to 5 (CT=CS).
//
//
//
//    The curve radius, curve and spiral lengths are returned in
//     the array lengths[1...4]
//
//        position  1  circle radius
//                  2  circle length
//                  3  left spiral length
//                  4  right spiral length
//
// ----------------------------------------------------------------------------
{
	Text hip_type;
	Integer ret;

	ret = Get_hip_type(align,hip,hip_type);

	// Get the co-ordinates of the special points for the HIP

	if(hip_type == "IP") {

		//  case of HIP only with no curve or spiral

		Real xip,yip;  ret = Get_hip_geom(align,hip,0,xip,yip);

		xval[6] = xip; yval[6] = yip;

		type = 0;

		// fill in other array positions - set them all to the HIP position

		xval[1] = xip; yval[1] = yip;
		xval[2] = xip; yval[2] = yip;
		xval[3] = xip; yval[3] = yip;
		xval[4] = xip; yval[4] = yip;
		xval[5] = xip; yval[5] = yip;

	} else if(hip_type == "Curve") {

		//  case of HIP with and curve and no spirals

		Real xip,yip;  ret = Get_hip_geom(align,hip,0,xip,yip);
		Real xtc,ytc;  ret = Get_hip_geom(align,hip,1,xtc,ytc);
		Real xct,yct;  ret = Get_hip_geom(align,hip,2,xct,yct);
		Real xcc,ycc;  ret = Get_hip_geom(align,hip,3,xcc,ycc);

		xval[1] = xtc; yval[1] = ytc;
		xval[2] = xct; yval[2] = yct;
		xval[3] = xcc; yval[3] = ycc;
		xval[6] = xip; yval[6] = yip;

		type = 1;

		// fill in the other array positions

		xval[4] = xtc; yval[4] = ytc;
		xval[5] = xct; yval[5] = yct;

	} else if(hip_type == "Spiral") {

		Real xip,yip;  ret = Get_hip_geom(align,hip,0,xip,yip);
		Real xts,yts;  ret = Get_hip_geom(align,hip,1,xts,yts);
		Real xsc,ysc;  ret = Get_hip_geom(align,hip,4,xsc,ysc);
		Real xcs,ycs;  ret = Get_hip_geom(align,hip,5,xcs,ycs);
		Real xst,yst;  ret = Get_hip_geom(align,hip,2,xst,yst);
		Real xcc,ycc;  ret = Get_hip_geom(align,hip,3,xcc,ycc);

		Integer left_spiral  = ((xts != xsc) || (yts != ysc)) ? 1 : 0;
		Integer right_spiral = ((xst != xcs) || (yst != ycs)) ? 1 : 0;
		Integer curve        = ((xsc != xcs) || (ysc != ycs)) ? 1 : 0;

		xval[1] = xts; yval[1] = yts;
		xval[2] = xst; yval[2] = yst;
		xval[3] = xcc; yval[3] = ycc;
		xval[4] = xsc; yval[4] = ysc;
		xval[5] = xcs; yval[5] = ycs;
		xval[6] = xip; yval[6] = yip;

		type = curve + 2*left_spiral + 4*right_spiral;
	}

	//  Get chainages for all points

	Real tmp_z,tmp_x,tmp_y,tmp_z2,tmp_off,tmp_dir;

	for(Integer num=1;num<=6;num++){
		Drop_point(align,xval[num],yval[num],tmp_z,tmp_x,tmp_y,tmp_z2,ch[num],tmp_dir,tmp_off);
	}

	//  Get the curve radius, curve and spiral lengths

	Real x,y,radius,left_spiral,right_spiral;

	Get_hip_data(align,hip,x,y,radius,left_spiral,right_spiral);

	Real ch1,ch2,xf,yf,zf,dir,off;  // to get curve length

	if(radius != 0) {
		Drop_point(align,xval[4],yval[4],0.0,xf,yf,zf,ch1,dir,off);
		Drop_point(align,xval[5],yval[5],0.0,xf,yf,zf,ch2,dir,off);
		lengths[2] = ch2 - ch1;
	} else {
		lengths[2] = 0.0;
	}

	lengths[1] = radius;
	lengths[3] = left_spiral;
	lengths[4] = right_spiral;

	return;
}

#endif