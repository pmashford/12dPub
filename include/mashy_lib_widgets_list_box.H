#ifndef mashy_lib_widgets_list_box_h_included
#define mashy_lib_widgets_list_box_h_included

#include "mashy_lib_system_calls.H"
#include "mashy_lib_shares.H"

Integer select_none(List_Box &box){
	Integer items;
	Get_number_of_items(box,items);
	if(items<1)return 1;
	Integer list[items];
	for(Integer i=1;i<=items;i++)list[i] = 0;
	Integer tf = 0;	// tf 0 means off???
	Set_selection_list(box,items,list,tf);
	return 0;
}

Integer select_all(List_Box &box){
	Integer items;
	Get_number_of_items(box,items);
	if(items<1)return 1;
	Integer list[items];
	for(Integer i=1;i<=items;i++)list[i] = 0;
	Integer tf = 1;	// tf 1 means on???
	Set_selection_list(box,items,list,tf);
	return 0;
}

Integer get_list_box_selection(List_Box &box, Dynamic_Text &dt){
	Integer count;
	Get_selection_count(box,count);
	if(count < 1)return 1;
	Integer list[count];
	Get_selection_list(box,count,list);
	for(Integer i=1;i<=count;i++){
		Text t;
		Get_item(box,list[i],t);
		Append(t,dt);
	}
	return(0);
}

Integer get_list_box_files(List_Box &box, Text path_no_end_slash, Dynamic_Text &dt){
	if(!get_list_box_selection(box,dt)){
		Integer items;
		Get_number_of_items(dt,items);
		for(Integer i=1;i<=items;i++){
			Text t;
			Get_item(dt,i,t);
			Set_item(dt,i,path_no_end_slash+"\\"+t);
		}

		return 0;
	}
	return 1; //error - no selection
}

Dynamic_Text get_list_box_files(List_Box &box,  Text path_no_end_slash){
	Dynamic_Text dt;
	get_list_box_files(box, path_no_end_slash, dt);
	return dt;
}

Integer copy_list_box_files(List_Box &box,Text path_no_end_slash,Text dest_path_no_end_slash){
	Dynamic_Text dt = get_list_box_files(box,path_no_end_slash);
	dest_path_no_end_slash+="\\";
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		if(copy_file(t,dest_path_no_end_slash,0)){
			Print("File exists in target project! <"+t+"> Copy Failed!");Print();
			ok++;
		}else{
			//Print("File copied<"+t+">");Print();
		}
	}

	return ok;
}

Integer share_list_box_models(List_Box &box,Integer mode){
	Dynamic_Text dt;
	get_list_box_selection(box,dt);
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		if(share_model(t,mode)){
			Print("<"+t+"> Share Failed!");Print();
			ok++;
		}else{
			//Print("File copied<"+t+">");Print();
		}
	}

	return ok;
}

Integer share_in_list_box_models(List_Box &box,Text path_no_end_slash,Text project_name){
	Dynamic_Text dt;
	get_list_box_selection(box,dt);
#if 1 // leave this off - this methof falls over if the model already exists in the project (ie none of the list is read in) - fixed function to check if model exists!
	Integer ok = share_in_models(dt, path_no_end_slash, project_name);
#else
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		if(share_in_model(t,path_no_end_slash,project_name)){
			Print("<"+t+"> Share Failed!");Print();
			ok++;
		}else{
			//Print("File copied<"+t+">");Print();
		}
	}
#endif
	return ok;
}

Integer create_model_share(Text model_name, Text dot_project_path_no_end_slash){
	Integer ok=0;
	Text t = find_replace(model_name," ","_") + ".model";
	if(File_exists(dot_project_path_no_end_slash+"\\"+t)){
		ok+=Directory_create(dot_project_path_no_end_slash+"\\"+t+".ref");
	}else{
		Print("<"+model_name+"> Share Failed!");Print();
		ok++;
	}
	return ok;
}

Integer share_out_list_box_models(List_Box &box,Text path_no_end_slash){
	Dynamic_Text dt;
	get_list_box_selection(box,dt);
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		ok+=create_model_share(t,path_no_end_slash);
	}

	return ok;
}

Integer create_tin_share(Text tin_name, Text dot_project_path_no_end_slash){
	Integer ok=0;
	Text t = find_replace(tin_name," ","_") + ".tin";
	if(File_exists(dot_project_path_no_end_slash+"\\"+t)){
		ok+=Directory_create(dot_project_path_no_end_slash+"\\"+t+".ref");
	}else{
		Print("<"+tin_name+"> Share Failed!");Print();
		ok++;
	}
	return ok;
}

Integer share_out_list_box_tins(List_Box &box,Text path_no_end_slash){
	Dynamic_Text dt;
	get_list_box_selection(box,dt);
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		ok+=create_tin_share(t,path_no_end_slash);
	}

	return ok;
}

Integer share_list_box_tins(List_Box &box,Integer mode){
	Dynamic_Text dt;
	get_list_box_selection(box,dt);


	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		if(share_tin(t,mode)){
			Print("<"+t+"> Share Failed!");Print();
			ok++;
		}else{
			//Print("File copied<"+t+">");Print();
		}
	}

	return ok;
}

Integer share_in_list_box_tins(List_Box &box,Text path_no_end_slash,Text project_name,Text prefix){
	Dynamic_Text dt;
	get_list_box_selection(box,dt);
#if 1 // leave this off - this methof falls over if the model already exists in the project (ie none of the list is read in) - fixed function to check if model exists!
	Integer ok = share_in_tins(dt, path_no_end_slash, project_name);
#else
	Integer items;
	Get_number_of_items(dt,items);
	Integer ok=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dt,i,t);
		if(share_in_tin(t,path_no_end_slash,project_name,prefix)){
			Print("<"+t+"> Share Failed!");Print();
			ok++;
		}else{
			//Print("File copied<"+t+">");Print();
		}
	}
#endif
	return ok;
}

Integer clear(List_Box &box){
	//Integer count;
	//Get_number_of_items(box,count);
	//for(Integer i=1;i<
	while(!Delete_item(box,1));
	return(0);
}

Integer set_items(List_Box &box, Dynamic_Text &dt){
	clear(box);
	Integer n;
	Get_number_of_items(dt,n);
	for(Integer i=1;i<=n;i++){
		Text t;
		Get_item(dt,i,t);
		Add_item(box,t);
	}
	return 0;
}






#endif
