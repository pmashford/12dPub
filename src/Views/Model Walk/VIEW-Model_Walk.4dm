//model walk

#include "mashy_lib_security.H"
#include "mashy_lib_standard_library.H"
#include "mashy_lib_widgets_list_box.H"
#include "mashy_lib_widgets_validate.H"
#include "mashy_lib_views.H"
#include "mashy_lib_sort.H"
#include "mashy_lib_text_functions.H"
#include "mashy_lib_model_functions.H"
#include "mashy_lib_about_panel_SKM.H"
#include "mashy_lib_winhelp.4dm"
#include "defaults.H"

#define DEBUG 0

void validate_size(Integer &size){
	if(size <= 1)size = 1;
	if(size >= 7)size = 7;
}

Integer get_model_list_in_order_grouped_with_wildcard(Dynamic_Text &list){
	get_model_list_in_order(list);


	return 0;
}


Integer refresh_list_box(List_Box &box){
	Dynamic_Text list;
	get_model_list_in_order(list);
	set_items(box,list);

	return 0;
}

Integer refresh_list_box(List_Box &box, Input_Box &input_box){
	Text t;
	Get_data(input_box,t);
	if(text_justify(t) == "") t="*";
	Dynamic_Text list;
	get_model_list_in_order(list);
	Dynamic_Text match;
	match_name(list, t, match);
	set_items(box,match);

	return 0;
}

void my_set_focus(Widget w){
	Integer x,y,l,h;
	Get_position(w,x,y);
	Get_size(w,l,h);
	Set_cursor_position(x+l/2,y+h*2);
}

Integer manage_a_panel(Integer &posx, Integer &posy, Integer &size, Integer &button_focus){

	Panel panel = Create_panel("Model Walk");
	Message_Box message_box = Create_message_box("");
	Vertical_Group vg = Create_vertical_group(0);

	validate_size(size);

	View_Box view_box = Create_view_box("",message_box,1);	Set_width_in_chars(view_box,8);
	Input_Box input_box = Create_input_box("",message_box);	Set_width_in_chars(input_box,8);
	List_Box list_box = Create_list_box("List",message_box,size*5);
	Named_Tick_Box tick_box = Create_named_tick_box("Fit view?",1,"fit view");

	Horizontal_Group hg_buttons = Create_button_group();

	Button button = Create_button("  Refresh  ","refresh");

	Button button_grow = Create_button("v","grow");	Set_width_in_chars(button_grow,2);
	Button button_shrink = Create_button(" ^ ","shrink");	Set_width_in_chars(button_shrink,2);
	Horizontal_Group hg_resize = Create_button_group();
	Append(button_grow,hg_resize);
	Append(button_shrink,hg_resize);

	Button skmcad_help_button = Create_button("  Help  ","ModelWalk.html");

	Horizontal_Group hg = Create_horizontal_group(0);
	Append(view_box,hg);
	Append(input_box,hg);
	Append(hg,vg);
	Append(list_box,vg);
	//Append(tick_box,vg);
	//Append(message,vg);
	Append(button,hg_buttons);

	Horizontal_Group hg2 = Create_horizontal_group(0);
	Append(hg_buttons,hg2);
	Append(hg_resize,hg2);
	Append(skmcad_help_button,hg2);

	Append(hg2,vg);

	Append(vg,panel);

	Show_widget(panel,posx,posy);


	Set_sort(list_box,0);
	Set_selections(list_box,0);
	Set_auto_cut_paste(list_box,0);

	Set_data(input_box,"*");

	Integer dud;
	read_defaults_file(view_box, input_box, size);

	refresh_list_box(list_box,input_box);



	if(button_focus){
		if(button_focus==1){
			my_set_focus(button_grow);
		}else{
			//Set_focus(button_shrink);
			my_set_focus(button_shrink);
		}
		button_focus = 0;
	}

	while(1){
		Integer id;
		Text cmd,msg;
		Wait_on_widgets(id,cmd,msg);
#if DEBUG
		Print("Id=" + To_text(id) + " cmd=" + cmd + " msg=" + msg + "\n");
#endif
		if(cmd == "keystroke"){
			Print(msg);Print();
			continue;
		}

		switch(id){
			case Get_id(panel) : {
				if(cmd == "Panel Quit"){
					write_defaults_file(view_box, input_box, size);
					return(1);
				}
				if(cmd == "Panel About"){
					manage_about_panel_skm();
				}
			} break;

			case Get_id(skmcad_help_button):{
				open_skmcad_help(cmd);
			}break;

			case Get_id(button) : {
				if(cmd == "refresh"){
					refresh_list_box(list_box);
					Set_data(input_box,"*");
					Set_data(message_box,"OK");

					//RUN THE FUNCTION
				}
			}break;
			//Id=185083712 cmd=keystroke msg=d
			case Get_id(input_box) : {

				if(cmd == "keystroke" || cmd == "kill_focus"){
					refresh_list_box(list_box,input_box);
				}
			}break;

			case Get_id(list_box) : {
				if (cmd == "click_lb" || cmd == "click_rb"){
					View view;
					Text view_name;
					if(must_exist(view_box, view, view_name, message_box))break;
					Integer item;
					Get_selection(list_box,item);
					Text t;
					Get_item(list_box,item,t);
					view_all_off(view);
					view_add_model(view,t);
					View_fit(view);
				}
			}break;

			case Get_id(button_grow) : {
				Get_widget_size(panel,posx,posy);
				Print(To_text(posx)+" " + To_text(posy)+"\n");
				Get_widget_position(panel,posx,posy);
				Print(To_text(posx)+" " + To_text(posy)+"\n");
				size++;
				validate_size(size);
				write_defaults_file(view_box, input_box, size);

				button_focus = 1;

				return 0;
			}
			case Get_id(button_shrink) : {
				Get_widget_size(panel,posx,posy);
				Print(To_text(posx)+" " + To_text(posy)+"\n");
				Get_widget_position(panel,posx,posy);
				Print(To_text(posx)+" " + To_text(posy)+"\n");
				size--;
				validate_size(size);
				button_focus = (-1);
				write_defaults_file(view_box, input_box, size);

				return 0;
			}
		}


	}
	return(1);
}
void main(){
	standard_skm_macro_message("Model Walk");
	lock_to_skm_cad();
	Integer x,y;
	Integer size=2;
	View_Box b1; Input_Box b2;
	read_defaults_file(b1, b2, size);
	validate_size(size);
	Get_cursor_position(x,y);
	Integer button_focus = 0;
	while(1){
		if (manage_a_panel(x,y,size,button_focus))break;
	}
}