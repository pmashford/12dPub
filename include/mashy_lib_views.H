#ifndef mashy_lib_views_H_included
#define mashy_lib_views_H_included

#include "mashy_lib_model_functions.H"
#include "mashy_lib_element_functions.H"
#include "mashy_lib_create_super.H"
#include "12d/slf.H"

#include "mashy_lib_xml_v10.H"


Integer draw_last_view(){
	Text t;
	Get_last_view(t);
	View v;
	return View_redraw(Get_view(t));
}

Integer view_colour(Text view_name, Integer colour){
	return run_panel_via_chain(slf_view_colour(view_name,colour));
}


Integer get_all_views(Text type, Dynamic_Text &list){

	Dynamic_Text all_views;
	Get_project_views(all_views);
	Integer items;

	Get_number_of_items(all_views,items);

	for(Integer i=1;i<=items;i++){
		Text view_name;
		Get_item(all_views,i,view_name);
		View view = Get_view(view_name);
		Text type_test;
		Get_type(view,type_test);
		if(type_test==type) Append(view_name,list);
	}
	return (0);
}

Dynamic_Text get_all_view_type_Perspective_OpenGL(){
	Dynamic_Text dt;
	get_all_views("Perspective OpenGL",dt);
	return(dt);
}

Dynamic_Text get_all_view_type_Plan(){
	Dynamic_Text dt;
	get_all_views("Plan",dt);
	return(dt);
}

Dynamic_Text get_all_view_type_Section(){
	Dynamic_Text dt;
	get_all_views("Section",dt);
	return(dt);
}


void model_to_back_all_views(Text name_to_back){
	Model model_to_back = Get_model(name_to_back);
	Dynamic_Text views;
	Model_get_views(model_to_back,views);
	Integer items;
	Get_number_of_items(views, items);
	for(Integer i = 1; i<= items ; i++){
		Text view;
		Get_item(views,i,view);
		Dynamic_Text models, models_in_order;
		View v = Get_view(view);
		View_get_models(v,models);
		Integer imodels;
		Get_number_of_items(models,imodels);
		for (Integer j = 1 ; j <= imodels ; j++){
			Text model_name;
			Get_item(models,j,model_name);
			if(model_name != name_to_back){		// remove other models from view and add to list to turn on later
				Append(model_name,models_in_order);
				View_remove_model(v,Get_model(model_name));
			}
		}
		Get_number_of_items(models_in_order,imodels);
		for (j =0 ; j <= imodels ; j++){
			Text name;
			Get_item(models_in_order,j,name);
			Model mdl = Get_model(name);
			View_add_model(v,mdl);
		}
		Null(models);
		Null(models_in_order);
	}
	Null(views);
}

void set_view_model_order(View v,Dynamic_Text &model_order){
	Integer items;
	Get_number_of_items(model_order,items);
	// remove all models except the first, so that we dont have to re-zoom
	for (Integer i = 2 ; i <= items ; i++ ){
		Text t;
		Get_item(model_order,i,t);
		View_remove_model(v,Get_model(t));
	}
	// add all the models back on it the order
	for (i = 2 ; i <= items ; i++ ){
		Text t;
		Get_item(model_order,i,t);
		View_add_model(v,Get_model(t));
	}

}


Integer view_get_models(Dynamic_Text &view_names, Dynamic_Text &model_names){
	Integer items;
	Get_number_of_items(view_names,items);
	for(Integer i = 1; i <= items ; i++ ){
		Text view_name;
		Get_item(view_names,i,view_name);
		View v = Get_view(view_name);
		Dynamic_Text dt;
		View_get_models(v,dt);
		Append(dt,model_names);
	}
	Integer count;
	remove_duplicates(model_names);
	Get_number_of_items(model_names,count);
	return(count);
}

Integer set_view_model_order(Dynamic_Text &view_names, Dynamic_Text &model_names_to_move, Text mode){
	//modes "-tins", "-rasters", "-back" , "-front"
	Integer iviews;
	Get_number_of_items(view_names,iviews);

	//build a list of models with rasters or tins (if mode selected)... process once
	Dynamic_Text  models_tin, models_raster;
	Null(models_tin);Null(models_raster);
	if(Find_text(mode,"-rasters") != 0 || Find_text(mode,"-tins") != 0){
		Dynamic_Text dt;
		view_get_models(view_names,dt);
		if(Find_text(mode,"-rasters") != 0){
			//check model for rasters
			models_raster = get_models_with_element_type(dt,"Raster Page");
		}
		if(Find_text(mode,"-tins") != 0){
			//check model for tins (-tin is a special mode to search for both "Tin" and "SuperTin"
			models_tin = get_models_with_element_type(dt,"-tins");
#if DEBUG
			Print("TINS\n");debug_dynamic_text(models_tin);
#endif
		}
		Null(dt);
	}

	for(Integer i = 1 ; i <= iviews ; i++ ){ //cycle through each view in dt
		Text view_name;
		Get_item(view_names,i,view_name);
		Dynamic_Text view_models;
		Null(view_models);
		View view = Get_view(view_name);
		View_get_models(view,view_models);

//		Integer imodel_names_to_move;
//		Get_number_of_items(model_names_to_move,imodel_names_to_move);
		Integer iview_models;
		Get_number_of_items(view_models,iview_models);
		Dynamic_Text dtm,dtt,dtr,dtv; //dtm (matched) dtt (tins) dtv (other models in view)
		Null(dtm);Null(dtt);Null(dtr);Null(dtv);
		Dynamic_Text models_in_order;Null(models_in_order);//models in the order that we want to set in that view
		for(Integer j = 1 ; j <= iview_models ; j++ ){ // cycle through a list of all the models in the view
			Text view_model;
			Get_item(view_models,j,view_model);
			Integer found = 0;
			if( find_item(view_model,model_names_to_move) != 0 )	{found++;	Append(view_model,dtm);}//there's a match!
			if( find_item(view_model,models_tin) != 0 ) 		{found++;	Append(view_model,dtt);}//there's a match!
			if( find_item(view_model,models_raster) != 0 ) 		{found++;	Append(view_model,dtr);}//there's a match!
			if( found == 0 )							Append(view_model,dtv); //no match so add to 'other models'
		}

		if(Find_text(mode,"-front") != 0){
			Append(dtv,models_in_order);
			Append(dtr,models_in_order);
			Append(dtt,models_in_order);
			Append(dtm,models_in_order);
		} else if (Find_text(mode,"-back") != 0){
			Append(dtr,models_in_order);
			Append(dtt,models_in_order);
			Append(dtm,models_in_order);
			Append(dtv,models_in_order);
		}
		//need to remove duplicates
#if DEBUG
		debug_dynamic_text(view_models);
		debug_dynamic_text(models_in_order);
#endif
		Null(dtm);Null(dtt);Null(dtr);Null(dtv);
		set_view_model_order(view,models_in_order);
		Null(models_in_order);
		Null(view_models);

	}
	Null(models_tin);Null(models_raster);
	return(0);
}

Integer set_view_model_order(Text view_name, Text model_name_to_move, Text mode){
	Dynamic_Text views, models;
	Append(view_name,views);Append(model_name_to_move,models);
	Integer retval = set_view_model_order(views,models,mode);
	return(retval);
}

//--------------------------------------------------------------------------------
//                        ADDING MODELS TO A VIEW - there is a call to add to the back of the view, this will reorder for each model added
//--------------------------------------------------------------------------------

Integer add_model_to_view_back(Model &model, Text view_name){
	View view = Get_view(view_name);
	Integer ok=View_add_model(view,model);
	if(ok == 0 ){ //send it to the back
		Text model_name;
		Get_name(model,model_name);
		set_view_model_order(view_name,model_name,"-back");
	}
	return(ok);
}

Integer add_model_to_view_back(Model &model, Dynamic_Text &list_of_views){
	Integer items;
	Get_number_of_items(list_of_views,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text view_name;
		Get_item(list_of_views,i,view_name);
		ok+=add_model_to_view_back(model,view_name);
	}
	return ok;
}

Integer add_model_to_view(Model &model, Text view_name){
	View view = Get_view(view_name);
	Integer ok=View_add_model(view,model);
	return(ok);
}

Integer add_model_to_view(Model &model, Dynamic_Text &list_of_views){
	Integer items;
	Get_number_of_items(list_of_views,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text view_name;
		Get_item(list_of_views,i,view_name);
		ok+=add_model_to_view(model,view_name);
	}
	return ok;
}

Integer view_add_models(View &view, Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		Model m = Get_model(t);
		ok+=View_add_model(view,m);
	}
	return ok;
}

Integer view_remove_models(View &view, Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		Model m = Get_model(t);
		ok+=View_remove_model(view,m);
	}
	return ok;
}

Integer view_match_name_add_to_view(View &view, Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	Dynamic_Text all_models,models;
	Get_project_models(all_models);

	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		Dynamic_Text list_to_add;
		match_name(all_models, t,list_to_add);
		Append(list_to_add,models);
	}
	return (view_add_models(view,models));
}

Integer view_match_name_add_to_view(View &view, Text t){
	Dynamic_Text dt;
	Append(t,dt);
	return(view_match_name_add_to_view(view, dt));
}

Integer view_match_name_remove_from_view(View &view, Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	Dynamic_Text all_models,models;
	if(View_get_models(view,all_models))return 1;

	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		Dynamic_Text list_to_add;
		match_name(all_models, t,list_to_add);
		Append(list_to_add,models);
	}
	return (view_remove_models(view,models));
}

Integer view_match_name_remove_from_view(View &view, Text t){
	Dynamic_Text dt;
	Append(t,dt);
	return(view_match_name_remove_from_view(view, dt));
}

Integer view_all_off(View &view){
	Integer ok=0;
	Dynamic_Text list;
	View_get_models(view,list);
	Integer items;
	Get_number_of_items(list,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(list,i,t);
		Model m = Get_model(t);
		ok+=View_remove_model(view,m);
	}
	return ok;
}

Integer view_add_model(View &view, Text model_name){
	Model m = Get_model(model_name);
	return View_add_model(view,m);
}


/*
Panel "View Dump" {

  Position 2139 161
  Field    "View"        "DATA INPUT"
  Field    "Include title"        "false"
  Field    "Format"        "JPEG"
  Field    "File"        "xxx.jpg"
  Run      "Dump"
}
*/
Integer dump_view_jpg(Text viewname, Text filename, Integer include_title){
#if 1
	XML_Node panel_node = slf_view_dump(viewname, include_title, "JPEG", filename);
	return run_panel_via_chain(panel_node);
#else
	Text data = "Position 99999 99999\n";
	//data = "";
	data+=field("View",viewname);
	Text t = "true";
	if(!include_title)t="false";
	data+=field("Include title",t);
	data+=field("Format","JPEG");
	data+=field("File",filename);

	Text    name        = "View Dump";
	Integer interactive = 0;
	if(File_exists(filename)){
		File_delete(filename);
	}
	return Panel_prompt(name,interactive,data);
#endif
}
//--------------------------------------------------------------------------------
//                        zooming a view to a given extent / location
//--------------------------------------------------------------------------------
Integer view_zoom(View &view, Real x1, Real y1, Real x2, Real y2){
	Integer ok=0;
	Element e = create_super(x1, y1, 0.0, x2, y2, 0.0);
	Dynamic_Text list_of_models;
	Model m = get_model_create_unique();
	View_get_models(view,list_of_models);
	ok+=view_remove_models(view,list_of_models);
	Set_colour(e,0);
	Set_model(e,m);
	View_add_model(view,m);
	ok+=View_fit(view);
	ok+=view_add_models(view,list_of_models);
	Model_delete(m);
	return ok;
}



Integer view_zoom(View &view, Element &e, Real buffer){
	Real min_x, max_x, min_y, max_y;
	get_extents(e, min_x, max_x, min_y, max_y);
	min_x-=buffer;
	min_y-=buffer;
	max_x+=buffer;
	max_y+=buffer;
	return view_zoom(view,min_x,min_y,max_x,max_y);
}

Integer view_zoom(View &view, Element &e){
	Real min_x, max_x, min_y, max_y;
	get_extents(e, min_x, max_x, min_y, max_y);
	Real w = max_x - min_x;
	Real h = max_y - min_y;
	Real buffer = 0.0;
	if(w > h){
		buffer = w;
	}else{
		buffer = h;
	}
	buffer/=10;
	min_x-=buffer;
	min_y-=buffer;
	max_x+=buffer;
	max_y+=buffer;
	return view_zoom(view,min_x,min_y,max_x,max_y);
}

Integer view_zoom(Element &e){
	Print("ZOOM\n");
	Model m;
	Get_model(e,m);
	Dynamic_Text list;
	Model_get_views(m,list);
	Integer items;
	Get_number_of_items(list,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(list,i,t);
		View view = Get_view(t);
		Print("zoom <"+t+"\n");
		view_zoom(view, e);
	}
	return 0;
}

Integer view_element(View &view, Element &e){
	Integer fit_view = 1;
	view_zoom(e);
	return Section_view_profile(view,e,fit_view);
}

Integer view_element(View &view, Uid &m, Uid &e){
	Element this_e;
	Get_element(m,e,this_e);
	view_zoom(this_e);
	return view_element(view,this_e);
}

//--------------------------------------------------------------------------------
//                        Deleting views
//--------------------------------------------------------------------------------

void view_delete(Text view_name){
	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);
	append_command(chain,create_link_delete_view(view_name));

	if(run_chain(chain)){	// run check for errors
		Print("Deleted view <"+view_name+"> FAILED!/n");
	}else{
		Print("Deleted view <"+view_name+"> OK/n");
	}
}

void view_delete_all(){
	Dynamic_Text views;
	Get_project_views(views);
	Integer items;
	Get_number_of_items(views,items);
	XML_Document  chain = create_chain();
	set_meta_data_metric(chain);
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(views,i,t);
		append_command(chain,create_link_delete_view(t));
	}

	if(run_chain(chain)){	// run check for errors
		Print("Delete all views FAILED!/n");
	}else{
		Print("Delete all views OK/n");
	}
}

#endif