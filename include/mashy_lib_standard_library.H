#ifndef mashy_lib_standard_library_h_included
#define mashy_lib_standard_library_h_included

#include "12d/standard_library.H"

Integer xeqy(Real x1, Real y1, Real z1, Real x2, Real y2, Real z2){
// NOTE: USE Xeqy()
// check if 2 points match within a tolerance
	if(xeqy(x1,x2)){
		if(xeqy(y1,y2)){
			if(xeqy(z1,z2)){
				return 1;
			}
		}
	}
	return 0;
}

Integer xeqy(Real x1, Real y1, Real x2, Real y2){
// NOTE: USE Xeqy()
// check if 2 points match within a tolerance
	if(xeqy(x1,x2)){
		if(xeqy(y1,y2)){
			return 1;
		}
	}
	return 0;
}

void standard_skm_macro_message(Text macro_name){
#ifdef TK_MACRO
	return;
#endif
	Print("_________________________________________");Print();
	Print(macro_name + " for Paul Mashford");Print();
}

void standard_mashy_macro_message(Text macro_name){
	Print("_________________________________________");Print();
	Print(macro_name + " for Paul Mashford");Print();
}



#endif