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


// #include "mashy_lib_real_functions.H"
// check to see is a text value contains digits and or decimals
Integer is_real_for_sort(Text t, Integer idx_from, Integer &idx_to, Real &r){
	Integer length = Text_length(t);
	Integer count_decimals = 0;
	Integer count_chars = 0;
	for(Integer i = idx_from ; i <= length ; i++){
		Integer c; Get_char(t,i,c);
		if(c >= '0' && c <= '9')continue;
		if(c == '.' ){
			count_decimals++;
			if(count_decimals > 1){
				//Print("too many decimals\n");
				i--;
				break; // more than 1 decimal
			}
			continue;
		}
		if(i == idx_from){
			if (c == '+' || c == '-')continue; // is first char + or -
		}
		i--;
		break; // not a number
	}
	idx_to = i;
	if(i-idx_from >0){
		//should have a real
		From_text(Get_subtext(t,idx_from,i),r);
		return(1);
	}
	Null(r);
	return(0);
}


/// THIS IS A WORK IN PROGRESS - NOT FINISHED YET
#if 0
Integer is_less_than_or_eq_aux(Text t1, Text t2){
	Integer l1 = Text_length(t1);
	Integer l2 = Text_length(t2);
	if(l1 == 0 && l2 == 0)return(1);
	if(l1 == 0)return(1);
	if(l2 == 0)return(0);

	Integer j=0;
	Real r1,r2;
	for(Integer i=1;i<=l1;i++){
		j++;
		if(j>l2)return(0);// t2 is less than t1 based on less number of parts after i
		Text a,b;
		Get_char(t1,i,a);
		Get_char(t2,j,b);

		if(a == b)continue; // same part, keep going
		
		Integer is_r1 = is_real_for_sort(t1, i, i, r1);
		Integer is_r2 = is_real_for_sort(t2, j, j, r2);
		if(is_r1 && is_r2){ // both are numbers
			if(r1 < r2)return(1); // t1 is less than t2
			else if(r1 == r2)continue; // t1 is equal to t2, keep going as there may be more characters to check
			return(0); // t2 is greater than t1
		}
		if(is_r1 && !is_r2) return (1); // t1 is a number, t2 is not
		if(!is_r1 && is_r2) return (0); // t2 is a number, t1 is not

		return is_less_than_aux(a,b); // check if a is less than b
	}


	Integer ir1 = is_real_for_sort(t1);
	Integer ir2 = is_real(t2);
	if(ir1 && !ir2) return (1); // t1 is a number, t2 is not
	if(!ir1 && ir2) return (0); // t1 is not a number, t2 is a number
	if(ir1 && ir2){ // both are numbers
		Real r1,r2;
		Integer ierr1 = From_text(t1,r1);
		Integer ierr2 = From_text(t2,r2);
		if(ierr1 == 0 && ierr2 == 0){
			if(r1 < r2)return(1); // t1 is less than t2
			else return(0); // t1 is greater than or equal to t2
		}
	}
	// both are text
	return( t1<t2 ); // t1 is greater than or equal to t2
}


Integer is_less_than(Text t1, Text t2){
	
	Integer l1 = Text_length(t1);
	Integer l2 = Text_length(t2);
	if(l1 == 0 && l2 == 0)return(1);
	if(l1 == 0) return(1);
	if(l2 == 0) return(0);

	Dynamic_Text p1,p2;
	From_text(t1,p1);
	From_text(t2,p2);
	Integer n1,n2;
	Get_number_of_items(p1,n1);	
	Get_number_of_items(p2,n2);


	for(Integer i=1;i<=n1;i++){
		if(i>n2)return(0);// t2 is less than t1 based on less number of parts after i
		Text a,b;
		Get_item(p1,i,a);
		Get_item(p2,i,b);
		if(a == b)continue; // same part, keep going
		return is_less_than_aux(a,b); // check if a is less than b
	}
	return(1); // the continue means t1 == t2
}


void qsort_in_chuncks_partit (Dynamic_Text &list, Integer &index[], Integer &i, Integer &j, Integer left, Integer right )
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer temp;
   Integer ii;
   Integer jj;
   Integer pivot;

   ii = left;
   jj = right;
   pivot = index [(ii+jj)/2];

   while ( ii <= jj )
   {
	
		while ( is_less_than( get_item(list,index[ii]) , get_item(list,pivot) )     ) ++ii;
      while ( is_less_than( get_item(list,pivot)     , get_item(list,index[jj]) ) ) --jj;
      if ( ii <= jj )
      {
         temp      = index[ii];
         index[ii] = index[jj];
         index[jj] = temp;
         ++ii;
         --jj;
      }
   }

   i = ii;
   j = jj;

   return;
}
void qsort_in_chuncks1 (Dynamic_Text &list, Integer &index[], Integer m, Integer n)
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer i;
   Integer j;

   if ( m < n )
   {
	  qsort_in_chuncks_partit  (list,index,i,j,m,n);
      qsort_in_chuncks1 (list,index,m,j);
      qsort_in_chuncks1 (list,index,i,n);
   }
   return;
}
void qsort_in_chuncks(Dynamic_Text &list,Integer &index[],Integer number) // TODO - test
//--------------------------------------------------------
// sort routine
//--------------------------------------------------------
{
   Integer j;

   // initialise the index
   for ( j=1 ; j<=number ; ++j ) index[j]=j;

   qsort_in_chuncks1 (list,index,1,number);

   return;
}
#endif

#endif