#ifndef mashy_lib_colours_h_included
#define mashy_lib_colours_h_included

Text rgb_to_hex(Integer rgb) {
	Text chars = "0123456789ABCDEF";
	Integer c1 = Floor(rgb/16)+1;
	Integer c2 = rgb - (Floor(rgb/16)*16)+1;
	return ( Get_subtext(chars,c1,c1) + Get_subtext(chars,c2,c2) );
}
Text rgb_to_hex(Integer r, Integer g, Integer b){
	return( rgb_to_hex(r) + rgb_to_hex(g) + rgb_to_hex(b) );
}

Text colour_to_hex(Integer c){
	Integer r,g,b;
	Convert_colour(c,r,g,b);
	return ( rgb_to_hex(r,g,b) );
}


Integer convert_colour(Text name){
	Integer value;
	if(Convert_colour(name,value)){
		value = 1;
	}
	return (value);
}

Integer get_inverse_rgb(Integer c, Integer &r,Integer &g, Integer &b){
	Integer ierr = Convert_colour(c,r,g,b);
	r=255-r;	g=255-g;	b=255-b;
	return ierr;
}

void get_inverse_rgb(Integer rr, Integer gg, Integer bb, Integer &r,Integer &g, Integer &b){
	r=255-rr;	g=255-gg;	b=255-bb;
}

#endif