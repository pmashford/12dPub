#ifndef mashy_lib_about_panel_included
#define mashy_lib_about_panel_included

#include "mashy_lib_element_functions.h"
#include "mashy_lib_files.H"

//THIS SHOULD BE AT THE START OF EVERY MACRO
#ifndef ABOUT_PANEL_DATA_INCLUDED
#define ABOUT_PANEL_DATA_INCLUDED
{
	Text about_macro_by = "Paul Mashford";
	Text about_macro_for = "ME";
	Text about_macro_description = "MY Generic about panel";
	Text about_macro_changes = "";
}
#endif

#if 0
/*		USAGE		*/
		switch(id){
			case Get_id(panel) : {
				if(cmd == "Panel Quit") return(1);
				if(cmd == "Panel About")	manage_about_panel();
			} break;

#endif

void format_logo_element(Element &e, Integer colour){
	Set_colour(e,colour);
	Set_super_use_solid(e,1);
	Set_super_solid_colour(e,colour);
	Set_super_solid_blend(e,1.0);
}

Element create_logo_element(Integer flags, Real x[], Real y[], Real z[], Real r[], Integer f[], Integer idx[], Integer size){
    Real dx[size],dy[size];
    for(Integer i=1;i<=size;i++){
        dx[i]=x[idx[i]];    dy[i]=y[idx[i]];
    }
    return Create_super(flags,dx,dy,z,r,f,size);
}

// return a logo as 12d elements
Dynamic_Element create_logo(){

    Integer S = 13;
    Real x[S],y[S];

    x[ 1]=2.000   ;  y[ 1]=4.000 ;
    x[ 2]=2.000   ;  y[ 2]=0.000 ;
    x[ 3]=3.948   ;  y[ 3]=2.266 ;
    x[ 4]=3.948   ;  y[ 4]=5.274 ;
    x[ 5]=1.488   ;  y[ 5]=5.747 ;
    x[ 6]=-0.776  ;  y[ 6]=4.831 ;
    x[ 7]=-0.776  ;  y[ 7]=1.550 ;
    x[ 8]=2.000   ;  y[ 8]=2.028 ;
    x[ 9]=2.974   ;  y[ 9]=2.910 ;
    x[10]=2.974   ;  y[10]=4.637 ;
    x[11]=1.667   ;  y[11]=4.954 ;
    x[12]=0.612   ;  y[12]=4.415 ;
    x[13]=0.612   ;  y[13]=2.621 ;


    Integer a[4];	a[1] = 1; a[2] = 2; a[3] = 3; a[4] = 4;
    Integer b[4];	b[1] = 4; b[2] = 5; b[3] = 6; b[4] = 1;
    Integer c[4];	c[1] = 6; c[2] = 7; c[3] = 2; c[4] = 1;

    Integer A[4];	A[1] = 1; A[2] = 8; A[3] = 9; A[4] = 10;
    Integer B[4];	B[1] = 10; B[2] = 11; B[3] = 12; B[4] = 1;
    Integer C[4];	C[1] = 12; C[2] = 13; C[3] = 8; C[4] = 1;

	Integer flags = 1;

	Real z[S],r[S];
	Integer f[S];
	for(Integer i=1;i<=S;i++){
		z[i] = 0.0;	r[i] = 0.0;	f[i] = 0;
	}

	Element ea = create_logo_element(flags,x,y,z,r,f,a,4);
    Element eb = create_logo_element(flags,x,y,z,r,f,b,4);
    Element ec = create_logo_element(flags,x,y,z,r,f,c,4);
    Element eA = create_logo_element(flags,x,y,z,r,f,A,4);
    Element eB = create_logo_element(flags,x,y,z,r,f,B,4);
    Element eC = create_logo_element(flags,x,y,z,r,f,C,4);

	Integer colour;
	if(Convert_colour("purple",colour))colour=1;
	format_logo_element(ea,colour);
    if(Convert_colour("blue",colour))colour=1;
	format_logo_element(eb,colour);
    if(Convert_colour("green",colour))colour=1;
	format_logo_element(ec,colour);
    if(Convert_colour("grey",colour))colour=1;
	format_logo_element(eA,colour);
    if(Convert_colour("grey",colour))colour=1;
	format_logo_element(eB,colour);
    if(Convert_colour("white",colour))colour=1;
	format_logo_element(eC,colour);

	Dynamic_Element delt;
	Append(ea,delt);
	Append(eb,delt);
	Append(ec,delt);
	Append(eA,delt);
	Append(eB,delt);
	Append(eC,delt);

	return delt;
}

Text get_about_panel_build_date(){
	return __DATE__ + " " + __TIME__;
}

#ifndef DRAW_BOX_FACTOR
#define DRAW_BOX_FACTOR 0.9
#endif

Integer draw_draw_box(Draw_Box &db, Dynamic_Element &delt,Integer scale_x_and_y, Integer delete_delt){
//scale_x_and_y - TRUE means that draw box x and y scaled togeather
//              - FALSE means that the x and y scale may be different, ie distorted
	Real x_min, x_max, y_min, y_max;
	Integer x,y;
	Get_size(db,x,y);
	get_extents(delt,x_min,x_max,y_min,y_max);
	Real x_scale = x / (x_max - x_min) * DRAW_BOX_FACTOR;
	Real y_scale = y/(y_max - y_min) * DRAW_BOX_FACTOR;
	if(!scale_x_and_y){
		Real scale = (x_scale > y_scale) ? y_scale : x_scale;
		x_scale = scale;
		y_scale = scale;
	}
	Real dx = (0 - x_min) * x_scale;
	Real dy = (0 - y_min) * y_scale;
//	if(!scale_x_and_y){
		dy += (y - (y_max - y_min) * y_scale) / 2;
		dx += (x - (x_max - x_min) * x_scale) / 2;
//	}
	Affine(delt,0,0,x_scale,y_scale,dx,dy);
	Start_batch_draw(db);
	Clear(db,0,0,0);
	Integer items;	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;	Get_item(delt,i,e);
		Draw_element(db,e);
	}
	End_batch_draw(db);
	if(delete_delt)elements_delete(delt);
	return(0);
}

void draw_about_panel_draw_box_logo(Draw_Box &db){
	Dynamic_Element delt = create_logo();
	Integer scale_x_and_y = 0;
	Integer delete_delt = 1;
	draw_draw_box(db, delt, scale_x_and_y,  delete_delt);
}


Text get_about_panel_12d_info(){
	/*
	Integer Get_program_version_number       () L_Get_Program_Version_Number;
	Integer Get_program_major_version_number () L_Get_Program_Major_Version_Number;
	Integer Get_program_minor_version_number () L_Get_Program_Minor_Version_Number;
	Integer Get_program_folder_version_number() L_Get_Program_Folder_Version_Number;
	Integer Get_program_build_number         () L_Get_Program_Build_Number;
	Text    Get_program_special_build_name   () L_Get_Program_Special_Build_Name;
	Text    Get_program_patch_version_name   () L_Get_Program_Patch_Version_Name;
	Text    Get_program_full_title_name      () L_Get_Program_Full_Title_Name;
	Text    Get_program                      () L_Get_Program;
	Text    Get_program_name                 () L_Get_Program_Name;
	Text    Get_program_folder               () L_Get_Program_Folder;
	Text    Get_program_parent_folder        () L_Get_Program_Parent_Folder;
	*/
	Text t="";
	//t+="Build name: " +	Get_program_special_build_name()+"\n";
	t+="12d Version: " +		Get_program_full_title_name()+"\n";
	t+="12d Patch: " +		Get_program_patch_version_name()+"\n";
	t+="12d exe: " +	Get_program()+"\n";
	//t+="Name: " +		Get_program_name()+"\n";
	//t+="Foler: " +		Get_program_folder()+"\n";
	t+="12d Path: " +	Get_program_parent_folder()+"\n";

	Text user_name,project_name,project_path;
	Get_user_name(user_name);
	Get_project_name(project_name);
	Get_project_folder(project_path);

	t+="\n";
	t+="Macro path: "+Get_macro_name()+"\n";
	t+="User: "+user_name+"\n";
	t+="Project Name: "+project_name+"\n";
	t+="Project Path: "+project_path+"\n";

	return t;
}


Integer manage_about_panel(){

	Panel panel = Create_panel(about_macro_for + " | About Macro");
	Message_Box message_box = Create_message_box("");
	Vertical_Group vg_all = Create_vertical_group(0);

	Vertical_Group vg = Create_vertical_group(0);
	Horizontal_Group hg_buttons = Create_button_group();

	Draw_Box draw_box = Create_draw_box(250,120,0);

	Tab_Box tab_box = Create_tab_box();
	Vertical_Group tab1 = Create_vertical_group(0);
	Vertical_Group tab2 = Create_vertical_group(0);
	Vertical_Group tab3 = Create_vertical_group(0);
	Vertical_Group tab4 = Create_vertical_group(0);
	Vertical_Group tab5 = Create_vertical_group(0);
	Append(tab1,"Description",tab_box);
	Append(tab2,"Info",tab_box);
	Append(tab3,"Build",tab_box);
	Append(tab4,"12d Info",tab_box);
	//if(about_macro_changes != "")Append(tab5,"Changes",tab_box);

	Integer no_lines = 7;
	Text_Edit_Box description_box = Create_text_edit_box("Description",message_box,no_lines);	Set_read_only(description_box,1);	Set_word_wrap(description_box,1);
	Append(description_box,tab1);
//DESCRIPTION
	Text description_text="";
	description_text+=about_macro_description;
	Set_data(description_box,about_macro_description);

	Text_Edit_Box info_box = Create_text_edit_box("Description",message_box,no_lines);	Set_read_only(info_box,1);
	Append(info_box,tab2);
//INFO
	Text info_text = "";
	info_text+="For: " + about_macro_for + "\n";
	info_text+="by: " + about_macro_by + "\n";
	Set_data(info_box,info_text);


	Text_Edit_Box build_box = Create_text_edit_box("Description",message_box,no_lines);	Set_read_only(build_box,1);
	Append(build_box,tab3);
//BUILD
	Text build_text = "";
	build_text+="Compiler version: " + To_text(VERSION_4D) + "\n";
#ifdef QUOTED_CC4D_VERSION_DATA
  build_text+="Compiler info: " + QUOTED_CC4D_VERSION_DATA + "\n";
#endif
  //build_text+="Compiler text: " + To_text(QUOTED_CC4D_VERSION_DATA) + "\n";
	build_text+="Compiled: " + get_about_panel_build_date() + "\n";
	//build_text+="From: " + find_replace(__FILE__,"\\","\\\\") + "\n";
	Set_data(build_box,build_text);


	Text_Edit_Box info12d_box = Create_text_edit_box("Description",message_box,no_lines);	Set_read_only(info12d_box,1);
	Append(info12d_box,tab4);
//12D INFO
	Text info12d_text = "";
	info12d_text+=get_about_panel_12d_info();
	Set_data(info12d_box,info12d_text);


	Append(draw_box,vg);
	Append(tab_box,vg);

	Text email_new_line = "%0A";
	Text email_guts = "?subject=Macro: " + get_macro_name_text() + "&body=";
	email_guts+= "Comments:\n\n\n";
	email_guts+= info_text + "\n" + build_text + "\n" + info12d_text;
	email_guts = find_replace(email_guts,"\n", email_new_line);


	HyperLink_Box hyperlink_box = Create_hyperlink_box   ("title",message_box);	Set_width_in_chars(hyperlink_box,10);
	Set_data(hyperlink_box,"mailto:p.mashford@gmail.com"+email_guts);
	Append(hyperlink_box,vg);

	Append(vg,vg_all);

	//Append(message_box,vg_all);

	Append(hg_buttons,vg_all);

	Append(vg_all,panel);

	Show_widget(panel);
	draw_about_panel_draw_box_logo(draw_box);

	while(1){
		Integer id;
		Text cmd,msg;
		Wait_on_widgets(id,cmd,msg);
		// Print("Id=" + To_text(id) + " cmd=" + cmd + " msg=" + msg + "\n");
		if(cmd == "keystroke") continue;
		if(cmd == "Panel Quit") return(0);
        if(cmd == "CodeShutdown"){
#if VERSION_4D > 1400
            Set_exit_code(cmd);
#endif
        }
	}
	return(0);
}

#endif