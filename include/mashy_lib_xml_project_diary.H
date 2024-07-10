#ifndef mashy_lib_xml_project_diary_h_included
#define mashy_lib_xml_project_diary_h_included

#include "mashy_lib_xml.H"

#include "mashy_lib_system_calls.H"

Integer add_project_diary_day_entry(XML_Node parent_node, Text entry, Text timestamp, Text user ){
	XML_Node diary_node = Create_node("DiaryDay");
	if(add_xml_node(diary_node,"Timestamp",timestamp) != 0) return (-1);
	XML_Node entry_node = Create_node("DiaryEntry");
	Append_node(diary_node,entry_node);
	if(add_xml_node(entry_node,"User",user) != 0) return (-2);
	if(add_xml_node(entry_node,"Text",entry) != 0) return (-3);
	if(add_xml_node(entry_node,"Timestamp",timestamp) != 0) return (-4);
	return ( Append_node(parent_node,diary_node) );
}

// Set Date format if required, example %Y-%m-%d = 2009-01-01, 4 digit year, 2 digit month and day
//%a   abbreviated weekday name
//%A   full weekday name
//%b   abbreviated month name
//%B   full month name
//%c   date and time representation for locale
//%d   day of month as decimal number (01 - 31)
//%H   hour in 24-hour format (00 - 23)
//%I   hour in 12-hour format (01 - 12)
//%j   day of year as decimal number (001 - 366)
//%m   month as decimal number (01 - 12)
//%M   minute as decimal number (00 - 59)
//%p   current locale's A.M./P.M. indicator for 12-hour clock
//%S   second as decimal number (00 - 59)
//%U   week of year as decimal number, with Sunday as first day of week (00 - 51)
//%w   weekday as decimal number (0 - 6; Sunday is 0)
//%W   week of year as decimal number, with Monday as first day of week (00 - 51)
//%x   date representation for current locale
//%X   time representation for current locale
//%y   year without century, as decimal number (00 - 99)
//%Y   year with century, as decimal number
// Date format is case sensitive

Text get_timestamp(){

	Integer time;	Time(time);
	time -= (10*60*60);
	Text format = "%d-%m-%Y %H:%M:%S";
	Text time_text;	Convert_time(time,format,time_text);
	return ( time_text );
}

Integer add_project_diary_day_entry(XML_Node parent_node, Text entry){
	Text timestamp = get_timestamp();
	Text user; Get_user_name(user);
	return ( add_project_diary_day_entry(parent_node, entry, timestamp, user ) );
}

Text get_project_diary_xml_filename(){
	return (get_dot_project_folder() + "\\" + "project_diary.xml");
}

Integer get_project_diary_xml_doc(XML_Document &doc){
	if(Read_XML_document(doc, get_project_diary_xml_filename()) != 0 ) return (-1);
	return (0);
}

Integer add_project_diary_day_entry(Text entry){
	XML_Document doc = Create_XML_document();
	if(get_project_diary_xml_doc(doc) != 0) return (-1);
	XML_Node root_node;
	Get_root_node(doc,root_node);
	if(add_project_diary_day_entry(root_node,entry) != 0) return (-2);
	return ( Write_XML_document(doc,get_project_diary_xml_filename()) );
}

Text project_diary_header(){
	return ( "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang3081{\\fonttbl{\\f0\\fnil\\fcharset0 Tahoma;}}");
}

Text project_diary_footer(){
	return ( " }");
}

Text project_diary_normal_font(Text t){
	return (" \\viewkind4\\uc1\\pard\\f0\\fs17 " + t + "\\par " );
}

Text project_diary_bold_font(Text t){
	return (" \\viewkind4\\uc1\\pard\\b\\f0\\fs17 " + t + "\\b0\\par " );
}


#endif