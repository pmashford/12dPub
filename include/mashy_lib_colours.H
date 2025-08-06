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

Integer get_saturated(Integer colour, Real factor) { // this will factor the colour saturation
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

void rgb_to_hsv(Integer r, Integer g, Integer b, Real &h, Real &s, Real &v) { // h = 0-360, s = 0-1, v = 0-1
	Real min = r < g ? (r < b ? r : b) : (g < b ? g : b);
	Real max = r > g ? (r > b ? r : b) : (g > b ? g : b);

	v = max / 255.0;
	if (max == 0.) {
		s = 0.;
		h = 0.;
		return;
	}
	s = (max - min) / max;
	if(s == 0.){
		h = 0.;
		return;
	}
	if(max == r){
		h = (g - b) / (max - min);
	}else if(max == g){
		h = 2.0 + (b - r) / (max - min);
	}else{
		h = 4.0 + (r - g) / (max - min);
	}
	h *= 60.0;
	if (h < 0.) h += 360.0;
}

void hsv_to_rgb(Real h, Real s, Real v, Integer &r, Integer &g, Integer &b) {
	if (s == 0.) {
		r = g = b = v * 255.0;
		return;
	}
	h /= 60.0;
	Integer i = Floor(h);
	Real f = h - i;
	Real pee = v * (1.0 - s);
	Real q = v * (1.0 - f * s);
	Real t = v * (1.0 - (1.0 - f) * s);
	switch (i) {
		case 0:  {r = v * 255.0; g = t * 255.0; b = pee * 255.0; }break;
		case 1:  {r = q * 255.0; g = v * 255.0; b = pee * 255.0;} break;
		case 2:  {r = pee * 255.0; g = v * 255.0; b = t * 255.0;} break;
		case 3:  {r = pee * 255.0; g = q * 255.0; b = v * 255.0;} break;
		case 4:  {r = t * 255.0; g = pee * 255.0; b = v * 255.0;} break;
		default: {r = v * 255.0; g = pee * 255.0; b = q * 255.0;} break;
	}
}

Integer_Integer_Map create_colour_book(Integer r1, Integer g1, Integer b1, Integer r2, Integer g2, Integer b2, Integer count_must_be_even) { // create a colour book with 10 colours between two colours
	// note that count_must_be_even should be even, otherwise the colours will not be evenly distributed
	// the key will be from 0 -> count-1

	Real h1, s1, v1;
	Real h2, s2, v2;
	rgb_to_hsv(r1, g1, b1, h1, s1, v1);
	rgb_to_hsv(r2, g2, b2, h2, s2, v2);

	Integer_Integer_Map colour_book;
	Real steps = 2.0 / count_must_be_even;
	for (Integer i = 0; i < count_must_be_even / 2; i++) { // walkboth ways, each colour should fade to the middle white-ish
		Real factor = (i * steps) / count_must_be_even;
		hsv_to_rgb(h1 , s1 * factor, v1 , r1, g1, b1);
		hsv_to_rgb(h2 , s2 * factor, v2 , r2, g2, b2);
		Container_insert_key(colour_book, i, (r1 << 16) | (g1 << 8) | b1 | (1 << 31) );
		Container_insert_key(colour_book, count_must_be_even - i - 1, (r2 << 16) | (g2 << 8) | b2 | (1 << 31) );
	}
	return colour_book;
}

void get_rgb_from_colour_book(Integer_Integer_Map &colour_book, Integer count_must_be_even, Real low, Real high, Real value, Integer &r, Integer &g, Integer &b) {
	count_must_be_even--; // get in sync with the call above
	if(value < low ) value = low;
	if(value > high) value = high;
	Real factor = (value - low) / (high - low);
	Integer lookup = Floor(factor * count_must_be_even);
	Integer colour = 0;
	Container_get_value(colour_book, lookup, colour);
	if(colour == 0) {
		r = 0;	g = 0;	b = 0;	return;
	}
	Convert_colour(colour,r,g,b);
}


#endif