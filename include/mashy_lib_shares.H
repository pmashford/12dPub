#ifndef mashy_lib_shares_H_included
#define mashy_lib_shares_H_included

#include "12d/slf.H"
#include "mashy_lib_files.H"
#include "mashy_lib_system_calls.H"
#include "mashy_lib_chains.H"
#include "mashy_lib_xml_v10.H"

#ifndef USE_CHAIN_INSTEAD_OF_PANEL_PROMPT
#define USE_CHAIN_INSTEAD_OF_PANEL_PROMPT 0
#endif


Integer unshare_model_v15(Dynamic_Text &model_names){
	Text data;
	pad = 2;
	data += field("Pattern"    ,"");
	data += group("Grid","Share models");
		data += " Columns"+spacer+"{"+new_line+spacer+"\"Share\""+new_line+spacer+"\"Model\""+new_line+spacer+"\"Share as\""+new_line+spacer+"}"+new_line;
		data += " Data"+spacer+"{"+new_line;

	Integer items;
	Get_number_of_items(model_names,items);
	for(Integer i=1;i<=items;i++){
		Text model_name;
		Get_item(model_names,i,model_name);
			data += "{"+spacer+"\"0\""+spacer+"\""+model_name+"\""+spacer+"\"\""+spacer+"}"+new_line;
	}
	data += "}"+new_line;
	data += group();
	Text    name        = "Share Project Models";
	Integer interactive = 1;
#if USE_CHAIN_INSTEAD_OF_PANEL_PROMPT
	return run_panel_via_chain(data);
#else
	return Panel_prompt(name,interactive,data);
#endif
}

Integer unshare_model_v15(Text &model_name)
{
	Dynamic_Text model_names;
	Append(model_name,model_names);
	return unshare_model_v15(model_names);
}

// THIS CANT WORK AS THE PANEL PROMPT COMES UP WITH MODEL IS LOCKED!!
Integer unshare_model(Dynamic_Text &model_names){
	Text data;
	pad = 2;

	data += field("Match sub strings" ,"false");
	data += field("Search"    ,"");
	data += field("Pattern expression","true");
	data += field("Search"      ,"");
	data += field("Regular expression","false");
	data += group("Grid","Share Models");
		data += " Columns"+spacer+"{"+new_line+spacer+"\"Remove\""+new_line+spacer+"\"Model name\""+new_line+spacer+"\"Share path\""+new_line+spacer+"\"Status\""+new_line+spacer+"}"+new_line;
		data += " Data"+spacer+"{"+new_line;

	Integer items;
	Get_number_of_items(model_names,items);
	for(Integer i=1;i<=items;i++){
		Text model_name;
		Get_item(model_names,i,model_name);
			data += "{"+spacer+"\"1\""+spacer+"\""+model_name+"\""+spacer+"\"\""+spacer+"\"\""+spacer+"}"+new_line;
			// the problem doesn't seem to be related to the path field missing in the grid (at least in v9)
			//data += "{"+spacer+"\"1\""+spacer+"\""+model_name+"\""+spacer+"\"..\\\\drainage\\\\drainage.project\\\\drainage_n1_data_catchments\""+spacer+"\"\""+spacer+"}"+new_line;
			//..\\drainage\\drainage.project\\drainage_n1_data_catchments
	}

		data += "}"+new_line;
	data += group();
	Text    name        = "Remove Shared Models from Project";
	Integer interactive = 1;
#if USE_CHAIN_INSTEAD_OF_PANEL_PROMPT
	return run_panel_via_chain(data);
#else
	return Panel_prompt(name,interactive,data);
#endif

}

Integer unshare_model(Text &model_name)
{
	Dynamic_Text model_names;
	Append(model_name,model_names);
	return unshare_model(model_names);
}


Integer share_model(Text &model_name,Integer mode){
	Text data;
	pad = 2;
	//data += field("Pattern","\'\'");
	data += group("Grid","Share Models");
		data += " Columns"+spacer+"{"+new_line+spacer+"\"Share\""+new_line+spacer+"\"Model\""+new_line+spacer+"}"+new_line;
		data += " Data"+spacer+"{"+new_line;
			data += "{"+spacer+"\""+To_text(mode)+"\""+spacer+"\""+model_name+"\""+spacer+"}"+new_line;
		data += "}"+new_line;
	data += group();
	Text    name        = "Share Project Models";
	Integer interactive = 0;
#if USE_CHAIN_INSTEAD_OF_PANEL_PROMPT
	return run_panel_via_chain(data);
#else
	return Panel_prompt(name,interactive,data);
#endif
}

Integer share_model(Text &model_name){
	return(share_model(model_name,1));
}

Integer share_model_off(Text &model_name){
	return(share_model(model_name,0));
}

Integer share_in_models(Dynamic_Text &model_list, Text folder, Text project){
	Integer show_console = Is_console_visible();

	Integer items;
	Get_number_of_items(model_list,items);
	Dynamic_Integer add;
	Dynamic_Text original_model,new_model_name,status;
	for(Integer i=1;i<=items;i++){
		Text this_model;
		Get_item(model_list,i,this_model);
		if(Model_exists(this_model)){
			Print("Model <"+this_model+"> already exists in this project and will be skipped!\n");
			continue;
		}else{
			Append(this_model,original_model);
			Append(1,add);
			Append("",new_model_name);
			Append("",status);
		}
	}
	Integer match_sub_strings = 0, pattern_expression = 1, regular_expression = 1;
	Text pattern_search = "", pattern_replace = "", regex_search = "", regex_replace = "";
	XML_Node panel_node = slf_add_shared_models_to_project( folder, project, match_sub_strings,
													pattern_expression, pattern_search, pattern_replace,
													regular_expression, regex_search, regex_replace,
													add, original_model, new_model_name, status);

	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);
	append_command(chain,create_link_run_option(panel_node));
	Integer ok = 0;
	if(run_chain(chain)){	// run check for errors
		Print("Failed to read models in from project <"+folder+"\\"+project+">\n !");
		ok = 1;
	}
	if(!show_console)Show_console(0);
	return 0;
}

#if 1
Integer share_in_model(Text &model_name, Text folder, Text project){
	Dynamic_Text model_list;
	Append(model_name,model_list);
	return share_in_models(model_list, folder, project);
}
#else
Integer share_in_model(Text &model_name, Text project_path, Text project_name){
	Text data;
	pad = 2;
	data += field("Folder",project_path);
	data += field("Project",project_name);

	data += field("Match sub strings" ,"false");
	data += field("Pattern Search"    ,"");
	data += field("Pattern Replace"   ,"");
	data += field("Pattern expression","true");
	data += field("Regex Search"      ,"");
	data += field("Regex Replace"     ,"");
	data += field("Regular expression","false");
	data += group("Grid","Share Models");
		data += " Columns"+spacer+"{"+new_line+spacer+"\"Add\""+new_line+spacer+"\"Original Model Name\""+new_line+spacer+"\"New Model Name\""+new_line+spacer+"\"Status\""+new_line+spacer+"}"+new_line;
		data += " Data"+spacer+"{"+new_line;
			data += "{"+spacer+"\"1\""+spacer+"\""+model_name+"\""+spacer+"\"\""+spacer+"\"\""+spacer+"}"+new_line;
		data += "}"+new_line;
	data += group();
	Text    name        = "Add Shared Models to Project";
	Integer interactive = 0;
#if USE_CHAIN_INSTEAD_OF_PANEL_PROMPT
	return run_panel_via_chain(data);
#else
	return Panel_prompt(name,interactive,data);
#endif

}
#endif

Integer share_in_tins(Dynamic_Text &tin_list, Text folder, Text project){
	Integer show_console = Is_console_visible();

	Integer items;
	Get_number_of_items(tin_list,items);
	Dynamic_Integer add;
	Dynamic_Text original_name,new_name,model_name,status;
	for(Integer i=1;i<=items;i++){
		Text this_name;
		Get_item(tin_list,i,this_name);
		if(Tin_exists(this_name)){
			Print("Tin <"+this_name+"> already exists in this project and will be skipped!\n");
			continue;
		}else{
			Append(this_name,original_name);
			Append(1,add);
			Append("",new_name);
			Append("tin " + this_name,model_name);
			Append("",status);
		}
	}
	Integer match_sub_strings = 0, pattern_expression = 1, regular_expression = 1;
	Text pattern_search = "", pattern_replace = "", regex_search = "", regex_replace = "";
	XML_Node panel_node = slf_add_shared_tins_to_project( folder, project, match_sub_strings,
													pattern_expression, pattern_search, pattern_replace,
													regular_expression, regex_search, regex_replace,
													add, original_name, new_name, model_name, status);

	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);
	append_command(chain,create_link_run_option(panel_node));
	Integer ok = 0;
	if(run_chain(chain)){	// run check for errors
		Print("Failed to read tins in from project <"+folder+"\\"+project+">\n !");
		ok = 1;
	}
	if(!show_console)Show_console(0);
	return 0;
}


Integer share_tin(Text &tin_name,Integer mode){
	Text data;
	pad = 2;
	//data += field("Pattern","\'\'");
	data += group("Grid","Share Tins");
		data += " Columns"+spacer+"{"+new_line+spacer+"\"Share\""+new_line+spacer+"\"Tin\""+new_line+spacer+"}"+new_line;
		data += " Data"+spacer+"{"+new_line;
			data += "{"+spacer+"\""+To_text(mode)+"\""+spacer+"\""+tin_name+"\""+spacer+"}"+new_line;
		data += "}"+new_line;
	data += group();
	Text    name        = "Share Project Tins";
	Integer interactive = 0;
#if USE_CHAIN_INSTEAD_OF_PANEL_PROMPT
	return run_panel_via_chain(data);
#else
	return Panel_prompt(name,interactive,data);
#endif

}

Integer share_tin(Text &tin_name){
	return(share_tin(tin_name,1));
}

Integer share_tin_off(Text &tin_name){
	return(share_tin(tin_name,0));
}

Integer share_in_tin(Text &tin_name, Text project_path, Text project_name, Text prefix){
	Text data;
	pad = 2;
	data += field("Folder",project_path);
	data += field("Project",project_name);

	data += field("Match sub strings" ,"false");
	data += field("Pattern Search"    ,"");
	data += field("Pattern Replace"   ,"");
	data += field("Pattern expression","true");
	data += field("Regex Search"      ,"");
	data += field("Regex Replace"     ,"");
	data += field("Regular expression","false");
	data += group("Grid","Share Tins");
		data += " Columns"+spacer+"{"+new_line+spacer+"\"Add\""+new_line+spacer+"\"Original Tin Name\""+new_line+spacer+"\"New Tin Name\""+new_line+spacer+"\"Model for Tin\""+new_line+spacer+"\"Status\""+new_line+spacer+"}"+new_line;
		data += " Data"+spacer+"{"+new_line;
			data += "{"+spacer+"\"1\""+spacer+"\""+tin_name+"\""+spacer+"\"\""+spacer+"\""+prefix+tin_name+"\""+spacer+"\"\""+spacer+"}"+new_line;
		data += "}"+new_line;
	data += group();
	Text    name        = "Add Shared Tins to Project";
	Integer interactive = 0;
#if USE_CHAIN_INSTEAD_OF_PANEL_PROMPT
	return run_panel_via_chain(data);
#else
	return Panel_prompt(name,interactive,data);
#endif

}

Integer get_shared_models(Text dot_project_path,Dynamic_Text &names){
	Text command = "dir \"" + dot_project_path + "\\*.Model.ref\" /ogn /ad /s /b";
	Dynamic_Text report = run_system_command_get_output(command);
	Integer items;
	Get_number_of_items(report,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(report,i,t);
		Dynamic_Text parts;
		Integer num_parts;
		get_file_path_parts(t,parts);
		Get_number_of_items(parts,num_parts);
		Text model_name;
		Get_item(parts,num_parts,model_name);
		Append(find_replace(Get_subtext(model_name,1,Text_length(model_name)-10),"_"," "),names);
	}
	Get_number_of_items(names,items);
	if(items<1)return(1);
	return(0);
}

Integer get_shared_models(Dynamic_Text &names){
	return (get_shared_models(get_dot_project_folder(),names));
}

Integer get_shared_tins(Text dot_project_path,Dynamic_Text &names){
	Text command = "dir \"" + dot_project_path + "\\*.Tin.ref\" /ogn /ad /s /b";
	Dynamic_Text report = run_system_command_get_output(command);
	Integer items;
	Get_number_of_items(report,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(report,i,t);
		Dynamic_Text parts;
		Integer num_parts;
		get_file_path_parts(t,parts);
		Get_number_of_items(parts,num_parts);
		Text model_name;
		Get_item(parts,num_parts,model_name);
		Append(find_replace(Get_subtext(model_name,1,Text_length(model_name)-8),"_"," "),names);
	}
	Get_number_of_items(names,items);
	if(items<1)return(1);
	return(0);
}

Integer get_shared_tins(Dynamic_Text &names){
	return (get_shared_tins(get_dot_project_folder(),names));
}


// binds a shared model

Integer bind_model(Text model_name){
	Model m = Get_model(model_name);
	Uid uid;
	Get_id(m,uid);
	if(Is_null(uid))return 1;
	// is model shared?
	if(!Is_global(uid))return 2;
	// do the copy
	Text temporary_model_name = model_name;
	Integer suffix = 123;
	while(1){
		Text this_name = temporary_model_name+To_text(suffix);
		if(Model_exists(this_name))
		{
			suffix++;
			continue;
		}
		temporary_model_name = this_name;
		break;
	}

	if(Model_duplicate(m,temporary_model_name))return 3;
	//unshare
	unshare_model(model_name); // cant really tell if this fails - maybe check if model exists?
	if(Model_exists(model_name)) return 4;
	//rename
	if(Model_rename(temporary_model_name,model_name)) return 5;
	return 0;
}

#endif