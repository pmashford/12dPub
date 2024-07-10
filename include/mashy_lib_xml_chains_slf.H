#ifndef mashy_lib_xml_chains_slf_H_included
#define mashy_lib_xml_chains_slf_H_included

#include "mashy_lib_xml.H"

XML_Node slf_create_panel_node(Text panel_name){
	XML_Node node = Create_node("panel");
	add_xml_node(node,"name",panel_name);
	return node;
}

XML_Node slf_create_macro_panel_node(Text path_to_macro){
	XML_Node node = Create_node("macro_panel");
	add_xml_node(node,"name",path_to_macro);
	return node;
}

Integer slf_append_input_box(XML_Node &panel_node, Text name, Text value){
	XML_Node node = Create_node("input_box");
	add_xml_node(node,"name",name);
	add_xml_node(node,"value",value);
	return Append_node(panel_node,node);
}

Integer slf_append_tick_box(XML_Node &panel_node, Text name, Integer value){
	Text tick_value = "true";
	if(value == 0) tick_value = "false";
	XML_Node node = Create_node("tick_box");
	add_xml_node(node,"name",name);
	add_xml_node(node,"value",tick_value);
	return Append_node(panel_node,node);
}

Integer slf_append_file_box(XML_Node &panel_node, Text name, Text value){
	XML_Node node = Create_node("file_box");
	add_xml_node(node,"name",name);
	add_xml_node(node,"value",value);
	return Append_node(panel_node,node);
}

Integer slf_append_run_button(XML_Node &panel_node, Text button_name){
	XML_Node node = Create_node("run_button");
	add_xml_node(node,"name",button_name);
	return Append_node(panel_node,node);
}

Integer slf_append_panel_position(XML_Node &panel_node, Integer x, Integer y){
	add_xml_node(panel_node,"x",To_text(x));
	add_xml_node(panel_node,"y",To_text(y));
	return 0;
}

// grid stuff
XML_Node slf_create_grid_box_node(Text grid_name){
	XML_Node node = Create_node("grid_box");
	add_xml_node(node,"name",grid_name);
	return node;
}

Integer slf_grid_append_column(XML_Node &grid_node, Text column_name){
	XML_Node columns_node;
	get_create_node2(grid_node, "columns", columns_node);
	return add_xml_node(columns_node,"column",column_name);
}

Integer slf_grid_append_row(XML_Node &grid_node, Dynamic_Text &row){
	XML_Node row_node = Create_node("r");
	Integer items;
	Get_number_of_items(row,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(row,i,t);
		add_xml_node(row_node,"c",t);
	}
	XML_Node data_node;
	get_create_node2(grid_node, "data", data_node);
	return Append_node(data_node,row_node);
}


//--------------------------------------------------------------------------------
//                        SPECIFIC PANELS AS SLF
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//                        View Dump
//--------------------------------------------------------------------------------
// format PNG, JPEG, BMP, GIF, TGA, JPEG 2000
// filename should contain extension
XML_Node slf_view_dump(Text view_name, Integer include_title, Text format, Text file_name){
	XML_Node panel_node = slf_create_panel_node("View Dump");
	slf_append_input_box	(panel_node,"View",view_name);
	slf_append_tick_box		(panel_node,"Include title",include_title);
	slf_append_input_box	(panel_node,"Format",format);
	slf_append_file_box		(panel_node,"File",file_name);
	slf_append_run_button	(panel_node,"Dump");
	return panel_node;
}

XML_Node slf_view_colour(Text view_name, Text colour_name){
	XML_Node panel_node = slf_create_panel_node("View Background Colour");
	slf_append_input_box	(panel_node,"View",view_name);
	slf_append_input_box	(panel_node,"Colour",colour_name);
	slf_append_run_button	(panel_node,"Set");
	return panel_node;
}

XML_Node slf_view_colour(Text view_name, Integer colour){
	Text colour_name = "1";
	Convert_colour(colour,colour_name);
	return slf_view_colour(view_name, colour_name);
}

XML_Node slf_reference_las_files(Text filename, Text model_name){
	XML_Node panel_node = slf_create_panel_node("Reference LAS Files");
	slf_append_tick_box	(panel_node,"Advanced",0);
	slf_append_file_box	(panel_node,"File to read",filename);
	slf_append_tick_box	(panel_node,"Use file pre*post stem",0);
	slf_append_input_box(panel_node,"Colour for data","acad 001");
	slf_append_input_box(panel_node,"Model for data",model_name);
	slf_append_tick_box	(panel_node,"Draw cloud string",1);
	slf_append_tick_box	(panel_node,"Pin cloud string",1);
	return panel_node;
}

XML_Node slf_read_geotiff_files(Text filename, Text model_name, Text string_name){
	XML_Node panel_node = slf_create_panel_node("Read Geotiff DEM Files");
	slf_append_tick_box	(panel_node,"Advanced",0);
	slf_append_file_box	(panel_node,"File to read",filename);
	slf_append_tick_box	(panel_node,"Use file pre*post stem",0);
	slf_append_input_box(panel_node,"Grid Name","string_name"); //TODO - need to do better than that
	slf_append_input_box(panel_node,"Grid Model",model_name);
	slf_append_input_box(panel_node,"Grid Colour","acad 002");
	slf_append_input_box(panel_node,"Create mode","String");
	slf_append_input_box(panel_node,"Pixel Area shift mode","North East");
	
	return panel_node;
}

//--------------------------------------------------------------------------------
//                        Add Shared Models to Project
//--------------------------------------------------------------------------------
XML_Node slf_add_shared_models_to_project(Text folder, Text project, Integer match_sub_strings,
												Integer pattern_expression, Text pattern_search, Text pattern_replace,
												Integer regular_expression, Text regex_search, Text regex_replace,
												Dynamic_Integer &add, Dynamic_Text &original_model_name, Dynamic_Text &new_model_name, Dynamic_Text &status){
	//
	//

	Text this_panel_name = "Add Shared Models to Project";
	//this_panel_name = "Share In Models to this Project";

	XML_Node panel_node = slf_create_panel_node(this_panel_name);

	slf_append_input_box	(panel_node,"Folder",folder);
	slf_append_input_box	(panel_node,"Project",project);

	slf_append_tick_box		(panel_node,"Match sub strings",match_sub_strings);

	slf_append_tick_box		(panel_node,"Pattern expression",pattern_expression);
	slf_append_input_box	(panel_node,"Pattern Search",pattern_search);
	slf_append_input_box	(panel_node,"Pattern Replace",pattern_replace);

	slf_append_tick_box		(panel_node,"Regular expression",regular_expression);
	slf_append_input_box	(panel_node,"Regex Search",regex_search);
	slf_append_input_box	(panel_node,"Regex Replace",regex_replace);

	XML_Node grid_node = slf_create_grid_box_node("Share Models");
	slf_grid_append_column(grid_node,"Add");
	slf_grid_append_column(grid_node,"Original Model Name");
	slf_grid_append_column(grid_node,"New Model Name");
	slf_grid_append_column(grid_node,"Status");

	Integer items;
	Get_number_of_items(add,items);
	for(Integer i=1;i<=items;i++){
		Integer i1;
		Text f1,f2,f3,f4;
		Get_item(add,i,i1);	f1 = To_text(i1);
		Get_item(original_model_name,i,f2);
		Get_item(new_model_name,i,f3);
		Get_item(status,i,f4);
		Dynamic_Text row;
		Set_item(row,1,f1);
		Set_item(row,2,f2);
		Set_item(row,3,f3);
		Set_item(row,4,f4);
		slf_grid_append_row(grid_node, row);
	}
	Append_node(panel_node,grid_node);
	return panel_node;
}

//--------------------------------------------------------------------------------
//                        Add Shared Tins to Project
//--------------------------------------------------------------------------------
XML_Node slf_add_shared_tins_to_project(Text folder, Text project, Integer match_sub_strings,
												Integer pattern_expression, Text pattern_search, Text pattern_replace,
												Integer regular_expression, Text regex_search, Text regex_replace,
												Dynamic_Integer &add, Dynamic_Text &original_name, Dynamic_Text &new_name, Dynamic_Text &model_name, Dynamic_Text &status){
	//
	//
	Text this_panel_name = "Add Shared Tins to Project";

	//this_panel_name = "Share In Tins to this Project";

	XML_Node panel_node = slf_create_panel_node(this_panel_name);

	slf_append_input_box	(panel_node,"Folder",folder);
	slf_append_input_box	(panel_node,"Project",project);

	slf_append_tick_box		(panel_node,"Match sub strings",match_sub_strings);

	slf_append_tick_box		(panel_node,"Pattern expression",pattern_expression);
	slf_append_input_box	(panel_node,"Pattern Search",pattern_search);
	slf_append_input_box	(panel_node,"Pattern Replace",pattern_replace);

	slf_append_tick_box		(panel_node,"Regular expression",regular_expression);
	slf_append_input_box	(panel_node,"Regex Search",regex_search);
	slf_append_input_box	(panel_node,"Regex Replace",regex_replace);

	XML_Node grid_node = slf_create_grid_box_node("Share Tins");
	slf_grid_append_column(grid_node,"Add");
	slf_grid_append_column(grid_node,"Original Tin Name");
	slf_grid_append_column(grid_node,"New Tin Name");
	slf_grid_append_column(grid_node,"Model for Tin");
	slf_grid_append_column(grid_node,"Status");

	Integer items;
	Get_number_of_items(add,items);
	for(Integer i=1;i<=items;i++){
		Integer i1;
		Text f1,f2,f3,f4,f5;
		Get_item(add,i,i1);	f1 = To_text(i1);
		Get_item(original_name,i,f2);
		Get_item(new_name,i,f3);
		Get_item(model_name,i,f4);
		Get_item(status,i,f5);
		Dynamic_Text row;
		Set_item(row,1,f1);
		Set_item(row,2,f2);
		Set_item(row,3,f3);
		Set_item(row,4,f4);
		Set_item(row,5,f5);
		slf_grid_append_row(grid_node, row);
	}
	Append_node(panel_node,grid_node);
	return panel_node;
}




#if 0
void usage(){
	XML_Node panel_node = slf_create_panel_node("View Dump");
	slf_append_input_box	(panel_node,"View","1");
	slf_append_tick_box		(panel_node,"Include title",1);
	slf_append_input_box	(panel_node,"Format","JPEG");
	slf_append_file_box		(panel_node,"File","my_file.jpg");
	slf_append_run_button	(panel_node,"Dump");
}
#endif






#endif