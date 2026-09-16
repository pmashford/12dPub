#ifndef mashylib_printl_H_INCLUDED
#define mashylib_printl_H_INCLUDED

// any required prototypes for this file?
void printe(Text t);

#include "mashy_lib_element_functions.H"

#if 0

Log_Line Create_text_log_line(Text message,Integer log_level); // ID = 2663
Log_Line Create_highlight_string_log_line(Text message,Integer log_level,Uid model_id,Uid string_id); // ID = 2664
Log_Line Create_highlight_string_log_line(Text message,Integer log_level,Uid model_id,Uid string_id,Real x,Real y,Real z); // ID = 2665
Log_Line Create_highlight_point_log_line(Text message,Integer log_level,Real x,Real y,Real z); // ID = 2666
Log_Line Create_edit_string_log_line(Text message,Integer log_level,Uid model_id,Uid string_id); // ID = 2667
Log_Line Create_macro_log_line(Text message,Integer log_level,Text macro,Text select_cmd_line); // ID = 2668
Log_Line Create_macro_log_line(Text message,Integer log_level,Text macro,Text select_cmd_line,Dynamic_Text menu_names,Dynamic_Text menu_command_lines); // ID = 2669
Integer Print_log_line(Log_Line line,Integer is_error); // ID = 2670

Log_Box Create_log_box(Text name,Integer width,Integer height); // ID = 2671
Integer Add_log_line(Log_Box log_box,Log_Line line); // ID = 2672
Integer Clear(Log_Box box); // ID = 2673

Log_Line Create_group_log_line(Text message,Integer log_level); // ID = 3757
Integer Get_type(Log_Line line,Integer &type); // ID = 3758
Integer Get_type(Log_Line line,Text &type); // ID = 3759
Integer Get_id(Log_Line line,Integer &id); // ID = 3760
Integer Get_parent_id(Log_Line line,Integer &parent); // ID = 3761
Integer Get_parent(Log_Line line,Log_Line &parent); // ID = 3762
Integer Append_log_line(Log_Line line,Log_Line parent); // ID = 3763



#endif

// Printing log lines, these are valid log_level for Create_text_log_line(Text message,Integer log_level)
// 0 ==   (none)
// 1 == ! (green)
// 2 == ! (orange)
// 3 == ! (red)

// no idea what is_error means for Print_log_line(Log_Line line,Integer is_error)

#if 0
	// usage
	printl(); // plain log line
	printg(); // green log line
	printw(); // orange log line
	printe(); // red log line
#endif
//--------------------------------------------------------------------------------
//                        printing log lines highlighting point
//--------------------------------------------------------------------------------

// ------- there is a bug in there as at v10c1i
// http://beta.12dmodel.com/forums/viewtopic.php?p=19099
// need to shift the x y z values
void print(Text t, Real x, Real y, Real z, Integer log_level){
	#if VERSION_4D >= 1000
		//Print_log_line(Create_highlight_point_log_line(t,log_level,x,y,z),1);
		Print_log_line(Create_highlight_point_log_line(t,log_level,z,x,y),1);
	#else
		t = t += " <"+To_text(x,3)+","+To_text(y,3)+","+To_text(z,3)+">";
		Print(t+"\n");
	#endif
}
// provide element - overloads
void print(Text t, Real x, Real y, Real z){	print(t,x,y,z,0);	}
void printl(Text t, Real x, Real y, Real z){	print(t,x,y,z,0);	}
void printg(Text t, Real x, Real y, Real z){	print(t,x,y,z,1);	}
void printw(Text t, Real x, Real y, Real z){	print(t,x,y,z,2);	}
void printe(Text t, Real x, Real y, Real z){	print(t,x,y,z,3);	}

//--------------------------------------------------------------------------------
//                        printing log lines with and element
//--------------------------------------------------------------------------------
void print(Text t, Element &e, Integer include_name_in_message, Integer log_level){
	if(include_name_in_message){
		t+=" <" + get_full_name(e) + ">";
	}
	Uid uid_e,uid_m;
	get_uid(e,uid_e,uid_m);
	#if VERSION_4D >= 1000
		Print_log_line(Create_highlight_string_log_line(t,log_level,uid_m,uid_e),1);
	#else
		Print(t+"\n");
	#endif
}

//
void print(Text t, Element &e, Real x, Real y, Real z, Integer include_name_in_message, Integer log_level){
	if(include_name_in_message){
		t+=" <" + get_full_name(e) + ">";
	}
	Uid uid_e,uid_m;
	get_uid(e,uid_e,uid_m);
#if VERSION_4D >= 1000
	Print_log_line(Create_highlight_string_log_line(t,log_level,uid_m,uid_e,x,y,z),1);
#else
	Print(t+"\n");
#endif
}

void print(Text t, Element &e, Integer vertex_to_highlight, Integer include_name_in_message, Integer log_level){
	if(include_name_in_message){
		t+=" <" + get_full_name(e) + ">";
	}
	Real x,y,z;
	Get_data(e,vertex_to_highlight,x,y,z);
	Uid uid_e,uid_m;
	get_uid(e,uid_e,uid_m);
	#if VERSION_4D >= 1000
		Print_log_line(Create_highlight_string_log_line(t,log_level,uid_m,uid_e,x,y,z),1);
	#else
		Print(t+"\n");
	#endif
}


// provide element and x,y,z - overloads
void print(Text t, Element &e, Real x, Real y, Real z, Integer include_name_in_message){	print(t,e,x,y,z,include_name_in_message,0);	}
void print(Text t, Element &e, Real x, Real y, Real z){		print(t,e,x,y,z,0,0);	}
void printl(Text t, Element &e, Real x, Real y, Real z){	print(t,e,x,y,z,0,0);	}
void printg(Text t, Element &e, Real x, Real y, Real z){	print(t,e,x,y,z,0,1);	}
void printw(Text t, Element &e, Real x, Real y, Real z){	print(t,e,x,y,z,0,2);	}
void printe(Text t, Element &e, Real x, Real y, Real z){	print(t,e,x,y,z,0,3);	}

// provide element and x,y
void print(Text t, Element &e, Real x, Real y, Integer include_name_in_message){	print(t,e,x,y,0.0,include_name_in_message,0);	}
void print(Text t, Element &e, Real x, Real y){		print(t,e,x,y,0.0,0,0);	}
void printl(Text t, Element &e, Real x, Real y){	print(t,e,x,y,0.0,0,0);	}
void printg(Text t, Element &e, Real x, Real y){	print(t,e,x,y,0.0,0,1);	}
void printw(Text t, Element &e, Real x, Real y){	print(t,e,x,y,0.0,0,2);	}
void printe(Text t, Element &e, Real x, Real y){	print(t,e,x,y,0.0,0,3);	}

// provide element and vertex
//void print(Text t, Element &e, Integer v, Integer include_name_in_message){	print(t,e,v,include_name_in_message,0);	}
void print(Text t, Element &e, Integer v){		print(t,e,v,0,0);	}
void printl(Text t, Element &e, Integer v){	print(t,e,v,0,0);	}
void printg(Text t, Element &e, Integer v){	print(t,e,v,0,1);	}
void printw(Text t, Element &e, Integer v){	print(t,e,v,0,2);	}
void printe(Text t, Element &e, Integer v){	print(t,e,v,0,3);	}

//--------------------------------------------------------------------------------
//                        basic text log lines
//--------------------------------------------------------------------------------

void printl(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,0),0);
#else
	Print(t+"\n");
#endif
}

void printg(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,1),0);
#else
	Print(t+"\n");
#endif
}

void printw(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,2),0);
#else
	Print(t+"\n");
#endif
}

void printe(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,3),0);
#else
	Print(t+"\n");
#endif
}

void print_green(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,1),0);
#else
	Print(t+"\n");
#endif
}

void error_green(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,1),1);
#else
	Print(t+"\n");
#endif
}

void print_orange(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,2),0);
#else
	Print(t+"\n");
#endif
}

void error_orange(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,2),1);
#else
	Print(t+"\n");
#endif
}

void print_red(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,3),0);
#else
	Print(t+"\n");
#endif
}

void error_red(Text t){
#if VERSION_4D >= 1000
	Print_log_line(Create_text_log_line(t,3),1);
#else
	Print(t+"\n");
#endif
}

#endif