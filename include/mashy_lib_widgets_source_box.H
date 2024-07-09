#ifndef mashy_lib_widgets_standard_H_included
#define mashy_lib_widgets_standard_H_included

#include "mashy_lib_element_functions.H"

Dynamic_Element get_source_box_elements(Text source_box_data_string){

	Dynamic_Element de;
	Panel panel   = Create_panel("");
	Message_Box message = Create_message_box("  ");;
	Source_Box  source_box = Create_source_box("temp",message,0);
	Append(source_box,panel);
	Append(message,panel);
	Show_widget(panel);
	Set_data(source_box,source_box_data_string);
	Validate(source_box,de);
	Hide_widget(panel);
	return (de);
}

Integer fix_source_box_text_string(Text in, Source_Box &source_box, Text &out){
	Text to_find = "Field \"$SOURCE_BOX_NAME - Mode\" \"Source_Box_String\"";

	if(Find_text(in,to_find)){
		Text start = "Field \"$SOURCE_BOX_NAME - Mode\" \"Source_Box_String\"\nField \"$SOURCE_BOX_NAME - String\" \"Select_Box_String_Mode ";
		Dynamic_Element delt;	Validate(source_box,delt);
		Integer ierr = 0;
		Integer items;	Get_number_of_items(delt,items);
		if(items > 0)ierr = items; //set ierr to the number of items if not eq to 0 as there should only be 1 item
		else return(0);
		Element e;	Get_item(delt,1,e);
		Text model_string;
		get_element_name_ids(e,model_string);
		Text end   = model_string + "\"";
		out = start + end;
		return(ierr);
	}
	return (0); // nothing done
}

Text set_source_box_model(Text model_name){
	return("Field \"$SOURCE_BOX_NAME - Mode\" \"Source_Box_Model\"\nField    \"$SOURCE_BOX_NAME - Model\"        \""+model_name+"\"");
}

void set_source_box_model(Source_Box &box, Text model_name){
	Set_data(box,set_source_box_model(model_name));
}



#endif