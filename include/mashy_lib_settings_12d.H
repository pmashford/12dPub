#ifndef mashy_lib_settings_12d_H_included
#define mashy_lib_settings_12d_H_included

#if 0
    Integer Remove_project_setting(Text name); // ID = 3545
    Integer Project_setting_exists(Text name); // ID = 3546
    Integer Get_project_setting_integer(Text name); // ID = 3547
    Real Get_project_setting_real(Text name); // ID = 3548
    Text Get_project_setting_text(Text name); // ID = 3549
    Integer Get_project_setting_colour(Text name); // ID = 3550
    Attributes Get_project_setting_attributes(Text name); // ID = 3551
    Integer Set_project_setting_integer(Text name,Integer value); // ID = 3552
    Integer Set_project_setting_real(Text name,Real value); // ID = 3553
    Integer Set_project_setting_text(Text name,Text value); // ID = 3554
    Integer Set_project_setting_colour(Text name,Integer value); // ID = 3555
    Integer Set_project_setting_attributes(Text name,Attributes value); // ID = 3556


    Print("ok");Print();
    Print("");Print(Get_project_setting_integer(t+"/show_function_results"));Print();
    Print("testing");Print();

#endif

void test_settings_12d_values(){
    Text prefix = "default";
    Integer show_errors = 1, show_oks = 1;
    Dynamic_Text list;
    // Settings
    //  Defaults
    Append("show_function_results",list);
    // Plot and reports
    Append("null_height",list);
    Append("display_report",list);
    Append("print_reports",list);
    Append("send_plots",list);
    Append("xs",list);

    Integer items;
    Get_number_of_items(list,items);
    Text msg;
    for(Integer i=1;i<=items;i++){
        Text t;
        Get_item(list,i,t);
        if(Project_setting_exists(prefix+"/"+t)){
            if(show_oks) msg+="<"+t+"> Setting exists\n";
        }else{
            if(show_errors) msg+="! <"+t+"> Setting doesn't exist\n";
        }
    }
    Print(msg);
}
#if 0
void main(){	Clear_console();	return test_settings_12d_values();  }
#endif




Integer settings_function_results_off(){ // returns old settng so that it can be restored
    Integer old_value = Get_project_setting_integer("default/show_function_results");
    if(old_value == 0) return 0; // already off
    Set_project_setting_integer("default/show_function_results", 0);
    return 1;
} 

void settings_function_results_restore(Integer old_value){
    Set_project_setting_integer("default/show_function_results", old_value);
}


Integer settings_set(Text key, Integer new_value, Integer &old_value){
	if(!Project_setting_exists("default/"+key))return 999;
    old_value = Get_project_setting_integer("default/"+key);
	return Set_project_setting_integer("default/"+key, new_value);
}




Integer settings_set(Text key, Integer new_value){
	return Set_project_setting_integer("default/"+key, new_value);
}

Integer settings_set(Text key, Real new_value, Real &old_value){
	if(!Project_setting_exists("default/"+key))return 999;
    old_value = Get_project_setting_real("default/"+key);
	return Set_project_setting_real("default/"+key, new_value);
}

Integer settings_set(Text key, Real new_value){
	return Set_project_setting_real("default/"+key, new_value);
}

Integer settings_set(Text key, Text new_value, Text &old_value){
	if(!Project_setting_exists("default/"+key))return 999;
    old_value = Get_project_setting_text("default/"+key);
	return Set_project_setting_text("default/"+key, new_value);
}

Integer settings_set(Text key, Text new_value){
	return Set_project_setting_text("default/"+key, new_value);
}

Integer settings_set_colour(Text key, Integer new_value, Integer &old_value){
	if(!Project_setting_exists("default/"+key))return 999;
    old_value = Get_project_setting_colour("default/"+key);
	return Set_project_setting_colour("default/"+key, new_value);
}

Integer settings_set_colour(Text key, Integer new_value){
	return Set_project_setting_colour("default/"+key, new_value);
}

#endif