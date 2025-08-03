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

Integer get_inverse_colour(Integer colour){ // Tested
	// 0xFFFFFF == 255 << 16 | 255 << 8 | 255
	if(colour & (1 << 31)){ //
		//     (3 x 255) - (r,g,b),  then put back the significant bit
		return ((0xFFFFFF - (colour & 0xFFFFFF) ) | (1 << 31) );
	}
	Integer r,g,b;
	Convert_colour(colour,r,g,b);
	return ( (0xFFFFFF - ( (r << 16) | (g << 8) | b  ) ) | (1 << 31) );
}

Integer get_grayscale(Integer colour) {
    Integer r, g, b;
	Convert_colour(colour,r,g,b);
    Integer gray = (0.299 * r + 0.587 * g + 0.114 * b);
	return (gray << 16) | (gray << 8) | gray | (1 << 31);
}

Real clamp(Real value, Real min, Real max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

Integer get_brighter(Integer colour, Real factor) { // 0 dull, 2 - 3 to get brighter // TODO - test
    Integer r, g, b;
	Convert_colour(colour,r,g,b);
    r = clamp( r * factor, 0., 255.);
	g = clamp( g * factor, 0., 255.);
	b = clamp( b * factor, 0., 255.);
    return (r << 16) | (g << 8) | b | (1 << 31);
}

Integer get_saturated(Integer colour, Real factor) {
    Integer r, g, b;
	Convert_colour(colour,r,g,b);

    Real avg = (r + g + b) / 3.0;
	r = clamp( avg + factor * (r - avg), 0., 255.);
	g = clamp( avg + factor * (g - avg), 0., 255.);
	b = clamp( avg + factor * (b - avg), 0., 255.);
    return (r << 16) | (g << 8) | b | (1 << 31);
}

Integer get_adjusted_contrast(Integer colour, Real factor) {
    Integer r, g, b;
	Convert_colour(colour,r,g,b);

    Real avg = (r + g + b) / 3.0;

    r = clamp( ( factor * (r - avg) + avg) , 0., 255.);
	g = clamp( ( factor * (g - avg) + avg) , 0., 255.);
	b = clamp( ( factor * (b - avg) + avg) , 0., 255.);
    return (r << 16) | (g << 8) | b | (1 << 31);	
}

void lerp_color(Integer r1, Integer g1, Integer b1, Integer r2, Integer g2, Integer b2, Real factor, Integer &r, Integer &g, Integer &b) {
    // clamp factor between 0.0 and 1.0
    if (factor < 0. ) factor = 0. ;
    if (factor > 1. ) factor = 1. ;
    r = (r1 * (1. - factor) + r2 * factor);
    g = (g1 * (1. - factor) + g2 * factor);
    b = (b1 * (1. - factor) + b2 * factor);
}


#endif