#ifndef mashy_lib_chainage_H_included
#define mashy_lib_chainage_H_included

#include "12d/standard_library.H"
#include "mashy_lib_debug.H"

Integer check_chainages_within_extent(Element &e, Real start_ch, Real end_ch, Real &chs, Real &che){
// adjust the start and end chainage (chs and che) to be within the limits of the element and the 
// start_ch & end_ch
	Integer ok=0;
	ok+=Get_chainage(e,chs);
	ok+=Get_end_chainage(e,che);

	if(!Is_null(start_ch) || start_ch < chs){
		chs = start_ch;
	}

	if(!Is_null(end_ch) || end_ch > che){
		che = end_ch;
	}
	return (ok);
}

Integer get_ch_regular(Element &e, Real start_ch, Real end_ch, Real interval, Real filter_tolerance, Integer force_start_ch, Integer force_end_ch, Dynamic_Real &chainages){
// for an element e, between start and end, build a list of &chainages
// interval is the chainage interval
// filter tolerance is for removing duplicates
	Real chs,che;
	check_chainages_within_extent(e, start_ch, end_ch, chs, che);

	Real remainder = Mod(chs,interval);

	Real this_ch = chs + remainder;

	Integer count = 0;

	if(force_start_ch){
		Append(chs,chainages);
		count++;
		#if DEBUG
			printd("added",chs);
		#endif
	}

	while(this_ch<che){
		if(count <= 1){ // start may have been added if forced
			if(xeqy(this_ch,chs,filter_tolerance)){
				this_ch+=interval;
				continue;
			}
		}
		#if DEBUG
			printd("this_ch",this_ch);
		#endif
		Append(this_ch,chainages);
		this_ch+=interval;
		#if DEBUG
			printd("next is this_ch",this_ch);
		#endif
	}
	if(force_end_ch){
		if(!xeqy(this_ch,che,filter_tolerance)){
			Append(che,chainages);
		}
	}
	return 0;
}

Integer get_ch_regular(Element &e, Real interval, Dynamic_Real &chainages){
// get regular chainages
	Real start_ch, end_ch;
	Get_chainage(e,start_ch);
	Get_end_chainage(e,end_ch);
	Real filter_tolerance = 1.0e-6;
	Integer force_start_ch = 1, force_end_ch = 1;
	return get_ch_regular(e, start_ch, end_ch, interval, filter_tolerance, force_start_ch, force_end_ch, chainages);
}



#endif