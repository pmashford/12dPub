#ifndef mashy_lib_xml_chains_H_included
#define mashy_lib_xml_chains_H_included

#include "mashy_lib_xml.H"
#include "mashy_lib_xml_chains_slf.H"
#include "mashy_lib_element_functions.H"

/*
      <Chain>
        <Name>Recalc chain 100 DESIGN.rcn</Name>
        <Active>true</Active>
        <Continue_on_failure>false</Continue_on_failure>
        <Uses_parameters>false</Uses_parameters>
        <Interactive>false</Interactive>
        <Comments>
          <Comment/>
        </Comments>
        <Chain>.\\100 DESIGN.chain</Chain>
        <Chain_Parameters></Chain_Parameters>
      </Chain>
*/

//--------------------------------------------------------------------------------
//                        Chain commands generic
//--------------------------------------------------------------------------------

XML_Node create_child_comment(Text comment){
	XML_Node node = Create_node("Comments");
	add_xml_node(node,"Comment",comment);
	return node;
}

Text to_true_false(Integer i){
	if(i)return "true";
	return "false";
}

XML_Node create_chain_link_standard(Text chain_command, Text chain_link_name, Integer i_active, Integer i_continue_on_failure, Integer i_use_parameteres, Integer i_interactive, Text comment){
	XML_Node node = Create_node(chain_command);
	add_xml_node(node,"Name",chain_link_name);
	add_xml_node(node,"Active",to_true_false(i_active));
	add_xml_node(node,"Continue_on_failure",to_true_false(i_continue_on_failure));
	add_xml_node(node,"Uses_parameters",to_true_false(i_use_parameteres));
	add_xml_node(node,"Interactive",to_true_false(i_interactive));
	Append_node(node,create_child_comment(comment));
	return node;
}

XML_Node create_chain_link(Text chain_command, Text chain_link_name){
	return create_chain_link_standard(chain_command, chain_link_name, 1, 0, 0, 0, "");
}

XML_Node create_chain_parameter_mapping(Text plot_parameter, Text parameter){
	XML_Node node = Create_node("Parameter_Mapping");
	add_xml_node(node,"Plot_Parameter",plot_parameter);
	add_xml_node(node,"Parameter",parameter);
	return node;
}
XML_Node create_chain_parameter_mappings(Dynamic_Text &plot_parameters, Dynamic_Text &parameters){
	XML_Node node = Create_node("Parameter_Mappings");
	Integer items;
	Get_number_of_items(plot_parameters,items);
	for(Integer i=1;i<=items;i++){
		Text t1,t2;
		Get_item(plot_parameters,i,t1);
		Get_item(parameters,i,t2);
		Append_node(node,create_chain_parameter_mapping(t1,t2));
	}
	return node;
}

XML_Node create_chain_parameter_mappings(){
	return Create_node("Parameter_Mappings");
}

// this is the syntax for strings in xml chains
Integer append_element(Element &e, XML_Node &parent_node){
	Text model_name,name;
	Uid model_id,id;
	get_element_names_ids(e, model_name, model_id, name, id);
	add_xml_node(parent_node,"Model_Name",model_name);
	add_xml_node(parent_node,"Model_ID",To_text(model_id));
	add_xml_node(parent_node,"Element_Name",name);
	add_xml_node(parent_node,"Element_ID",To_text(id));
	return 0;
}

//--------------------------------------------------------------------------------
//                        Chain commands specific
//--------------------------------------------------------------------------------

// create the panel node first - this adds all the guff to make the panel a chain "run option", returns the run_option node
XML_Node create_link_run_option(XML_Node &panel_node){
	XML_Node node = create_chain_link("Run_option","run_option");
	XML_Node slf_node = Create_node("SLF_data");
	XML_Node screen_layout_node = Create_node("screen_layout");
	add_xml_node(screen_layout_node,"version","1.0");			// static for now

	Append_node(node,slf_node);
	Append_node(slf_node,screen_layout_node);
	Append_node(screen_layout_node,panel_node);

	return node;
}

XML_Node create_link_option_manual(XML_Node panel_node, Text panel_name, Text button_name){
    XML_Node node = create_chain_link("Manual_option","Option Recalc Chain - Manual");
    XML_Node panel_data_node = Create_node("Panel_Data");
    Append_node(node,panel_data_node);
        XML_Node screen_layout_node = Create_node("screen_layout");
        add_xml_node(screen_layout_node,"version","1.0");			// static for now - def
        Append_node(panel_data_node,screen_layout_node);
            Append_node(screen_layout_node,panel_node);
    add_xml_node(node,"Panel_Name",panel_name);			// Is this even needed?
    add_xml_node(node,"Clean_Up","1");			// Is this even needed?
    XML_Node buttons_node = Create_node("Buttons");
    Append_node(node,buttons_node);
    XML_Node button_node = Create_node("Button");
    Append_node(buttons_node,button_node);
        add_xml_node(button_node,"Name",button_name);
        add_xml_node(button_node,"Order","0");			// static for now, how to handle mutliple button presses I guess
	return node;
}

XML_Node create_link_run_chain(Text chain_name){
	XML_Node node = create_chain_link("Chain","Recalc chain "+chain_name);
	add_xml_node(node,"Chain",chain_name);
	add_xml_node(node,"Chain_Parameters","");
	return node;
}

XML_Node create_link_function(Text function_name){
	XML_Node node = create_chain_link("Function","Recalc "+function_name);
	add_xml_node(node,"Function",function_name);
	return node;
}

XML_Node create_link_comment(Text my_comment){
	XML_Node node = create_chain_link("Comment",my_comment);
	return node;
}

XML_Node create_link_delete_file(Text file_name){
	XML_Node node = create_chain_link("Delete","delete a file");
	add_xml_node(node,"Active","false");
	add_xml_node(node,"Continue_on_failure","true");
	add_xml_node(node,"Mode","2");
	add_xml_node(node,"Target_Name",file_name);
	add_xml_node(node,"Target_ID","0");
	return node;
}

// this can be used as the last link in a chain to delete itself... gets around some edge cases inside wait_on_widget events being out of order
// ... so dont use File_delete(chain), use this to delete itself.
Integer append_delete_command_to_self_destruct(XML_Document &chain, Text file_name){
	XML_Node commands_node;
	get_create_node(chain,"Chain/Commands",commands_node);
	return Append_node(commands_node,create_link_delete_file(file_name));
}

XML_Node create_link_macro(Text macro_file, Text arguments){
	XML_Node node = create_chain_link("Run_macro","Macro "+macro_file);
	add_xml_node(node,"Macro_File",macro_file);
	add_xml_node(node,"Arguments",arguments);
	return node;
}

XML_Node create_link_macro(Text macro_file, Dynamic_Text arguments){
	Integer items;
	Get_number_of_items(arguments,items);
	Text args = "";
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(arguments,i,t);
		if(i>1)args+=" ";
		args += "\""+t+"\"";
	}
	return create_link_macro(macro_file, args);
}

XML_Node create_link_delete_view(Text view_name){
	XML_Node node = create_chain_link("Delete_view","Delete View "+view_name);
	add_xml_node(node,"View",view_name);
	return node;
}

Integer enumerate_view_type_for_chain(Text type){
	if(type == "Plan" || type == "Plan view"){
		return 2010;
	}else if(type == "Section" || type == "Section view"){
		return 2011;
	}else if(type == "Perspective" || type == "Perspective view"){
		return 2012;
	}else if(type == "OpenGL" || type == "OpenGL view"){
		return (-2012);
	}
	return 2010; // default to plan view if type is foobar
}

XML_Node create_link_create_view(Text view_name, Text view_type){
	XML_Node node = create_chain_link("Create_view","Create View "+view_name);
	add_xml_node(node,"View",view_name);
	add_xml_node(node,"View_Type",To_text(enumerate_view_type_for_chain(view_type)) );
	return node;
}


XML_Node create_link_plot(Text plot_type, Text plot_file){
	XML_Node node = create_chain_link("Plot","Plot "+plot_type+" "+plot_file);
	add_xml_node(node,"PPF_Type",plot_type);
	add_xml_node(node,"PPF_File",plot_file);
	Append_node(node,create_chain_parameter_mappings()); // </Parameter_Mappings> // <= need to add this some time
	return node;
}

XML_Node create_link_plot_cross_section(Text plot_file){
	return create_link_plot("Cross section",plot_file);
}

XML_Node create_link_plot_long_section(Text plot_file){
	return create_link_plot("Long section",plot_file);
}

XML_Node create_link_plot_drainage(Text plot_file){
	return create_link_plot("Drainage",plot_file);
}

XML_Node create_link_plot_drainage_plan(Text plot_file){
	return create_link_plot("Drainage Plan",plot_file);
}

XML_Node create_link_plot_pipeline(Text plot_file){
	return create_link_plot("Pipeline",plot_file);
}

XML_Node create_link_plot_plot_frame(Text plot_file){
	return create_link_plot("Plot Frame",plot_file);
}

XML_Node create_link_element_resolve_SA(Element &element_to_resolve){
	XML_Node node = create_chain_link("Resolve SA","Resolve "+get_full_name(element_to_resolve));
	append_element(element_to_resolve, node);
	return node;
}

// set stuff for links which already exist

Integer set_link_name(XML_Node &link, Text name){
	XML_Node name_node;
	get_create_node2(link, "Name", name_node);
	return Set_node_text(name_node,name);
}

Integer set_link_comment(XML_Node &link, Dynamic_Text comments){
	XML_Node comments_node;
	get_create_node2(link, "Comments", comments_node);
	delete_child_nodes(comments_node);
	Integer items;
	Get_number_of_items(comments,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(comments,i,t);
		Append_node(comments_node,Create_text_node("Comment",t));
	}
	return 0;
}

Integer set_link_comment(XML_Node &link, Text comment){
	Dynamic_Text dt;
	Append(comment,dt);
	return set_link_comment(link,dt);
}


//--------------------------------------------------------------------------------
//                        HEADER (mandatory)
//--------------------------------------------------------------------------------
#ifndef MY_12D_XML_VERSION
#define MY_12D_XML_VERSION "1.0"
#endif

XML_Node create_XML12D_root_node(){
	XML_Node node = Create_node("xml12d");
	Set_node_attribute(node,"xmlns","http://www.12d.com/schema/XML12d-10.0");
	Set_node_attribute(node,"xsi:schemaLocation","http://www.12d.com/schema/XML12d-10.0.xsd");
	Set_node_attribute(node,"language","English");
	Set_node_attribute(node,"version",MY_12D_XML_VERSION);
	Set_node_attribute(node,"date","");
	Set_node_attribute(node,"time","");
	return node;
}
//--------------------------------------------------------------------------------
//                        meta_data (optional)
//--------------------------------------------------------------------------------

XML_Node create_link_meta_data_units_metric(){
	XML_Node node = Create_node("units");
	XML_Node metric_node = Create_node("metric");
	Append_node(node,metric_node);

	add_xml_node(metric_node,"linear","metre");
	add_xml_node(metric_node,"area","squareMetre");
	add_xml_node(metric_node,"volume","cubicMetre");
	add_xml_node(metric_node,"temperature","celsius");
	add_xml_node(metric_node,"pressure","milliBars");
	add_xml_node(metric_node,"angular","decimal degrees");
	add_xml_node(metric_node,"direction","decimal degrees");

	return node;
}

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------

Integer append_command(XML_Document &chain, XML_Node new_command_node){
	XML_Node commands_node;
	get_create_node(chain,"Chain/Commands",commands_node);
	return Append_node(commands_node,new_command_node);
}

Integer set_meta_data_metric(XML_Document &chain){
	XML_Node meta_data_node;
	get_create_node(chain,"meta_data",meta_data_node);
	return Append_node(meta_data_node,create_link_meta_data_units_metric());
}

XML_Document create_chain(){
   XML_Node node = create_XML12D_root_node();
   XML_Document doc = Create_XML_document();
   Set_root_node(doc,node);
   //Integer Set_XML_declaration(XML_Document doc,Text version,Text encoding,Integer standalone);
   //<?xml version="1.0" encoding="UTF-8" ?>
   Text version= "1.0";
   //Text encoding="UTF-8";
   Text encoding="";
   Integer standalone = 1;
   Set_XML_declaration(doc,version, encoding, standalone);

   return doc;
}





Integer run_chain(XML_Document &chain_doc,Integer use_macro_name_in_chain_name, Integer do_file_delete){

	Text dir_name; Get_temporary_project_directory(dir_name);

	Text file_name = dir_name + "temp_chain.chain";
	Text macro_name,macro_path;	get_macro_name(macro_name,macro_path);
	if(use_macro_name_in_chain_name){
		file_name = dir_name + macro_name + "_chain.chain";
	}
 	#if USE_MACRO_NAME_IN_CHAIN_NAME
		file_name = dir_name + macro_name + "_chain.chain";
	#endif
	Integer ok=0;

	if(do_file_delete){
		append_command(chain_doc,create_link_delete_file(file_name));
	}
	#if DEBUG_CHAIN
		Print("Debugging: chain written to <debug_chain.chain>\n");
		Write_XML_document(chain_doc,"debug_chain.chain");
	#endif
	ok+=Write_XML_document(chain_doc,file_name);
	ok+=Run_chain(file_name);
	// if(do_file_delete){ // use a chain command to self delete to handle out of sequence execusion
	// 	//ok+=File_delete(file_name);
	// }
	return ok;
}

Integer run_chain(XML_Document &chain_doc){
	Integer use_macro_name_in_chain_name=0;
	Integer do_file_delete=1;
	return run_chain(chain_doc, use_macro_name_in_chain_name, do_file_delete);
}

Integer run_link_via_chain(XML_Node link_node){ // & commented out pass by reference
	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);
	append_command(chain,link_node);
	return run_chain(chain);
}

Integer run_panel_via_chain(XML_Node panel_node){ // & commented out pass by reference
	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);
	append_command(chain,create_link_run_option(panel_node));
	return run_chain(chain);
}

Integer run_panel_via_panel_prompt_as_slx(XML_Node panel_node){ // & commented out pass by reference
	Integer interactive = 0xff01; // 0xff01 stay open, 0xff00 closes
	Text panel_name;
	Text panel_data;
	panel_node_to_text(panel_node,panel_name,panel_data);
	debug_node(panel_node);
	return Panel_prompt(panel_name,interactive,panel_data);
}

Integer run_command_via_chain(XML_Node &command_node){
	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);
	append_command(chain,command_node);
	return run_chain(chain);
}

//////////////////// MORE SPECIFIC CALLS

Integer run_a_macro_via_chain(Text my_path){
	Text t="";
	XML_Node x = create_link_macro(my_path,t);
	return run_command_via_chain(x);

}

Integer run_a_macro_via_chain(Text my_path, Dynamic_Text &args){
	XML_Node x = create_link_macro(my_path,args);
	return run_command_via_chain(x);

}

Integer edit_chain_via_chain(Text file_name){
    XML_Node panel_node = slf_create_panel_node("Recalc Chain");
	slf_append_file_box		(panel_node,"Chain file",file_name);
    XML_Node link_node = create_link_option_manual(panel_node,"Recalc Chain","Edit");
    return run_link_via_chain(link_node);
}


#if XML_TEST_FUNCTION

void xml_test_function(){

// USAGE
	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);

	append_command(chain,create_link_comment("This is a comment"));	// a comment
	append_command(chain,create_link_run_chain("my_chain.chain"));	// run a chain
	append_command(chain,create_link_function("ROAD A"));		// run a function

	append_command(chain,create_link_plot_cross_section("xs.ppf"));
	append_command(chain,create_link_plot_long_section("ls.ppf"));
	append_command(chain,create_link_plot_drainage("drainage.ppf"));
	append_command(chain,create_link_plot_drainage_plan("drainage_plan.ppf"));
	append_command(chain,create_link_plot_pipeline("pipeline.ppf"));
	append_command(chain,create_link_plot_plot_frame("plot_frame.ppf"));

	Write_XML_document(chain,"my_first_chain.chain");	//writes the chain
	//or
	run_chain(chain);	// writes and runs the chain

}

/*
void xml_test_function(){
   XML_Node node = create_XML12D_root_node();

   XML_Node chain_node = Create_node("Chain");
   Append_node(node,chain_node);

   XML_Node commands_node = Create_node("Commands");
   Append_node(chain_node,commands_node);

   Append_node(commands_node,create_link_comment("This is a comment"));
   Append_node(commands_node,create_link_run_chain("my_chain.chain"));

   XML_Document  doc = Create_XML_document();
   Set_root_node(doc,node);
   Write_XML_document(doc,"my_first_chain.chain");
}
*/
#endif


#endif