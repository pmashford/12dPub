#include "mashy_lib_widgets_panel_handler_std.H"


Text panel_command_parser(Text command, Integer flags){
// flags might be a way of testing that a command is executable without actually executing
    Text return_text = "";
    Dynamic_Text parts;
    From_text(command,'\"','\t',parts);
    Text this_command;
    Get_item(parts,1,this_command);
    switch (Text_lower(this_command)){
        case ("message") :{
            Print("NO ERRORS: message found... but what are the argumentsOK\n"); // DEBUG - TODO - REMOVE
        }break;

        case ("my command example") :{

        }break;


        default :{
            Print("\nERROR: ("+this_command+") not found! - full text passed in <"+command+">\n");
        }
    }
    return return_text;
}

Text make_command(Text button_name, Text command_with_tab_seperator){
    return button_name +"\t" + command_with_tab_seperator;
}

void test_custom_panel(){
    Text title = "Custom Title";
    Integer n_columns = 3;
    Integer n_buttons_top = 23;
    Text buttons_top[n_buttons_top];
    Integer button_x = 150, button_y = 30;  // NOT YET IMPLEMENTED
    buttons_top[1] = make_command("Measure...",     "Panel\tMeasure");    // format is -> "TEXT\tCMD\tARG1\tARG2"
    buttons_top[2] = make_command("View...",        "Panel\tView");          
    buttons_top[3] = make_command("Create...",      "Panel\tCreate");
    buttons_top[4] = make_command("Edit...",        "Panel\tEdit");

    buttons_top[5] = make_command("Measure\\Measure 2...",      "Panel\tMeasure\\Measure 2");  // if button is on a panel need to supply it's path
    buttons_top[6] = make_command("Measure\\Measure Button 2",  "Message\t2");
    buttons_top[7] = make_command("Measure\\Measure Button 3",  "Message\t3");
    buttons_top[8] = make_command("Measure\\Measure Button 4",  "Message\t4");

    buttons_top[9] = make_command("Measure\\Measure 2\\Measure 2 Button 1",     "Message\t1"); 
    buttons_top[10] = make_command("Measure\\Measure 2\\Measure 2 Button 2",    "Message\t1"); 
    buttons_top[11] = make_command("Measure\\Measure 2\\Measure 2 Button 3",    "Message\t1"); 

    buttons_top[12] = make_command("View\\View Button 1",   "Message\t1");
    buttons_top[13] = make_command("View\\View Button 2",   "Message\t2");
    buttons_top[14] = make_command("View\\View Button 3",   "Message\t3");
    buttons_top[15] = make_command("View\\View Button 4",   "Message\t4");
    buttons_top[16] = make_command("View\\View Button 5",   "Message\t4");
    buttons_top[17] = make_command("View\\View Button 6",   "Message\t4");
    buttons_top[18] = make_command("View\\View Button 7",   "Message\t4");
    buttons_top[19] = make_command("View\\View Button 8",   "Message\t4");

    buttons_top[20] = make_command("Create\\Create Button 1",   "Message\t1");
    buttons_top[21] = make_command("Create\\Create Button 2",   "Message\t2");
    buttons_top[22] = make_command("Create\\Create Button 3",   "Message\t3");
    buttons_top[23] = make_command("Create\\Create Button 4",   "Message\t4");

    Integer n_buttons_bot = 3;
    Text buttons_bot[n_buttons_bot];
    buttons_bot[1] = make_command("Finish", "Panel Quit");
    buttons_bot[2] = make_command("Home",   "Panel");              // get to the home panel by simply having a panel command with no args
    buttons_bot[3] = make_command("About",  "Panel About");

    handle_custom_panel(title, buttons_top, n_buttons_top, buttons_bot, n_buttons_bot, n_columns, button_x, button_y);

}

void main(){
    test_custom_panel();



}