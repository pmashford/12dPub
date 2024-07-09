#ifndef mashy_lib_sort_via_containers_H_INCLUDED
#define mashy_lib_sort_via_containers_H_INCLUDED
/*
	List of Containers of type *_Set and *_Multiset

	a "set" is a unique list of keys, a "multiset" is not unique, duplicates can occur

	Containers use a

	Integer
	Integer64
	Real
	Text
	Point
	Guid
	Uid
	Vector2
	Vector3
	Vector4

*/


// protoypes - THESE ARE FUNCTION PROTOYPES TO PRINT VARIOUS CONTAINERS TO THE CONSOLE
void print(Integer_Set &container);					void print(Integer_Set &container, Text pre, Text suff);
void print(Integer64_Set &container);				void print(Integer64_Set &container, Text pre, Text suff);
void print(Real_Set &container);					void print(Real_Set &container, Text pre, Text suff);
void print(Text_Set &container);					void print(Text_Set &container, Text pre, Text suff);
void print(Point_Set &container);					void print(Point_Set &container, Text pre, Text suff);
void print(Guid_Set &container);					void print(Guid_Set &container, Text pre, Text suff);
void print(Uid_Set &container);						void print(Uid_Set &container, Text pre, Text suff);
void print(Vector2_Set &container);					void print(Vector2_Set &container, Text pre, Text suff);
void print(Vector3_Set &container);					void print(Vector3_Set &container, Text pre, Text suff);
void print(Vector4_Set &container);					void print(Vector4_Set &container, Text pre, Text suff);

void print(Integer_Multiset &container);			void print(Integer_Multiset &container, Text pre, Text suff);
void print(Integer64_Multiset &container);			void print(Integer64_Multiset &container, Text pre, Text suff);
void print(Real_Multiset &container);				void print(Real_Multiset &container, Text pre, Text suff);
void print(Text_Multiset &container);				void print(Text_Multiset &container, Text pre, Text suff);
void print(Point_Multiset &container);				void print(Point_Multiset &container, Text pre, Text suff);
void print(Guid_Multiset &container);				void print(Guid_Multiset &container, Text pre, Text suff);
void print(Uid_Multiset &container);				void print(Uid_Multiset &container, Text pre, Text suff);
void print(Vector2_Multiset &container);			void print(Vector2_Multiset &container, Text pre, Text suff);
void print(Vector3_Multiset &container);			void print(Vector3_Multiset &container, Text pre, Text suff);
void print(Vector4_Multiset &container);			void print(Vector4_Multiset &container, Text pre, Text suff);

// helpers to convert types to text
Text tt(Integer 	t)	{	return To_text(t);}
Text tt(Integer64 	t)	{	return To_text(t);}
Text tt(Real 		t)	{	return To_text(t,3);}
Text tt(Text 		t)	{	return t;		}
Text tt(Point 		t)	{	return tt(Get_x(t))+","+tt(Get_y(t))+","+tt(Get_z(t));}
Text tt(Guid 		t)	{	return To_text(t);}
Text tt(Uid 		t)	{	return To_text(t);}
Text tt(Vector2 	t)	{	return tt(Get_vector(t,1))+","+tt(Get_vector(t,2));}
Text tt(Vector3 	t)	{	return tt(Get_vector(t,1))+","+tt(Get_vector(t,2))+","+tt(Get_vector(t,3));}
Text tt(Vector4 	t)	{	return tt(Get_vector(t,1))+","+tt(Get_vector(t,2))+","+tt(Get_vector(t,3))+","+tt(Get_vector(t,4));}

Text to_text(Integer v, Integer min_character_slots_to_pad_in_total_upto_10)
{
	Text t = To_text(v);
	Integer spaces = min_character_slots_to_pad_in_total_upto_10 - Text_length(t);
	if(spaces>13)spaces=13;
	switch (spaces){
		case 1 : { t="0"+t;} break;
		case 2 : { t="00"+t;} break;
		case 3 : { t="000"+t;} break;
		case 4 : { t="0000"+t;} break;
		case 5 : { t="00000"+t;} break;
		case 6 : { t="000000"+t;} break;
		case 7 : { t="0000000"+t;} break;
		case 8 : { t="00000000"+t;} break;
		case 9 : { t="000000000"+t;} break;
		case 10: { t="0000000000"+t;} break;
		case 11: { t="00000000000"+t;} break;
		case 12: { t="000000000000"+t;} break;
		case 13: { t="0000000000000"+t;} break;
	}
	return (t);
}

// print function as a macro
#define CONTAINER_PRINT(_FUNCTION_NAME,_TYPE,_CONTAINER) void _FUNCTION_NAME(_CONTAINER &container, Text prefix, Text suffix)\
{\
	Integer range = 1;\
	Integer res = Container_range_begin(container,range);\
	Integer count = 1;\
	Integer container_size_chars=0;\
	Container_size(container,container_size_chars);\
	container_size_chars = Text_length(To_text(container_size_chars));\
	Text t="v--------------------v\n";\
	while(res == 0)\
	{\
		_TYPE key;\
		Container_get_key(container,range,key);\
		t+=""+to_text(count,container_size_chars)+" |  "+prefix+tt(key)+suffix+"\n";\
		count++;\
		res = Container_range_next(container,range);\
		if(count%64 == 0){Print(t);t="";}\
	}\
	Container_range_remove(container,range);\
	Print(t+"^--------------------^\n");\
}\
void _FUNCTION_NAME(_CONTAINER &container){\
	return _FUNCTION_NAME(container, "", "");\
}

//call inline macro for each container type of *_Set and *_Multiset
CONTAINER_PRINT(print,Integer,Integer_Set)
CONTAINER_PRINT(print,Integer64,Integer64_Set)
CONTAINER_PRINT(print,Real,Real_Set)
CONTAINER_PRINT(print,Text,Text_Set)
CONTAINER_PRINT(print,Point,Point_Set)
CONTAINER_PRINT(print,Guid,Guid_Set)
CONTAINER_PRINT(print,Uid,Uid_Set)
CONTAINER_PRINT(print,Vector2,Vector2_Set)
CONTAINER_PRINT(print,Vector3,Vector3_Set)
CONTAINER_PRINT(print,Vector4,Vector4_Set)

CONTAINER_PRINT(print,Integer,Integer_Multiset)
CONTAINER_PRINT(print,Integer64,Integer64_Multiset)
CONTAINER_PRINT(print,Real,Real_Multiset)
CONTAINER_PRINT(print,Text,Text_Multiset)
CONTAINER_PRINT(print,Point,Point_Multiset)
CONTAINER_PRINT(print,Guid,Guid_Multiset)
CONTAINER_PRINT(print,Uid,Uid_Multiset)
CONTAINER_PRINT(print,Vector2,Vector2_Multiset)
CONTAINER_PRINT(print,Vector3,Vector3_Multiset)
CONTAINER_PRINT(print,Vector4,Vector4_Multiset)



// prototypes for sorting Dynamic_* via a container
// this will preserve number of items in the list (these could use the inline macro below, but decided this macro wont Null the list when it gets rebuilt
void sort_via_container(Dynamic_Text &list);
void sort_via_container(Dynamic_Real &list);
void sort_via_container(Dynamic_Integer &list);

// this will remove duplicates when sorting the list
void sort_unique_via_container(Dynamic_Text &list);
void sort_unique_via_container(Dynamic_Real &list);
void sort_unique_via_container(Dynamic_Integer &list);

// generic function as a macro  // NOTE - THESE HAVE A PRINT IN HERE FOR THE CONTAINER
#define SORT_VIA_CONTAINER(_FUNCTION_NAME,_DYNAMIC_LIST,_TYPE,_CONTAINER) void _FUNCTION_NAME(_DYNAMIC_LIST &list)\
{\
	_CONTAINER container;\
	Integer items;\
	Get_number_of_items(list,items);\
	for(Integer i=1;i<=items;i++)\
	{\
		_TYPE key;\
		Get_item(list,i,key);\
		Container_insert_key(container,key);\
	}\
	i = 1;\
	Integer range = -1;\
	Integer res = Container_range_begin(container,range);\
	while(res == 0)\
	{\
		_TYPE key;\
		Container_get_key(container,range,key);\
		Set_item(list,i,key);\
		i++;\
		res = Container_range_next(container,range);\
	}\
	Container_range_remove(container,range);\
	print(container);\
}\

#define SORT_UNIQUE_VIA_CONTAINER(_FUNCTION_NAME,_DYNAMIC_LIST,_TYPE,_CONTAINER) void _FUNCTION_NAME(_DYNAMIC_LIST &list)\
{\
	_CONTAINER container;\
	Integer items;\
	Get_number_of_items(list,items);\
	for(Integer i=1;i<=items;i++)\
	{\
		_TYPE key;\
		Get_item(list,i,key);\
		Container_insert_key(container,key);\
	}\
	Null(list);\
	Integer range = -1;\
	Integer res = Container_range_begin(container,range);\
	while(res == 0)\
	{\
		_TYPE key;\
		Container_get_key(container,range,key);\
		Append(key,list);\
		i++;\
		res = Container_range_next(container,range);\
	}\
	Container_range_remove(container,range);\
	print(container);\
}\

//call inline macro for each data and container type of *_Set and *_Multiset
SORT_VIA_CONTAINER(sort_via_container,Dynamic_Text,Text,Text_Multiset)
SORT_VIA_CONTAINER(sort_via_container,Dynamic_Real,Real,Real_Multiset)
SORT_VIA_CONTAINER(sort_via_container,Dynamic_Integer,Integer,Integer_Multiset)

SORT_UNIQUE_VIA_CONTAINER(sort_unique_via_container,Dynamic_Text,Text,Text_Set)
SORT_UNIQUE_VIA_CONTAINER(sort_unique_via_container,Dynamic_Real,Real,Real_Set)
SORT_UNIQUE_VIA_CONTAINER(sort_unique_via_container,Dynamic_Integer,Integer,Integer_Set)
//END


#ifndef mashy_lib_sort_via_containers_test_me
#define mashy_lib_sort_via_containers_test_me 0
#endif
#if mashy_lib_sort_via_containers_test_me
#include "mashy_lib_debug.H"
void test_me_sort_unique_via_container_1()
{
	Dynamic_Text list;
for(Integer i=1;i<=25;i++){
	Append("1",list);
	Append("00100",list);
	Append("01",list);
	Append("100",list);
	Append("-01",list);
	Append("999",list);
	Append("0.1",list);
}
	sort_via_container(list);
//	p(list);
}

void test_me_sort_unique_via_container_2()
{
	Dynamic_Integer list;

for(Integer i=1;i<=25;i++){
	Append(1,list);
	Append(00100,list);
	Append(01,list);
	Append(100,list);
	Append(-01,list);
	Append(999,list);
	Append(0.1,list);
}
	sort_via_container(list);
//	p(list);
}

void test_me_sort_unique_via_container_3()
{
	Dynamic_Real list;
for(Integer i=1;i<=25;i++){
	Append(1,list);
	Append(00100,list);
	Append(01,list);
	Append(100,list);
	Append(-01,list);
	Append(999,list);
	Append(0.1,list);
}
	sort_via_container(list);
//	p(list);
}
#endif


/*

*/
//sort_unique()

#endif