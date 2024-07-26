//model walk

#include "mashy_lib_widgets_list_box.H"
#include "mashy_lib_widgets_validate.H"
#include "mashy_lib_views.H"
//#include "mashy_lib_text_functions.H"
//#include "mashy_lib_model_functions.H"
#include "mashy_lib_about_panel.H"
#include "mashy_lib_widgets_standard.H"
#include "mashy_lib_panel_defaults.H"

#define DEBUG 0

void validate_size(Integer &size){
	if(size <= 1)size = 1;
	if(size >= 7)size = 7;
}

Integer get_model_list_in_order_grouped_with_wildcard(Dynamic_Text &list){
	return get_model_list_in_order(list);
}


Integer refresh_list_box(List_Box &box){
	Dynamic_Text list;
	get_model_list_in_order(list);
	return set_items(box,list);
}

Integer refresh_list_box(List_Box &box, Input_Box &input_box){
	Text t;
	Get_data(input_box,t);
	t = "*"+text_justify(t) +"*";
	Dynamic_Text list;
	get_model_list_in_order(list);
	Dynamic_Text match;
	match_name_ignore_case(list, t, match);
	set_items(box,match);
	return 0;
}

Integer manage_a_panel(){

	Panel panel = Create_panel("Model Walk",1);
	Message_Box message_box = Create_message_box("");
	Vertical_Group vg = Create_vertical_group(0);

	View_Box view_box = Create_view_box("",message_box,1);	Set_width_in_chars(view_box,8);	Set_dump_name(view_box,"View to walk");
	Input_Box input_box = Create_input_box("",message_box);	Set_width_in_chars(input_box,8);	Set_dump_name(input_box,"Model filter");
	List_Box list_box = Create_list_box("List",message_box,10);

	Button button = Create_button("  Refresh  ","refresh");

	Horizontal_Group hg_buttons = Create_button_group();
	Horizontal_Group hg_bot = Create_horizontal_group(0);
	Horizontal_Group hg_top = Create_horizontal_group(0);

	append(vg, panel);
	append(hg_top, list_box, hg_buttons,vg);
	append(view_box, input_box, hg_top);
	append(button, hg_buttons);

	Show_widget(panel);

	Set_selections(list_box,0);
	Set_auto_cut_paste(list_box,0);

	read_panel(panel);

	refresh_list_box(list_box,input_box);

	while(1){
		Integer id;	Text cmd,msg;
		Wait_on_widgets(id,cmd,msg);
		// Print("Id=" + To_text(id) + " cmd=" + cmd + " msg=" + msg + "\n");
		if(cmd == "keystroke")	{	if(id != Get_id(input_box))	continue;}
		if(cmd == "Panel About")	manage_about_panel();
		if(cmd == "Panel Quit")	{	write_panel(panel);	return(1);	}

		switch(id){

			case Get_id(button) : {
				if(cmd == "refresh"){
					refresh_list_box(list_box);
					Set_data(message_box,"OK");
				}
			}break;
			case Get_id(input_box) : {
				if(cmd == "keystroke" || cmd == "kill_focus"){
					refresh_list_box(list_box,input_box);
				}
			}break;

			case Get_id(list_box) : {
				if (cmd == "click_lb" || cmd == "click_rb"){
					View view;	Text view_name;
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

		}
	}
	return(1);
}

void main(){
	Print("VIEW-Model_Walk.4do by Paul Mashford\n");
	manage_a_panel();
}