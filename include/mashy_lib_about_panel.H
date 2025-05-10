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

void draw_polygon(Draw_Box &box, Vector4 v[] , Integer start_index, Integer count){
	Real x[count],y[count],z,r;
	Integer max = start_index + count - 1;
	for(Integer i=1 ; i<= count ;i++){
		Get_vector(v[start_index],x[i],y[i],z,r);
		start_index++;
	}
	Draw_polygon(box,4,x,y);
}

#include "mashy_lib_vectors_matrix.H"

void draw_polygon(Draw_Box &box, Vector4 v[] , Vector4 f[], Vector3 c[], Integer fid, Real b_0_1){
	// Integer count = 0;
	Real r,g,b,a;
	
	Get_vector(f[fid],r,g,b,a);
	Real x[4],y[4],z[4],zz;
	Get_vector(v[r],x[1],y[1],z[1],zz);
	Get_vector(v[g],x[2],y[2],z[2],zz);
	Get_vector(v[b],x[3],y[3],z[3],zz);
	// Real nz = (x[2] - x[1])*(y[3] - y[1]) - (x[3] - x[1])*(y[2] - y[1]);
	if( (x[2] - x[1])*(y[3] - y[1]) - (x[3] - x[1])*(y[2] - y[1]) < 0.0){ // check normal z
		// if the normal points down dont draw
		return;
	}
	Get_vector(v[a],x[4],y[4],z[4],zz);
	Get_vector(c[fid],r,g,b);
	// b_0_1 = b_0_1 * b_0_1; // always +ve and scales closer to 0.0 (I think)
	// r = r + (255 - r - r) * b_0_1; // gets the inverse colour factored by b_0_1
	// g = g + (255 - g - g) * b_0_1; // gets the inverse colour factored by b_0_1
	// b = b + (255 - b - b) * b_0_1; // gets the inverse colour factored by b_0_1

	Set_fill_colour(box,r,g,b);
	Set_colour(box,r,g,b);
	// Set_colour(box,240,240,240);
	Draw_polygon(box,4,x,y);
}

Real normalize_linear(Real number, Real min_val, Real max_val) { //  get a value between -1.0 and 1.0
	if (max_val == min_val) {
	  return 0.; // Avoid division by zero
	}
	return (2. * (number - min_val) / (max_val - min_val) - 1.);
}

{
	Real GLOBAL_FPS_LAST_TIME = 0xFFFFFFFF;
}

Real get_fps(){
	if(GLOBAL_FPS_LAST_TIME == 0xFFFFFFFF){
		Get_time(GLOBAL_FPS_LAST_TIME);
		return 0xFFFFFFFF;
	}
	Real fps;
	Get_time(fps);
	Real diff = 1.0 / (fps - GLOBAL_FPS_LAST_TIME);
	GLOBAL_FPS_LAST_TIME = fps;
	return diff;

}

void draw_box_text_setup(Draw_Box box){
	Set_text_colour(box,123,123,133);
	Set_text_font(box,"Arial");
	// Set_text_weight(box,0);
	// Set_text_align(box,0);
}


void draw_about_panel_draw_box_logo2(Draw_Box &box, Integer &cursor_x, Integer &cursor_y, Real &spin, Integer use_spin){
	Integer box_width,box_height;
	Get_size(box,box_width,box_height);
	Real centre_x = box_width / 2;
	Real centre_y = box_height / 2;

	Integer minimum_size = (box_width < box_height) ? box_width : box_height;

	Real delta_x = cursor_x - centre_x;
	Real delta_y = cursor_y - centre_y;

	Vector4 vertz[8];
	Set_vector(vertz[1],	-.5,	-.5,	0.5,	1.0);
	Set_vector(vertz[2],	0.5,	-.5,	0.5,	1.0);
	Set_vector(vertz[3],	0.5,	0.5,	0.5,	1.0);
	Set_vector(vertz[4],	-.5,	0.5,	0.5,	1.0);
	Set_vector(vertz[5],	-.5,	-.5,	-.5,	1.0);
	Set_vector(vertz[6],	0.5,	-.5,	-.5,	1.0);
	Set_vector(vertz[7],	0.5,	0.5,	-.5,	1.0);
	Set_vector(vertz[8],	-.5,	0.5,	-.5,	1.0);
	Vector4 facez[6]; // quad of verticies in anti clockwise order sets up a faces
	Vector3 cols[6];
	Set_vector(facez[1],	1,	2,	3,	4);		Set_vector(cols[1],	0,122,255	);
	Set_vector(facez[2],	1,	5,	6,	2);		Set_vector(cols[2],	255,76,76	);//g
	Set_vector(facez[3],	2,	6,	7,	3);		Set_vector(cols[3],	187,51,255	); //b
	Set_vector(facez[4],	3,	7,	8,	4);		Set_vector(cols[4],	56,220,78	);
	Set_vector(facez[5],	4,	8,	5,	1);		Set_vector(cols[5],	255,204,0	);
	Set_vector(facez[6],	8,	7,	6,	5);		Set_vector(cols[6],	0,210,211	);//w
	Matrix4 m;
	Vector4 vertz2[8]; // mini box
	vertz2[1] = vertz[1]; vertz2[2] = vertz[2]; vertz2[3] = vertz[3]; vertz2[4] = vertz[4];	vertz2[5] = vertz[5]; vertz2[6] = vertz[6]; vertz2[7] = vertz[7]; vertz2[8] = vertz[8];
	m = get_rotate_scale_matrix( 1.0 ,  0.0 , 1.0, Pi(),  0.50 );
	matmul(vertz2, 8, m);
	m = get_rotate_scale_matrix( 0.0 ,  1.0 , 1.0, Pi(),  1.0 );
	matmul(vertz2, 8, m);
	// m = get_translation_matrix(0.375,0.375,0.375 ); 
	m = get_translation_matrix(0.25,0.25,0.25 ); 
	matmul(vertz2, 8, m);

	Real sf=minimum_size*0.5;

	Real nx = normalize_linear(cursor_x, 0.0, box_width);
	Real ny = normalize_linear(cursor_y, 0.0, box_height);
	Real spin2 =  2.0 * Pi() * ny*nx; // spin the box(s)
	if(use_spin){
		spin2 = spin;
	}else{
		spin = spin2;
	}
	// Matrix4 m = get_rotate_scale_matrix( nx ,  ny , 0., Pi() * (Absolute(ny)*nx),  sf );
	m = get_rotate_scale_matrix( ny * box_width ,  nx * box_height , 1.0, spin2,  sf );
	matmul(vertz, 8, m);
	matmul(vertz2, 8, m);
	
	// m = Get_perspective_matrix(2.5);
	// Real angle_of_view = 90.,   near = 2. * sf,   far =5. * sf;
	// m = get_projection_matrix(angle_of_view,  near,  far );
	sf = 0.25;
	m = get_translation_matrix(centre_x + (nx * box_width * sf) , centre_y+ (ny * box_height * sf) ); // push towards the cursor
	// matmul(v, 16, m);
	matmul(vertz, 8, m);
	matmul(vertz2, 8, m);

	// set_scale(front_x,front_y,4,minimum_size*0.5,minimum_size*0.5);
	// translate(front_x,front_y,4,cursor_x,cursor_y);

	Start_batch_draw(box);
	Clear(box,240,240,240);
	// Real fps,t2;

	// nx = Absolute(nx);
	// ny = Absolute(ny);
	// nx = (nx > ny) ? nx : ny;
	// Print("nx = " + To_text(nx,5) + "\n");
	// Print("ny = " + To_text(ny,5) + "\n\n");
	draw_polygon(box, vertz , facez, cols, 6,nx);
	draw_polygon(box, vertz , facez, cols, 5,nx);
	draw_polygon(box, vertz , facez, cols, 4,nx);
	draw_polygon(box, vertz , facez, cols, 3,nx);
	draw_polygon(box, vertz , facez, cols, 2,nx);
	draw_polygon(box, vertz , facez, cols, 1,nx);

	draw_polygon(box, vertz2 , facez, cols, 6,nx);
	// draw_polygon(box, vertz2 , facez, cols, 5,nx);
	// draw_polygon(box, vertz2 , facez, cols, 4,nx);
	draw_polygon(box, vertz2 , facez, cols, 3,nx);
	draw_polygon(box, vertz2 , facez, cols, 2,nx);
	// draw_polygon(box, vertz2 , facez, cols, 1,nx);
	// Print("FPS = " + To_text(get_fps(),5) + "\n");
	// Draw_text(box, 5.0, 15.0,10.0,0.0,"FPS = " + To_text(get_fps(),5) );

	End_batch_draw(box);
	

}

void draw_about_panel_draw_box_logo2(Draw_Box &box){
	Integer box_width,box_height;
	Get_size(box,box_width,box_height);
	Integer cursor_x = box_width / 2;
	Integer cursor_y = box_height / 2;
	Real spin = 0.0;
	draw_about_panel_draw_box_logo2(box, cursor_x, cursor_y,spin,0);
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


Integer manage_about_panel(Text text_box_data){

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
	description_text+=text_box_data;
	Set_data(description_box,text_box_data);

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
	draw_box_text_setup(draw_box);
	draw_about_panel_draw_box_logo2(draw_box);

	Select_Box select_box = Create_select_box("Title","",SELECT_STRING,message_box);
	Integer cursor1 = 0,cursor2 = 0;
	Real spin=0.;
	// cursor1 = 0.0; cursor2 = 0.0; spin = 0.0;

	while(1){
		Integer id;
		Text cmd,msg;
		Wait_on_widgets(id,cmd,msg);
		// Print("Id=" + To_text(id) + " cmd=" + cmd + " msg=" + msg + "\n");
		if(cmd == "mouse_move"){
			Dynamic_Text parts;
			From_text(msg,parts);
			Text t;
			Get_item(parts,1,t);	From_text(t,cursor1);
			Get_item(parts,2,t);	From_text(t,cursor2);
			draw_about_panel_draw_box_logo2(draw_box,cursor1,cursor2,spin,0);
		}
		if(cmd == "mouse_leave" || cmd =="accept select" || cmd == "cancel select"){
			Select_start(select_box);
		}
		if(cmd == "motion select"){
			spin+=0.175;
			draw_about_panel_draw_box_logo2(draw_box,cursor1,cursor2,spin,1);
		}

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

Integer manage_about_panel(){
	return manage_about_panel(about_macro_description);
}


#endif