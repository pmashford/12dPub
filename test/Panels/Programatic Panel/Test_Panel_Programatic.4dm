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


void test_custom_panel(){
    Text title = "Custom Title";
    Integer columns = 3;
    Integer s = 23;
    Text t[s];
    Integer button_x = 150, button_y = 30;  // NOT YET IMPLEMENTED
    t[1] = "Measure...\tPanel\tMeasure";    // format is -> "TEXT\tCMD\tARG1\tARG2"
    t[2] = "View...\tPanel\tView";          
    t[3] = "Create...\tPanel\tCreate";
    t[4] = "Edit...\tPanel\tEdit";

    t[5] = "Measure\\Measure 2...\tPanel\tMeasure\\Measure 2"; 
    t[6] = "Measure\\Measure Button 2\tMessage\t2";
    t[7] = "Measure\\Measure Button 3\tMessage\t3";
    t[8] = "Measure\\Measure Button 4\tMessage\t4";

    t[9] = "Measure\\Measure 2\\Measure 2 Button 1\tMessage\t1"; 
    t[10] = "Measure\\Measure 2\\Measure 2 Button 2\tMessage\t1"; 
    t[11] = "Measure\\Measure 2\\Measure 2 Button 3\tMessage\t1"; 

    t[12] = "View\\View Button 1\tMessage\t1";
    t[13] = "View\\View Button 2\tMessage\t2";
    t[14] = "View\\View Button 3\tMessage\t3";
    t[15] = "View\\View Button 4\tMessage\t4";
    t[16] = "View\\View Button 5\tMessage\t4";
    t[17] = "View\\View Button 6\tMessage\t4";
    t[18] = "View\\View Button 7\tMessage\t4";
    t[19] = "View\\View Button 8\tMessage\t4";

    t[20] = "Create\\Create Button 1\tMessage\t1";
    t[21] = "Create\\Create Button 2\tMessage\t2";
    t[22] = "Create\\Create Button 3\tMessage\t3";
    t[23] = "Create\\Create Button 4\tMessage\t4";

    Integer s2 = 3;
    Text t2[s2];
    t2[1] = "Finish\tPanel Quit";
    t2[2] = "Home\tPanel";
    t2[3] = "About\tPanel About";

    include_handle_custom_panel(title, t, s, t2, s2, columns, button_x, button_y);

}

void main(){
    test_custom_panel();
}