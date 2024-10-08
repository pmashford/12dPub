#ifndef mashy_lib_widgets_standard_H_included
#define mashy_lib_widgets_standard_H_included

// append upto _9_ widgets to a [ Panel | Vertical_Group | Horizontal_Group ] (UPDATE _X_ )
void append(Widget child, Panel parent){	Append(child,parent);}
void append(Widget child1, Widget child2, Panel parent){	Append(child1,parent); Append(child2,parent); }
void append(Widget child1, Widget child2, Widget child3, Panel parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Panel parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Panel parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Panel parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Panel parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Widget child8, Panel parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); Append(child8,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Widget child8, Widget child9, Panel parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); Append(child8,parent); Append(child9,parent); }

void append(Widget child,  Vertical_Group parent){	Append(child,parent);}
void append(Widget child1, Widget child2, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); }
void append(Widget child1, Widget child2, Widget child3, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Widget child8, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); Append(child8,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Widget child8, Widget child9, Vertical_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); Append(child8,parent); Append(child9,parent); }

void append(Widget child,  Horizontal_Group parent){	Append(child,parent);}
void append(Widget child1, Widget child2, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); }
void append(Widget child1, Widget child2, Widget child3, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Widget child8, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); Append(child8,parent); }
void append(Widget child1, Widget child2, Widget child3, Widget child4, Widget child5, Widget child6, Widget child7, Widget child8, Widget child9, Horizontal_Group parent){	Append(child1,parent); Append(child2,parent); Append(child3,parent); Append(child4,parent); Append(child5,parent); Append(child6,parent); Append(child7,parent); Append(child8,parent); Append(child9,parent); }

//--------------------------------------------------------------------------------
//                        USED TO CREATE WIDGET ARRAYS, AND FOR GRIDS
//--------------------------------------------------------------------------------
#include "mashy_lib_cast.H"
//Widget cast(Widget w){
//	return w;
//}

//--------------------------------------------------------------------------------
//                        THESE REPLICATE STD 12D CALLS - BUT CAN SEND AN ARRAY OF WIDGETS
//--------------------------------------------------------------------------------

void set_enable(Widget w[], Integer items,Integer mode){
	for(Integer i=1;i<=items;i++){
		Set_enable(w[i],mode);
	}
}


void set_visible(Widget w[], Integer items,Integer mode){
	for(Integer i=1;i<=items;i++){
		Set_visible(w[i],mode);
	}
}

void set_optional(Widget w[], Integer items,Integer mode){
	for(Integer i=1;i<=items;i++){
		Set_optional(w[i],mode);
	}
}

void set_width(Widget w[], Integer items,Integer width){
	for(Integer i=1;i<=items;i++){
		Set_width_in_chars(w[i],width);
	}
}

void set_size(Widget w[], Integer items,Integer x, Integer y){
	for(Integer i=1;i<=items;i++){
		Set_size(w[i],x,y);
	}
}

void set_name(Widget w[], Integer items,Text name){
	for(Integer i=1;i<=items;i++){
		Set_name(w[i],name);
	}
}

void set_tooltip(Widget w[], Integer items,Text tooltip){
	for(Integer i=1;i<=items;i++){
		Set_tooltip(w[i],tooltip);
	}
}

void use_browse_button(Widget w[], Integer items,Integer mode){
	for(Integer i=1;i<=items;i++){
		Use_browse_button(w[i],mode);
	}
}

void show_browse_button(Widget w[], Integer items,Integer mode){
	for(Integer i=1;i<=items;i++){
		Show_browse_button(w[i],mode);
	}
}

//--------------------------------------------------------------------------------
//                        Tin_Box
//--------------------------------------------------------------------------------

void append_tin_list_seed_ground(Dynamic_Text &dt){
		Append("X EX SURVEY ground",dt);
		Append("SUPER ground",dt);
		Append("ground",dt);
		Append("NATURAL",dt);
		Append("NS",dt);
}

void append_tin_list_seed_design(Dynamic_Text &dt){
		Append("SUPER DESIGN",dt);
		Append("DESIGN",dt);
}

void append_tin_list_seed_overall(Dynamic_Text &dt){
		Append("SUPER OVERALL",dt);
}


Integer set_tin(Tin_Box &tin_box, Integer mode){
	// mode 1, look for survey (actually not 2 ||3)
	// mode 2, look for design
	// mode 3, look for overall
	Dynamic_Text list;
	if(mode == 2){
		append_tin_list_seed_design(list);
	}else if(mode == 3){
		append_tin_list_seed_overall(list);
	}else{
		append_tin_list_seed_ground(list);
	}

	Dynamic_Text tins;
	Get_project_tins(tins);
	Integer ntins;
	Get_number_of_items(tins,ntins);
	Integer items;
	Get_number_of_items(list,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(list,i,t);
		t = Text_lower(t);
		for(Integer j=1;j<=ntins;j++){
			Text this_tin;
			Get_item(tins,j,this_tin);
			if(t == Text_lower(this_tin)){
				Set_data(tin_box,this_tin);
				return 0;
			}
		}
	}
	return 1;
}
Integer set_tin(Tin_Box &box){
	return set_tin(box,1);
}

//--------------------------------------------------------------------------------
//                        Choice box
//--------------------------------------------------------------------------------

Integer set_data(Choice_Box &box, Dynamic_Text &list){
	Integer items;
	Get_number_of_items(list,items);
	Integer nc = 1;
	if(items < 1){
		Text choices[1];
		choices[1] = "";
		return Set_data(box,nc,choices);
	}
	nc = items;
	Text choices[nc];
	for(Integer i=1;i<=nc;i++){
		Text t;	Get_item(list,i,t);
		choices[i] = t;
	}
	return Set_data(box,nc,choices);
}

Integer set_choices(Choice_Box &box, Dynamic_Text &list){
	return set_data(box, list);
}

#endif