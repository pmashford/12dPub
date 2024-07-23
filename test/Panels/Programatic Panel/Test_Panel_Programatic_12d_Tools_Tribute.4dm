#define PANEL_EXPOSE_AUTO_HIDE 1
#include "mashy_lib_widgets_panel_handler_std.H"

Integer display_12d_panel_leave_open(Text panel_name){
	Text path;  	Get_temporary_project_directory(path);      Text file_name = path+"/delete_me.slx";     // build a temp filename
    File f;
    File_open(file_name,"w",f); //overwrite if it exists, gunna leave it there anyway
    File_write_line(f,"<xml12d><screen_layout><version>1.0</version><panel><name>"+panel_name+"</name></panel></screen_layout></xml12d>");  // write out the panel we want to display
    File_close(f);
    Integer interactive = 0xff00;   // execute the "Read Screen Layout"
    return Panel_prompt("Read Screen Layout",interactive,"<x>9000</x><y>9000</y><file_box><name>Layout file</name><value>"+file_name+"</value></file_box>");    
}

Text panel_command_parser(Text command, Integer flags){
// flags might be a way of testing that a command is executable without actually executing
    Text return_text = "";
    Text this_command, this_command_balance_right_after_tab;
    include_split_by_tab(command, this_command, this_command_balance_right_after_tab);

    switch (Text_lower(this_command)){
        case ("message") :{
            Print("NO ERRORS: cmd \"message\" found with the following argument<"+this_command_balance_right_after_tab+">\n"); // DEBUG - TODO - REMOVE
        }break;

        case ("12d") :{
            display_12d_panel_leave_open(this_command_balance_right_after_tab);
        }break;

        default :{
            Print("\nERROR: ("+this_command+") not found! - full text passed in <"+command+">\n");
        }
    }
    return return_text;
}

void add_cmd(Dynamic_Text &list, Text button_name, Text command_with_tab_seperator){    // TODO - at the moment \t is hardcoded as the seperator, make it customisable
    Append(button_name +"\t" + command_with_tab_seperator,list);
}

void test_custom_panel(){
    Text title = "12d Tools Tribute";
    Integer n_columns = 3;
    Dynamic_Text buttons_top;
    Integer button_width = 12, button_height = 60;  // button_width of 0 means panel will work it out |  button_height about 60 seems OK for phat buttons
    add_cmd(buttons_top,"Inquire/Measure...",       "Panel\tInquire/Measure");    // format is -> "TEXT\tCMD\tARG1\tARG2"
    add_cmd(buttons_top,"Create/Edit Points...",    "Panel\tCreate/Edit Points");
    add_cmd(buttons_top,"Menu Panels...",           "Panel\tMenu Panels");
    add_cmd(buttons_top,"View...",                  "Panel\tView");          
    add_cmd(buttons_top,"Create...",                "Panel\tCreate");
    add_cmd(buttons_top,"Edit...",                  "Panel\tEdit");

    add_cmd(buttons_top,"Inquire/Measure\\String Inquire",          "12d\tString Inquire");
    add_cmd(buttons_top,"Inquire/Measure\\Measure Brg/Dst",         "12d\tMeasure Bearing/Distance");
    add_cmd(buttons_top,"Inquire/Measure\\XFall 2 Strs",            "12d\tXFall Between 2 Strings Inquire");
    add_cmd(buttons_top,"Inquire/Measure\\Measure Str From Pt",     "12d\tMeasure String From a Point");
    add_cmd(buttons_top,"Inquire/Measure\\String Attributes",       "12d\tString Attributes");
    add_cmd(buttons_top,"Inquire/Measure\\Measure Value",           "12d\tMeasure Value");
    add_cmd(buttons_top,"Inquire/Measure\\Tin Depth From String",   "12d\tDepth From String to Tin Inquire");
    add_cmd(buttons_top,"Inquire/Measure\\Tin Height",              "12d\tTin Height Inquire");
    add_cmd(buttons_top,"Inquire/Measure\\Tin Slope",               "12d\tTin Slope Inquire");

    add_cmd(buttons_top,"Create/Edit Points\\Edit Vertex",               "12d\tEdit Vertex");
    add_cmd(buttons_top,"Create/Edit Points\\Delete Point",              "12d\tDelete Point");
    add_cmd(buttons_top,"Create/Edit Points\\Point Height",              "12d\tPoint Height");
    add_cmd(buttons_top,"Create/Edit Points\\Change String",             "12d\tChange String");

    add_cmd(buttons_top,"Menu Panels\\Cad Point",               "12d\tCad Point");
    add_cmd(buttons_top,"Menu Panels\\Cad Line",                "12d\tCad Line");
    add_cmd(buttons_top,"Menu Panels\\Cad Polygon",             "12d\tCad Polygon");
    add_cmd(buttons_top,"Menu Panels\\Points Edit",             "12d\tPoints Edit");
    add_cmd(buttons_top,"Menu Panels\\Strings Edit",            "12d\tStrings Edit");
    add_cmd(buttons_top,"Menu Panels\\Text Commands",           "12d\tCad Text Commands");
    

    add_cmd(buttons_top,"Inquire/Measure\\Measure 2...",                    "Panel\tInquire/Measure\\Measure 2");  // if button is on a panel need to supply it's path
    add_cmd(buttons_top,"Inquire/Measure\\Measure 2\\Measure 2 Button 1",   "Message\t1"); 
    add_cmd(buttons_top,"Inquire/Measure\\Measure 2\\Measure 2 Button 2",   "Message\t1"); 
    add_cmd(buttons_top,"Inquire/Measure\\Measure 2\\Measure 2 Button 3",   "Message\t1"); 
    

    add_cmd(buttons_top,"View\\View Button 1",   "Message\t1");
    add_cmd(buttons_top,"View\\View Button 2",   "Message\t2");
    add_cmd(buttons_top,"View\\View Button 3",   "Message\t3");
    add_cmd(buttons_top,"View\\View Button 4",   "Message\t4");
    add_cmd(buttons_top,"View\\View Button 5",   "Message\t4");
    add_cmd(buttons_top,"View\\View Button 6",   "Message\t4");
    add_cmd(buttons_top,"View\\View Button 7",   "Message\t4");
    add_cmd(buttons_top,"View\\View Button 8",   "Message\t4");

    add_cmd(buttons_top,"Create\\Create Button 1",   "Message\t1");
    add_cmd(buttons_top,"Create\\Create Button 2",   "Message\t2");
    add_cmd(buttons_top,"Create\\Create Button 3",   "Message\t3");
    add_cmd(buttons_top,"Create\\Create Button 4",   "Message\t4");

    Dynamic_Text buttons_bot;
    add_cmd(buttons_bot,"Finish", "Panel Quit");
    add_cmd(buttons_bot,"Home",   "Panel");              // get to the home panel by simply having a panel command with no args
    // add_cmd(buttons_bot,"About",  "Panel About");
    add_cmd(buttons_bot,"Hide",  "Panel Hide");
    Integer autohide_tick_state = 0; // -1 / 0 / 1 = disable, initial state off, initial state on

    handle_custom_panel(title, buttons_top, buttons_bot, n_columns, autohide_tick_state, button_width, button_height);
}


void main(){
    test_custom_panel();
}