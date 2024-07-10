#ifndef mashy_lib_super_alignment_info_h_included
#define mashy_lib_super_alignment_info_h_included

#include "mashy_lib_segments.H"
#include "mashy_lib_geometry.H"
#include "mashy_lib_element_functions.H"

Text get_point_chainage_text(Integer seg_left, Integer seg_right){
// construct a bit of text
// eg: TC,TS,SC,IP,CS,ST,CT
	Text value;
	Integer values[2];
	values[1] = seg_left;	values[2] = seg_right;
	for(Integer i = 1; i <= 2; i++){
		switch(values[i]){
			case(0):{
			}break;
			case(1):{
				value += "IP";
			}break;
			case(2):{
				value += "T";
			}break;
			case(3):{
				value += "C";
			}break;
			case(4):{
				value += "S";
			}break;
			case(5):{
				value += "P";
			}break;
			default:{
				Print("Error, unknown segment type <" + To_text(values[i]) + "\n");
			}break;

		}
	}
	if(seg_left == 2 && seg_right == 2){
		return "IP";
	}else if(seg_left == 0 || seg_right == 0){
		if(seg_left == 2 || seg_right == 2){
			return "IP";
		}
	}
	return value;

}


// note works for horizontal parts and vertical parts.  for vertical info back is in section: that is x=ch and y=ht
void get_sa_segment_info(Element &align, Element &sa_string,Integer seg_no,Integer &type, Real xval[], Real yval[],Real lengths[],Real &deflection,Real ch[],Text ch_types[]){
/*
	coordinates
		1 LH tangent
		2 RH tangent
		3 curve centre (for curves)
		4 sag or crest (for parabolas)
		6 HIP (for curves) IP (for parabolas)

	lengths
	position
		1  circle/spiral radius
		2  circle/spiral/straight length
		3  left spiral length
		4  right spiral length
		5  bearing at LH
		6  bearing at RH

	chainages (type) // note chainages only valid for HG, incorrect to VG :##> for vertical the chainage is the xval[1] and xval[2] <##:
		1 start
		2 end
get the next part and work out what type of text
*/

	Segment seg;	Get_segment(sa_string,seg_no,seg);
	type = Get_type(seg);
	Point p1, p2;
	Get_start(seg,p1);	Get_end(seg,p2);
	xval[1] = Get_x(p1);	yval[1] = Get_y(p1);
	xval[2] = Get_x(p2);	yval[2] = Get_y(p2);


	switch(type){
		case(1):{//point

		}break;
		case(2):{//line
			lengths[2] =	Get_distance(p1,p2);
		}break;
		case(3):{//arc
			Arc arc;	Get_arc  (seg,arc);
 			lengths[1] =    Get_radius      (arc);
 					Get_length      (arc,lengths[2]);	//need to check this
			Point pnt  =	Get_centre(arc);
 			xval[3]    =	Get_x(pnt);
 			yval[3]    =	Get_y(pnt);
		}break;
		case(4):{//spiral
			Spiral spiral;	Get_spiral  (seg,spiral);
 			lengths[1] =    Get_radius      (spiral);
 			lengths[2] =    Get_length      (spiral);
		}break;
		case(5):{//parabola
			Parabola parabola;	Get_parabola (seg,parabola);
			lengths[2] = Get_length (parabola);
			Point pi = Get_ip(parabola);
			xval[6] = Get_x(pi);
			yval[6] = Get_y(pi);
			//Print(get_full_name(align)+"\n");
			//Print("IP:,"+To_text(xval[6],3)+","+To_text(yval[6],3)+"\n");
			p1 = Get_start(parabola);
			p2 = Get_end(parabola);
			xval[1] = Get_x(p1);	yval[1] = Get_y(p1);
			xval[2] = Get_x(p2);	yval[2] = Get_y(p2);

        		//Point pi = Get_ip   (v); chip = Get_x(pi);   htip = Get_y(pi); //xval[6]
        		//Point p1 = Get_start(v); chs = Get_x(p1);   hts = Get_y(p1); //xval[1]
        		//Point p2 = Get_end  (v); che = Get_x(p2);   hte = Get_y(p2);//xval[2]

        		//Real L  = Get_length(v);//lengths[2]

			//Real g1 = ((htip - hts) / (chip - chs)) * 100;
			//Real g2 = ((hte - htip) / (che - chip)) * 100;
			Real g1 = ((yval[6] - yval[1]) / (xval[6] - xval[1])) * 100.0; // grade in
			Real g2 = ((yval[2] - yval[6]) / (xval[2] - xval[6])) * 100.0; // grade out

			//Print("yval[6]:"+To_text(yval[6],3)+", yval[1]:"+To_text(yval[1],3)+"\n");
			//Print("xval[6]:"+To_text(xval[6],3)+", xval[1]:"+To_text(xval[1],3)+"\n");
			//Print("yval[2]:"+To_text(yval[2],3)+", yval[6]:"+To_text(yval[6],3)+"\n");
			//Print("xval[2]:"+To_text(xval[2],3)+", xval[6]:"+To_text(xval[6],3)+"\n");

			//Print("g1:"+To_text(g1,3)+", g2:"+To_text(g2,3)+"\n");

//  double x0,y0,z0;  ip->get_ip_start(x0,y0,z0);
  //        double x1,y1,z1;  ip->get_ip_coord(x1,y1,z1);
   //       double x2,y2,z2;  ip->get_ip_end  (x2,y2,z2);

          	 	//Real dx = che - chs;
          	 	//Real dg = g1 - g2;
          	 	Real dx = xval[2] - xval[1];
          	 	Real dg = g1 - g2;
          	 	Real x  = dx*(g1/dg); // 100% correct

				Real y  = yval[1] + x*g1 - (x/(2.0*dx))*(x*Absolute(g2-g1));

				y = ( (g2-g1) * x * x ) / ( 200.0 * lengths[2]) * (-1.0);


				//Print("\ny:"+To_text(y,3)+"\n\n");

// calc sag/crest point


          	 	xval[4]  = xval[1] + x;
          	 	yval[4] = yval[1] + (x * (g1/100.0)) - y;

          	 	//Print("k:"+To_text(k,3)+", y:"+To_text(y,3)+"\n");
          	 	//Real y = k * (x * x);




          	 	//Real y  = hts + x*g1 - (x/(2*dx))*(x*dg);
          	 	//yval[4]  = yval[1] + x*g1/100.0 - y;// (x/(2.0*dx))*(x*dg);
          	 	//Print("***yval[4]:"+To_text(yval[4],3)+"\n");

// calc (equiv) radius TMR
				lengths[1] = (100.0 * (xval[2] - xval[1])) / ((g2 - g1 ));



			//Print("xval[4]:"+To_text(xval[4],3)+", yval[4]:"+To_text(yval[4],3)+"\n");

		}break;
		case(6):{//curve
			Curve curve;	Get_curve (seg,curve);
			lengths[1] = Get_radius(curve);
			lengths[2] = Get_length(curve);

			//Point p1,p2;

			if(Get_leading(curve)){
				p1 = Get_start_point(curve);
				p2 = Get_end_point(curve);
			}else{
				p1 = Get_end_point(curve);
				p2 = Get_start_point(curve);
			}
			get_point(p1,xval[1],yval[1]);
			get_point(p2,xval[2],yval[2]);
		}
		default:{
			Print("Error, unknown segment type <" + To_text(type) + "\n");
		}
	}

	Integer nsegs;	Get_segments(sa_string,nsegs);

	Integer seg_type_prev;
	if(seg_no > 1){
		Segment segf;	Get_segment(sa_string,seg_no-1,segf);
		seg_type_prev = Get_type(segf);
	}else{
		seg_type_prev = 0;
	}

	Integer seg_type_next;
	if(seg_no+1 <= nsegs){
		Segment segf;	Get_segment(sa_string,seg_no+1,segf);
		seg_type_next = Get_type(segf);

	}else{
		seg_type_next = 0;
	}

	ch_types[1] = get_point_chainage_text(seg_type_prev,type);
	ch_types[2] = get_point_chainage_text(type,seg_type_next);

	Real xf,yf,zf,inst_dir,off;
	Drop_point(align,xval[1],yval[1],0,xf,yf,zf,ch[1],inst_dir,off);
	lengths[5] = radians_to_bearing(inst_dir);
	Drop_point(align,xval[2],yval[2],0,xf,yf,zf,ch[2],inst_dir,off);
	lengths[6] = radians_to_bearing(inst_dir);

}

void get_sa_horiz_segment_info(Element align, Integer seg_no,Integer &type, Real xval[], Real yval[],Real lengths[],Real &deflection,Real ch[],Text ch_types[]){
	Element sa_string	=	Get_super_alignment_horizontal_string(align);
	get_sa_segment_info(align, sa_string, seg_no,type, xval, yval, lengths,deflection, ch,ch_types);
	Element_delete(sa_string);
}

void get_sa_vert_segment_info(Element align, Integer seg_no,Integer &type, Real xval[], Real yval[],Real lengths[],Real &deflection,Real ch[],Text ch_types[]){
	Element sa_string	=	Get_super_alignment_vertical_string(align);
	get_sa_segment_info(align, sa_string, seg_no,type, xval, yval, lengths,deflection, ch,ch_types);
	//BUG FIX FOR VERTICAL
	ch[1]=xval[1];
	ch[2]=xval[2];
	Element_delete(sa_string);
}


void get_sa_horiz_report(Element align,Dynamic_Text &report){
	Element sa_string	=	Get_super_alignment_horizontal_string(align);
	Integer nsegs;	Get_segments(sa_string,nsegs);
	for(Integer i = 1 ; i <= nsegs ; i++){
		Integer  type;
    		Real     xval[6],yval[6],lengths[4],deflection,ch[6];
    		Text	ch_types[6];
		get_sa_segment_info(align, sa_string, i,type, xval, yval, lengths,deflection, ch,ch_types);
		Append(
		"ch_type " + ch_types[1] + "\n"+
		"ch " + To_text(ch[1],3) + "\n"+
		"x " + To_text(xval[1],3) + "\n"+
		"y " + To_text(yval[1],3) + "\n"+
		"length " + To_text(lengths[2],3) + "\n"+
		"radius " + To_text(lengths[1],3) + ""+
		"",report);
		if(i == nsegs){
			Append(
			"ch_type " + ch_types[2] + "\n"+
			"ch " + To_text(ch[2],3) + "\n"+
			"x " + To_text(xval[2],3) + "\n"+
			"y " + To_text(yval[2],3) + "\n"+
			"length " + To_text(lengths[2],3) + "\n"+
			"radius " + To_text(lengths[1],3) + ""+
			"",report);
		}


	}
	Element_delete (sa_string);
}
Integer add_unique_chainages( Real needle, Dynamic_Real &haystack){
	Integer items;
	Get_number_of_items(haystack,items);
	for(Integer i=1;i<=items;i++){
		Real r;
		Get_item(haystack,i,r);
		if(xeqy(r,needle))return 0;
	}
	//Print("adding"+To_text(needle,3)+"\n");
	return Append(needle,haystack);
}

Integer get_critical_chainages(Element align, Integer do_vertical, Dynamic_Real &chainages){
	Element sa_string	=	Get_super_alignment_vertical_string(align);
	Integer nsegs;	Get_segments(sa_string,nsegs);
	for(Integer i = 1 ; i <= nsegs ; i++){
		Integer  type;
    		Real     xval[6],yval[6],lengths[6],deflection,ch[6];
    		Text	ch_types[6];
		get_sa_segment_info(align, sa_string, i,type, xval, yval, lengths,deflection, ch,ch_types);

		// always add the start and end chaianges
		add_unique_chainages( xval[1], chainages);
		add_unique_chainages( xval[2], chainages);

		switch(type){
			case (2) : {

			}break;
			case (5) : {
				add_unique_chainages( xval[4], chainages);
			}break;
			default : {

			}break;
		}
	}
	return 0;
}

void get_sa_vert_report(Element &align,Dynamic_Text &report){
	Element sa_string	=	Get_super_alignment_vertical_string(align);
	Integer nsegs;	Get_segments(sa_string,nsegs);
	for(Integer i = 1 ; i <= nsegs ; i++){
		Integer  type;
    		Real     xval[6],yval[6],lengths[6],deflection,ch[6];
    		Text	ch_types[6];
		get_sa_segment_info(align, sa_string, i,type, xval, yval, lengths,deflection, ch,ch_types);
		Append(
		"ch_type " + ch_types[1] + "\n"+
		"ch " + To_text(ch[1],3) + "\n"+
		"x " + To_text(xval[1],3) + "\n"+
		"y " + To_text(yval[1],3) + "\n"+
		"length " + To_text(lengths[2],3) + "\n"+
		"radius " + To_text(lengths[1],3) + ""+
		"",report);
		if(i == nsegs){
			Append(
			"ch_type " + ch_types[2] + "\n"+
			"ch " + To_text(ch[2],3) + "\n"+
			"x " + To_text(xval[2],3) + "\n"+
			"y " + To_text(yval[2],3) + "\n"+
			"length " + To_text(lengths[2],3) + "\n"+
			"radius " + To_text(lengths[1],3) + ""+
			"",report);
		}


	}
	Element_delete (sa_string);
}

#endif