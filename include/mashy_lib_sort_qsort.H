#ifndef mashy_lib_qsort_h_included
#define mashy_lib_qsort_h_included

#include "mashy_lib_text_functions.H"

#include "12d/Qsort.h"


//put routines in here which use 12d routines to return a sorted list
//http://forums.12dmodel.com/viewtopic.php?t=2058&highlight=qsort
void qsort(Dynamic_Real &list){
	Real m;
	Integer no_items;
	Get_number_of_items(list,no_items);
	if(no_items < 1)return;
	Integer index[no_items];
	Qsort(list,index,no_items);
	//Quick_sort(no_items,index,list); ?? cant yet straight use inbuilt
	Dynamic_Real newlist;
	for (Integer i=1; i <= no_items; i++){
		Get_item(list,index[i],m);
		Append(m,newlist);
	}
	Null(list);
	list = newlist;
}

void qsort(Dynamic_Text &list){
	Text m;
	Integer no_items;
	Get_number_of_items(list,no_items);
	if(no_items < 1)return;
	Integer index[no_items];
	Qsort(list,index,no_items);
	Dynamic_Text newlist;
	for (Integer i=1; i <= no_items; i++){
		Get_item(list,index[i],m);
		Append(m,newlist);
	}
	Null(list);
	list = newlist;
}


//	----------------------------------------------------------------------------------------------------
//			qsort_text_with_number(Dynamic_Text &list)
//	----------------------------------------------------------------------------------------------------

//this sort routine does an alphbetical sort, and then it looks for groups in the alphabetcal sort and will then see if there is a number on the end and sort that
//usefull to sort a list of string names which contain chainages, or a list of models

Integer qsort_text_with_number(Dynamic_Text &list){ // ie chainages or list of model

	Integer items;
	Get_number_of_items(list,items);
	if(items == 1) return (0); //if there is only 1 item then it is already sorted!


	//use qsort 1st
	Dynamic_Text new_list;
	Integer index[items];
	Qsort(list,index,items);
	//debug_dynamic_text(dt);
	Dynamic_Text models_in_order;
	for(Integer i=1;i<=items;i++){
		Integer position;
		position = index[i];
		Text name;
		Get_item(list,position,name);
		Append(name,new_list);
	}
	Null(list);
	Append(new_list,list);
	Null(new_list);


	Text left[items];
	Text right[items];
	Real value[items];
	Integer value_included[items];

	for(i=1;i<=items;i++){
		Text t;
		Get_item(list,i,t);
		value_included[i] = 0; //initialise
		if( split_text_check_for_number_as_last_number_part(t, left[i], right[i], value[i]) )value_included[i] = 0;
		else value_included[i] = 1;
	}

	//Integer index[items];

	//Dynamic_Text new_list;

	//start sorting groups
	Integer start = 0, end =0;
	for (i=1;i<items;i++){  //NOTE < less than _ONLY_
		if( left[i] == left[i+1] && value_included[i] == 1 && value_included[i+1] == 1){ //if 2 pieces of text are in a row && they both have a valid number on the end
			if(start==0)start = i;
			end = i+1;
			//Print(left[i] + " == " +left[i+1]);Print();
			if(i != (items-1) )continue; //if we are at the last item we need to , if we are at the last item we dont continue - we need to go to the sorting routine
			//continue;
		}
		if( (start !=0 && end != 0 ) ){ //if the 2 consecutive items are not equal and we have a start and end list then we should sort this group of items in the list
			//Print(To_text(start) + " => " + To_text(end));Print();
			Dynamic_Real dreal;
			for(Integer j=start;j<=end;j++){ //pull out the
				Append(value[j],dreal);
				Text test;
				Get_item(list,j,test);
				//Print("<"+test + "> real=" +To_text(value[j],3));Print();
			}
			Integer size = end-start+1;
			Integer order[size];
			Qsort(dreal,order,size);
			for(j=1;j<=size;j++){
				Text t;
				Get_item(list, (start + order[j] - 1 ) , t);
				Append(t,new_list);

			}

			start = 0;
			end =0;
			continue;
		}

		Text t;
		Get_item(list,i,t);
		Append(t,new_list); //need to add the
		if(i==(items-1)){//may need to append last item as well
			Get_item(list,items,t);
			Append(t,new_list);
		}

	}
	Null(list);
	Append(new_list,list);
	Null(new_list);
	return(0);


}

#endif