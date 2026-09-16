#ifndef mashy_lib_chains_h_included
#define mashy_lib_chains_h_included

#include "mashy_lib_files.H"
#include "mashy_lib_system_calls.H"

#ifndef NEW_LINE
#define NEW_LINE "\n"
#endif


// SUPER OLD STUFF AND DUMB WAY OF RUNNING CHAINS NOW THAT WE CAN DO XML....
//  -> USE THE XML CALL AND METHOD OF RUNNING CHAINS (mashy_lib_xml*.H)

// notes
// create_chain_* creates the commands
// append_chain_* appends the command to a list

#if 0
	// usage
	Dynamic_Text dt; // the chain
	append_chain_header(dt); // append the header
	append_chain_function(dt,function_name); // add a function recalc to the chain
	run_chain(dt);	// run the chain
#endif

Integer run_chain(Dynamic_Text &dt){
// create a new chain file + run it + delete it
	Text dir_name; Get_temporary_project_directory(dir_name);

	#if USE_MACRO_NAME_IN_CHAIN_NAME
		Text macro_name,macro_path;	get_macro_name(macro_name,macro_path);
		Text file_name = dir_name + macro_name + "_chain.rcn";
	#else
		Text file_name = dir_name + "temp_chain.rcn";
	#endif

	file_write_lines(file_name,dt,"w");
	Integer ok=0;
	ok+=Run_chain(file_name);
	ok+=File_delete(file_name);
	return ok;
}


Text create_chain_header(){
	return ("3" + NEW_LINE + NEW_LINE + NEW_LINE + "0" + NEW_LINE + "0" + NEW_LINE + "$");
}

Integer append_chain_header(Dynamic_Text &dt){
// add the header
	Append(create_chain_header(),	dt);
	return 0;
}

//this will run a 1 line chain, it appends the header to the command
Integer run_chain(Text &t){
	Dynamic_Text dt;
	append_chain_header(dt);
	Append(t,dt);
	return (run_chain(dt) );
}


////////////////////////////
/////// EXECUTION
////////////////////////////

//note - the "create_*" macro calls create the _text string_
//     - the append macro calls are generally what should be called
// eg
//Dynamic_Text dt;
//append header
//append comment
//append function

Text create_chain_run_option(Text command_name, Text panel_definition){
	return ("run_option" + NEW_LINE + command_name + NEW_LINE + "1 0 0" + NEW_LINE + "" + NEW_LINE + "~" + NEW_LINE + NEW_LINE + panel_definition + NEW_LINE + NEW_LINE + "~" + NEW_LINE + "$");
}

Integer append_chain_run_option(Dynamic_Text &dt,Text command_name, Text panel_definition){
	Append(create_chain_run_option(command_name,panel_definition),dt);
	return 0;
}

Text create_chain_comment(Text command_name, Text comments){
	return ("Comment" + NEW_LINE + command_name + NEW_LINE + "1 0 0" + NEW_LINE + comments + NEW_LINE + "~" + NEW_LINE + "$");
}

Integer append_chain_comment(Dynamic_Text &dt, Text command_name, Text comments){
	Append(create_chain_comment(command_name,comments),	dt);
	return 0;
}

Text create_chain_function(Text function_name){
	return ("Function" + NEW_LINE + "Recalc "+function_name + NEW_LINE + "1 0 0" + NEW_LINE + NEW_LINE + "~" + NEW_LINE + function_name + NEW_LINE + "$");
}


Integer append_chain_function(Dynamic_Text &dt, Text function_name){
	Append(create_chain_function(function_name),	dt);
	return 0;
}

Text create_chain_run_macro(Text macro_name, Text arguments){
	return ("run_macro" + NEW_LINE + "Macro "+macro_name + NEW_LINE + "1 0 0" + NEW_LINE + "" + NEW_LINE + "~" + NEW_LINE + macro_name + NEW_LINE + arguments + NEW_LINE + "$");
}

Integer append_chain_run_macro(Dynamic_Text &dt, Text macro_name, Text arguments){
	Append(create_chain_run_macro(macro_name, arguments),	dt);
	return 0;
}

Text create_chain_run_chain( Text chain_name){
	return ("chain" + NEW_LINE + "Recalc chain "+chain_name + NEW_LINE + "1 0 0" + NEW_LINE + "" + NEW_LINE + "~" + NEW_LINE + chain_name + NEW_LINE + "" + NEW_LINE + "$");
}


Integer append_chain_run_chain(Dynamic_Text &dt, Text chain_name){
	Append(create_chain_run_chain( chain_name),	dt);
	return 0;
}

////////////////////////////
/////// PLOTTING
////////////////////////////

Text create_chain_plot(Text type, Text file){
	return(
	"plot" + NEW_LINE +
	"Plot "+ type + " " + file + NEW_LINE +
	"1 0 0" + NEW_LINE +
	"" + NEW_LINE +
	"~" + NEW_LINE +
	type + NEW_LINE +
	file + NEW_LINE +
	"~" + NEW_LINE +
	"$"
	);
}

Integer append_chain_plot(Dynamic_Text &dt, Text type, Text file){
	// + valid types +
	//Cross section
	//Long section
	//Plot Frame
	//Drainage
	//Drainage Plan
	//Melbourne Water
	//Pipeline
	Append(create_chain_plot(type, file),	dt);
	return 0;
}


////////////////////////////
/////// DRAINAGE
////////////////////////////

Text create_chain_drainage_analysis(Text model_name){
	return (
	"drainage_analysis" + NEW_LINE +
	"Run drainage analysis on "+ model_name + NEW_LINE +
	"1 0 0" + NEW_LINE +
	"" + NEW_LINE +
	"~" + NEW_LINE +
	model_name + NEW_LINE +
	"$"
	);
}

Integer append_chain_drainage_analysis(Dynamic_Text &dt, Text model_name){
	Append(create_chain_drainage_analysis(model_name),	dt);
	return 0;
}

Text create_chain_drainage_analysis_dynamic(Text model_name){
	return (
	"drainage_analysis_dynamic" + NEW_LINE +
	"Run drainage analysis - dynamic on "+ model_name + NEW_LINE +
	"1 0 0" + NEW_LINE +
	"" + NEW_LINE +
	"~" + NEW_LINE +
	model_name + NEW_LINE +
	"$"
	);
}

Integer append_chain_drainage_analysis_dynamic(Dynamic_Text &dt, Text model_name){
	Append(create_chain_drainage_analysis_dynamic(model_name),	dt);
	return 0;
}

Text create_chain_set_pit_details(Text model_name){
	return (
	"set_pit_details" + NEW_LINE +
	"Run set pit details on "+ model_name + NEW_LINE +
	"1 0 0" + NEW_LINE +
	"" + NEW_LINE +
	"~" + NEW_LINE +
	model_name + NEW_LINE +
	"$"
	);
}

Integer append_chain_set_pit_details(Dynamic_Text &dt, Text model_name){
	Append(create_chain_set_pit_details(model_name),	dt);
	return 0;
}

Text create_chain_regrade_pipes(Text model_name){
	return (
	"regrade_pipes" + NEW_LINE +
	"Run regrade pipes on "+ model_name + NEW_LINE +
	"1 0 0" + NEW_LINE +
	"" + NEW_LINE +
	"~" + NEW_LINE +
	model_name + NEW_LINE +
	"$"
	);
}

Integer append_chain_regrade_pipes(Dynamic_Text &dt, Text model_name){
	Append(create_chain_regrade_pipes(model_name),	dt);
	return 0;
}
//--------------------------------------------------------------------------------
//                        Non standard links
//--------------------------------------------------------------------------------
Text create_chain_retriangulate_tin(Text option_name, Text tin_name){
	return(
	"run_option"					+ NEW_LINE +
	option_name					+ NEW_LINE +
	"1 0 0"						+ NEW_LINE +
	""						+ NEW_LINE +
	"~"						+ NEW_LINE +
	""						+ NEW_LINE +
	"Panel \"Retriangulate Tin\" {"			+ NEW_LINE +
	""						+ NEW_LINE +
	"  Widget_Pages \"Tabs\" {"			+ NEW_LINE +
	""						+ NEW_LINE +
	"    Field \"Current_Page_Name\" \"General\""	+ NEW_LINE +
	""						+ NEW_LINE +
	"    Widget_Page_Name \"General\" {"		+ NEW_LINE +
	"      Field    \"Tin\"        \""+tin_name+"\""+ NEW_LINE +
	"    }"						+ NEW_LINE +
	"}"						+ NEW_LINE +
	"  Run      \"Retriangulate\""			+ NEW_LINE +
	"}"						+ NEW_LINE +
	""						+ NEW_LINE +
	"~"						+ NEW_LINE +
	"$"
	);
}

Integer append_chain_retriangulate_tin(Dynamic_Text &dt, Text option_name, Text tin_name){
	Append(create_chain_retriangulate_tin(option_name,tin_name),	dt);
	return 0;
}

////////////////////////////
/////// PRE-DEFINED OPERATIONS
////////////////////////////

Integer run_panel_via_chain(Text panel_definition){
	Dynamic_Text dt;
	append_chain_header(dt);
	append_chain_run_option(dt,"macro_defined_command", panel_definition);
	run_chain(dt);
	return 0;
}


#endif
