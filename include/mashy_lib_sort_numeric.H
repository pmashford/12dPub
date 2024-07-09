#if 0

Integer Quick_sort(Integer count,Integer index[],Integer array[]); // ID = 2745
Integer Quick_sort(Integer count,Integer index[],Real array[]); // ID = 2746
Integer Quick_sort(Integer count,Integer index[],Text array[]); // ID = 2747

#endif

//sort

#ifndef mashy_lib_sort_numeric_H_included
#define mashy_lib_sort_numeric_H_included


#include "mashy_lib_text_functions.H"
#include "12d/QSort.H"

//--------------------------------------------------------------------------------
// sort numeric - will sort text with "reals" on the end
// sort numeric ignore text prefix - if a real is found (the end of the text string is searched) then they will be ordered in ascending order, followed by any strings without reals on the end
//
//
//		sort_numeric_ignore_text_prefix(Dynamic_Text / Dynamic_Element / Model, Integer index[]);
//		sort_numeric(Dynamic_Text / Dynamic_Element / Model, Integer index[]);
//--------------------------------------------------------------------------------

void qsort_v9(Integer size, Integer &index[], Text &array[]){
	Dynamic_Text list;
	for(Integer i=1;i<=size;i++){
		Append(array[i],list);
	}
	Qsort(list,index,size);
}

void qsort_v9(Integer size, Integer &index[], Real &array[]){
	Dynamic_Real list;
	for(Integer i=1;i<=size;i++){
		Append(array[i],list);
	}
	Qsort(list,index,size);
}

void qsort_v9(Integer size, Integer &index[], Integer &array[]){
	Dynamic_Integer list;
	for(Integer i=1;i<=size;i++){
		Append(array[i],list);
	}
	Qsort(list,index,size);
}



Integer sub_sort_numeric(Integer size, Integer start, Integer end, Integer &full_index[], Real &full_array[]){
	Integer sub_size = end - start + 1;
	if(sub_size<1){
		#if DEBUG
		Print("sub_sort() error... size <1\n");
		#endif
		return 1;
	}

	Real sub_array[sub_size];
	Integer sub_index[sub_size];
	Integer sub_array_pos_in_full_index[sub_size];
	Integer count = 0;
	for(Integer i=start;i<=end;i++){
		count++;
		if(count > sub_size){
			#if DEBUG
			Print("sub_sort() error... count > sub_size\n");
			#endif
			break;
		}
		sub_array[count] = full_array[ full_index[i] ];
		sub_array_pos_in_full_index[count] = full_index[i];
	}

	#if VERSION_4D >= 1000
	Quick_sort(sub_size,sub_index,sub_array);
	#else
	qsort_v9(sub_size,sub_index,sub_array);
	#endif

	count = 0;
	for(i=start;i<=end;i++){
		count++; // increment
		full_index[i] = sub_array_pos_in_full_index[ sub_index[count] ];
	}

	return 0;
}


Integer sort_numeric_group(Dynamic_Text &dt, Dynamic_Text &grouped_with_star_suffix){
	Null(grouped_with_star_suffix);
	Integer items;
	Get_number_of_items(dt,items);
	if(items<1){
		Append(dt,grouped_with_star_suffix);
		return 1;
	}


	Integer order[items];

	Text text[items];
	Text left[items],right[items];
	Real value[items];
	Integer has_number[items];

	for(Integer i=1;i<=items;i++){
		Get_item(dt,i,text[i]);
		//split t to text and number (right is actually the text for number)
		if(split_text_check_for_number_as_last_number_part(text[i],left[i],right[i],value[i])) has_number[i] = 0;
		else has_number[i] = 1;
	}

	#if VERSION_4D >= 1000
	Quick_sort(items,order,text);
	#else
	qsort_v9(items,order,text);
	#endif

	Integer start = 0, end = 0;

	Integer count_group = 0;

	for(i=1;i<items;i++){

		Integer j1 = order[i];
		Integer j2 = order[i+1];

		if( (left[j1] == left[j2]) &&  has_number[j1] && has_number[j2] ){
			if(start == 0)start = i;
			 // - wast the else a bug ? - // else end = (i+1);
			 end = (i+1);
		}else{
			if(start){
				//do a sub sort on the numbers
				sub_sort_numeric(items, start, end, order, value);
				start = 0;
				end = 0;
				Append(text[j1],grouped_with_star_suffix);
			}else{
				Append(left[j1]+"*",grouped_with_star_suffix);
			}
		}
		if(i == (items - 1)){
			if(start){
				sub_sort_numeric(items, start, end, order, value);
				start = 0;
				end = 0;
				Append(left[j1]+"*",grouped_with_star_suffix);
			}
		}
	}

	return 0;
}

Integer sort_numeric(Dynamic_Text &dt, Integer &order[]){

	Integer items;
	Get_number_of_items(dt,items);
	if(items<1)return 1;

	Text text[items];
	Text left[items],right[items];
	Real value[items];
	Integer has_number[items];

	for(Integer i=1;i<=items;i++){
		Get_item(dt,i,text[i]);
		//split t to text and number (right is actually the text for number)
		if(split_text_check_for_number_as_last_number_part(text[i],left[i],right[i],value[i])) has_number[i] = 0;
		else has_number[i] = 1;
	}

	#if VERSION_4D >= 1000
	Quick_sort(items,order,text);
	#else
	qsort_v9(items,order,text);
	#endif

	Integer start = 0, end = 0;

	for(i=1;i<items;i++){

		Integer j1 = order[i];
		Integer j2 = order[i+1];

		if( (left[j1] == left[j2]) &&  has_number[j1] && has_number[j2] ){
			if(start == 0)start = i;
			 // - wast the else a bug ? - // else end = (i+1);
			 end = (i+1);
		}else{
			if(start){
				//do a sub sort on the numbers
				sub_sort_numeric(items, start, end, order, value);
				start = 0;
				end = 0;
			}
		}
		if(i == (items - 1)){
			if(start){
				sub_sort_numeric(items, start, end, order, value);
				start = 0;
				end = 0;
			}
		}
	}

	return 0;
}
/*
Integer sort_numeric(Dynamic_Element &delt, Integer order[]){
	Dynamic_Text names;
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Element e;
		Get_item(delt,i,e);
		Get_name(e,t);
		Append(t,names);
	}
	return sort_numeric(names,order);
	return 0;
}
*/
Integer sort_numeric(Dynamic_Text &list){
	Integer items;
	Get_number_of_items(list,items);
	if(items<1)return 1;
	Integer order[items];
	sort_numeric(list,order);
	Dynamic_Text dt;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(list,order[i],t);
		Append(t,dt);
	}
	Null(list);
	Append(dt,list);
	return 0;
}


Integer sort_numeric_group(Dynamic_Text &list){
	Integer ok = 0;
	ok+=sort_numeric(list);
	return ok;
}

Integer sort_numeric(Dynamic_Element &delt){

	Dynamic_Text dt;
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(delt,i,e);
		Text name;
		Get_name(e,name);
		Append(name,dt);
	}
	Integer index[items];


	sort_numeric(dt,index);

	for(i=1;i<=items;i++){
		Model m;
		Element e;
		Get_item(delt,index[i],e);
		Get_model(e,m);
		Set_model(e,m);
	}
	return 0;

}

Integer sort_numeric(Model m){
	Dynamic_Element delt;
	Append(m,delt);
	return sort_numeric(delt);
}

Integer sort_numeric(Text model_name){
	Model m = Get_model(model_name);
	Uid uid;
	Get_id(m,uid);
	if(Is_null(uid)){
		return 1;
	}
	return (sort_numeric(m));
}

Integer sort_numeric_ignore_text_prefix(Dynamic_Text &dt, Integer &index[]){
	Integer items;
	Get_number_of_items(dt,items);

	if(items<1)return 1;

	Text text[items];
	Text left[items],right[items];
	Real value[items];
	Integer has_number[items];

	Real value2[items];
	Integer index2[items];
	Integer count = 0;

	Integer index_no_num[items];
	Integer count_no_num = 0;

	for(Integer i=1;i<=items;i++){
		Get_item(dt,i,text[i]);
		//split t to text and number (right is actually the text for number)
		if(split_text_check_for_number_as_last_number_part(text[i],left[i],right[i],value[i])){
			has_number[i] = 0;
			count_no_num++;
			index_no_num[count_no_num] = i;

		}else{
			has_number[i] = 1;

			count++;
			value2[count] = value[i];
			index2[count] = i;
		}
	}

	Integer index22[count];
	#if VERSION_4D >= 1000
	Quick_sort(count,index22,value2);
	#else
	qsort_v9(count,index22,value2);
	#endif

	for(i=1;i<=count;i++){
		index[i] = index2[index22[i]];
	}
	Integer count3 = 0;
	for(i=count+1;i<=items;i++){
		count3++;
		index[i] = index_no_num[count3];
	}

	return 0;
}

Integer sort_numeric_ignore_text_prefix(Dynamic_Element &delt){

	Dynamic_Text dt;
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(delt,i,e);
		Text name;
		Get_name(e,name);
		Append(name,dt);
	}
	Integer index[items];


	sort_numeric_ignore_text_prefix(dt,index);

	for(i=1;i<=items;i++){
		Model m;
		Element e;
		Get_item(delt,index[i],e);
		Get_model(e,m);
		Set_model(e,m);
	}
	return 0;

}

Integer sort_numeric_ignore_text_prefix(Model m){
	Dynamic_Element delt;
	Append(m,delt);
	return sort_numeric_ignore_text_prefix(delt);
}

Integer sort_numeric_ignore_text_prefix(Text model_name){
	Model m = Get_model(model_name);
	Uid uid;
	Get_id(m,uid);
	if(Is_null(uid)){
		return 1;
	}
	return (sort_numeric_ignore_text_prefix(m));
}


#endif
