#ifndef mashy_lib_xml_h_included
#define mashy_lib_xml_h_included

#include "mashy_lib_files.H"

#ifndef MASHY_LIB_DEFAULT_XML_PRECISION
#define MASHY_LIB_DEFAULT_XML_PRECISION 3
#endif

// This will get or create a node by supplying a path
// path = node1\node2\this node\ect
Integer remove_leading(Text t,Integer c1, Text &out){
	Integer c2;
	Get_char(t,1,c2);
	if(c2 == c1){
		out = Get_subtext(t,2,Text_length(t));
		return 0;
	}
	out = t;
	return 1;
}


Text trim_leading(Text t, Integer c1){
	Text out="";
	while(!remove_leading(t,c1,out)){
		t = out;
	}
	return t;
}

Text trim_leading(Text t, Integer c1, Integer c2){
	while(1){
		Text out="";
		Integer ierr = 0;
		ierr+=remove_leading(t,c1,out);
		t=out;
		ierr+=remove_leading(t,c2,out);
		t=out;
		if(ierr==2)return t;
	}
	return t;
}

Integer  remove_trailing(Text t,Integer c1, Text &out){
	Integer c2;
	Integer length = Text_length(t);
	Get_char(t,length,c2);
	if(c2 == c1){
		if(length <= 1){
			out = "";
			return 0;
		}
		out = Get_subtext(t,1,Text_length(t)-1);
		return 0;
	}
	out = t;
	return 1;
}

Text trim_trailing(Text t, Integer c1){
	Text out="";
	while(!remove_trailing(t,c1,out)){
		t = out;
	}
	return t;
}

Text trim_trailing(Text t, Integer c1, Integer c2){
	while(1){
		Text out="";
		Integer ierr = 0;
		ierr+=remove_trailing(t,c1,out);
		t=out;
		ierr+=remove_trailing(t,c2,out);
		t=out;
		if(ierr==2)return out;
	}
	return t;
}

Text trim(Text t, Integer c1){
	Text out = t;
	out = trim_leading(t,c1);
	out = trim_trailing(out,c1);
	return out;
}

Text trim(Text t, Integer c1, Integer c2){
	Text out = t;
	out = trim_leading(t,c1,c2);
	out = trim_trailing(out,c1,c2);
	return out;
}

Integer count_child_nodes(XML_Node &parent, Text child_node_name){
// count how many child nodes match the name
	Integer count = 0;
	Integer items = Get_number_of_nodes(parent);
	for(Integer i = 1;i<=items;i++){
		XML_Node child_node;
		Get_child_node(parent,i,child_node);
		Text name;
		Get_node_name(child_node,name);
		if(name == child_node_name){
			count++;
		}
	}
	return count;
}

Integer get_create_node2(XML_Node &parent_node, Text path, XML_Node &node){
	// need to preprocess and remove leading and trailing / or \\
	path = trim(path,'\\','/');

	Dynamic_Text parts;
	get_file_path_parts(path, parts);
	Integer items;
	Get_number_of_items(parts,items);
	XML_Node this_node = parent_node;

	Integer create_remaining_nodes = 0;
	for(Integer i=1;i<=items;i++){
		Text child_node_name;
		Get_item(parts,i,child_node_name);
		XML_Node child_node;
		if(Get_child_node(this_node,child_node_name,child_node)){
			child_node = Create_node(child_node_name);
			Append_node(this_node,child_node);
		}
		this_node = child_node;
	}
	node = this_node;
	return 0;
}

Integer get_node(XML_Document &doc, Text path, XML_Node &node){
	//get the node, if it does not exist throw an error
	// need to preprocess and remove leading and trailing / or \\
	path = trim(path,'\\','/');

	Dynamic_Text parts;
	get_file_path_parts(path, parts);
	Integer items;
	Get_number_of_items(parts,items);
	XML_Node this_node;
	if(Get_root_node(doc,this_node))return 1; // failed to get the root node!
	//Integer create_remaining_nodes = 0;
	for(Integer i=1;i<=items;i++){
		Text child_node_name;
		Get_item(parts,i,child_node_name);
		XML_Node child_node;
		if(Get_child_node(this_node,child_node_name,child_node)){
			child_node = Create_node(child_node_name);
			Append_node(this_node,child_node);
		}
		this_node = child_node;
	}
	node = this_node;
	return 0;
}


Integer get_create_node(XML_Document &doc, Text path, XML_Node &node){
	// need to preprocess and remove leading and trailing / or \\
	path = trim(path,'\\','/');

	Dynamic_Text parts;
	get_file_path_parts(path, parts);
	Integer items;
	Get_number_of_items(parts,items);
	XML_Node this_node;
	if(Get_root_node(doc,this_node))return 1; // failed to get the root node!
	Integer create_remaining_nodes = 0;
	for(Integer i=1;i<=items;i++){
		Text child_node_name;
		Get_item(parts,i,child_node_name);
		XML_Node child_node;
		if(Get_child_node(this_node,child_node_name,child_node)){
			child_node = Create_node(child_node_name);
			Append_node(this_node,child_node);
		}
		this_node = child_node;
	}
	node = this_node;
	return 0;
}



XML_Node create_node(Text name, Text value){
	//XML_Node node = Create_node(name);
	//Set_node_text(node,value);
	XML_Node node = Create_text_node(name,value);
#if DEBUG
	Print(": creating node <"+name+">"+value+"<\\"+name+">\n");
#endif
	return node;
}

Integer delete_child_nodes(XML_Node &node){
	Integer no_nodes = Get_number_of_nodes(node);
	Integer ierr=0;
	for(Integer i=1;i<=no_nodes;i++){
		ierr+=Remove_node(node,1);
	}
	return(ierr);
}

Integer get_xml_node(XML_Node &parent_node, Text xml_node_name, XML_Node &matching_node){
	Integer nnodes = Get_number_of_nodes(parent_node);

	for(Integer i=1;i<=nnodes;i++){
		XML_Node child_node;
		if( Get_child_node(parent_node,i,child_node) ) return (1);

		Text child_node_name;
		if( Get_node_name(child_node,child_node_name) ) return (2);

		if(child_node_name == xml_node_name){
			//matching_node = child_node; // had to change to fix for version 10
			Get_child_node(parent_node,i,matching_node);
			return (0);
		}
	}
	return (3);
}
/*
Integer get_create_node(XML_Node &parent_node, Text xml_node_name, XML_Node &child_node){
	if(!get_xml_node(parent_node, xml_node_name, child_node)){
		return 0;
	}else{

	}
}
*/
Integer get_xml_node_data(XML_Node &parent_node, Text xml_node_name, Text &xml_node_data){
	XML_Node child_node;
	if(get_xml_node(parent_node,xml_node_name,child_node) != 0) return (1);
	if(Get_node_text(child_node,xml_node_data) != 0) return (2);
	return (0);
}

Integer set_xml_node(XML_Node &parent_node, Text xml_node_name, Text xml_node_data){
	XML_Node child_node;
	if(get_xml_node(parent_node,xml_node_name,child_node) == 0 ){
		Set_node_text(child_node,xml_node_data);
		return (0);
	}else{
		XML_Node new_child_node = Create_text_node(xml_node_name,xml_node_data);
		Append_node(parent_node,new_child_node);
		return (0);
	}
	return (1);
}

Integer get_data(XML_Node &parent_node, Text xml_node_name, Text &xml_node_data){
	XML_Node child_node;
	if(get_xml_node(parent_node,xml_node_name,child_node) != 0) return (1);
	if(Get_node_text(child_node,xml_node_data) != 0) return (2);
	return (0);
}

Integer set_data(XML_Node &parent_node, Text xml_node_name, Text xml_node_data){
	XML_Node child_node;
	if(get_xml_node(parent_node,xml_node_name,child_node) == 0 ){
		Set_node_text(child_node,xml_node_data);
		return (0);
	}else{
		XML_Node new_child_node = Create_text_node(xml_node_name,xml_node_data);
		Append_node(parent_node,new_child_node);
		return (0);
	}
	return (-1);
}

Integer set_data(XML_Node &parent_node, Text xml_node_name, Real xml_node_data, Integer precision){
	return set_data(parent_node, xml_node_name, To_text(xml_node_data,precision));
}

Integer set_data(XML_Node &parent_node, Text xml_node_name, Real xml_node_data){
	return set_data(parent_node, xml_node_name, To_text(xml_node_data,MASHY_LIB_DEFAULT_XML_PRECISION));
}

Integer set_data(XML_Node &parent_node, Text xml_node_name, Integer xml_node_data){
	return set_data(parent_node, xml_node_name, To_text(xml_node_data));
}

Integer get_data(XML_Node &parent_node, Text xml_node_name, Real &xml_node_data){
	Text t;
	Integer ierr = get_data(parent_node, xml_node_name, t);
	if(ierr){
		Null(xml_node_data);
		return ierr;
	}
	if(From_text(t,xml_node_data)){
		return 3;
	}
	return 0;
}

Integer get_data(XML_Node &parent_node, Text xml_node_name, Integer &xml_node_data){
	Text t;
	Integer ierr = get_data(parent_node, xml_node_name, t);
	if(ierr){
		xml_node_data = 0;
		return ierr;
	}
	if(From_text(t,xml_node_data)){
		return 3;
	}
	return 0;
}


Integer add_xml_node(XML_Node &parent_node, Text xml_node_name, Text xml_node_data){
	XML_Node new_child_node = Create_text_node(xml_node_name,xml_node_data);
	Append_node(parent_node,new_child_node);
	return (0);
}

Integer set_xml_node_att(XML_Node &parent_node, Text xml_node_name, Text att_name, Text att_value){
	XML_Node child_node;
	if(get_xml_node(parent_node,xml_node_name,child_node) == 0 ){
		Set_node_attribute(child_node, att_name, att_value);
		return (0);
	}else{
		XML_Node new_child_node = Create_node(xml_node_name);
		Append_node(parent_node,new_child_node);
		Set_node_attribute(child_node, att_name, att_value);
		return (0);
	}
	return (-1);
}

Integer get_xml_document(Text filename, XML_Document &doc, XML_Node &root_node){
	// dsfadsafkj;
	// usage notes:
	//	doc needs to exist as a valid XML_Document doc = Create_XML_document();
	// -4 return on Real_XML_document means something invalid in the XML file, for example the node name could have invalid characters such as spaces <name 2>

	Integer ierr;
	// Integer type;
	// ierr = Get_type(doc,type);	// NOTE: if ierr !=0 then can't trust type
	// Print("=>doc has type ("+To_text(type)+") with Get_type() ierr of ("+To_text(ierr)+")\n");

	if(!File_exists(filename)){
		Print("XML file <"+filename+"> does not exist on disk!\n");
	}
	ierr = Read_XML_document(doc, filename);
	if(ierr){
		doc = Create_XML_document();
		ierr = Read_XML_document(doc, filename);
	}
	if(ierr != 0 ){
		Print("XML file exists <"+filename+">, but failed to open with ierr <"+ To_text(ierr)+">\n");
		if(ierr == -4){
			Print("... check for valid syntax, ie. do node names have spaces?\n");
		}else{
			Print("... check that doc object has been created... XML_Document doc = Create_XML_document(); _stated before_ Read_XML_document(doc, filename); _is called_\n");
		}
		return ierr;
	}
	if(Get_root_node(doc,root_node) != 0 ) return (-2);
	return (0);
}

Integer write_xml_root_node(Text filename, XML_Node &root_node, Text version, Text encoding, Integer standalone){
	XML_Document doc = Create_XML_document();
	Set_XML_declaration(doc, version, encoding, standalone);
	Set_root_node(doc,root_node);
	return ( Write_XML_document(doc,filename) );
}


Integer write_xml_root_node(Text filename, XML_Node &root_node){
	XML_Document doc = Create_XML_document();
	Set_root_node(doc,root_node);
	return ( Write_XML_document(doc,filename) );
}


void debug_node(XML_Node &node, Integer depth){
	Text tabs="";
	for(Integer i=1;i<=depth;i++){
		tabs+=".";
	}
	Text name;
	Integer ierr;
	ierr = Get_node_name(node,name);
	Integer nnodes = Get_number_of_nodes(node);

	Print(tabs+"Node name <"+name+"> (Get_name() ierr<"+To_text(ierr)+">) has <"+To_text(nnodes)+"> child nodes\n");

	for(i=1;i<=nnodes;i++){
		XML_Node child;
		Get_child_node(node,i,child);
		debug_node(child,depth+1);
	}

}

void node_to_text(XML_Node &node, Integer depth, Text &data){
	Text tabs="";
	for(Integer i=1;i<=depth;i++){
		tabs+=".";
	}
	Text name="",t="";
	Integer ierr;
	ierr = Get_node_name(node,name);
	ierr+= Get_node_text(node,t);
	Integer nnodes = Get_number_of_nodes(node);

	//Print(tabs+"Node name <"+name+"> (Get_name() ierr<"+To_text(ierr)+">) has <"+To_text(nnodes)+"> child nodes\n");
	if(t!=""){
		data+="<"+name+">"+t+"</"+name+">";
	}else{
		data+="<"+name+">";
	}

	for(i=1;i<=nnodes;i++){
		XML_Node child;
		Get_child_node(node,i,child);
		node_to_text(child,depth+1,data);
	}
	if(t==""){
		data+="</"+name+">";
	}
}

Integer has_doc_at_top_of_tree(XML_Node &node){ // blows up where XML_Node is not contained in a XML_Document
	Print("\n\nhas_doc_at_top_of_tree()\n");
	XML_Node parent = node;
	Integer max_errors =0;
	Integer count_errors = 0;
	while(1){
		Integer ierr;
		ierr =  Get_parent_node(parent,parent); // ID = 2427
		Integer type;
		Get_type(parent,type);
		Text name;
		Get_node_name(parent,name);
		Print("ierr=" +To_text(ierr)+" name=" +name+ " type="+To_text(type)+"\n");
		if(ierr)count_errors++;
		if(count_errors >= max_errors ) return 0;
	}
	return 0;
}

// https://beta.12dmodel.com/forums/viewtopic.php?t=18614
// https://beta.12dmodel.com/forums/viewtopic.php?t=18615

void node_to_text_data(XML_Node &node, Text &data){		// TODO - handle attributes and declaration
	Text name="",t="";
	Get_node_name(node,name);
	if(Get_node_text(node,t))t="";	//
	data+="<"+name+">"+t;
	Integer n_nodes = Get_number_of_nodes(node);
	for(Integer i=1;i<=n_nodes;i++){	// 
		XML_Node child;
		Get_child_node(node,i,child);
		Integer type;	Get_type(child,type);
		if(i==1 && type == 4){ 
			Text name2;
			Get_node_name(child,name2);
			data +=name2;
		}else{
			node_to_text_data(child,data);
		}
	}
	data+="</"+name+">";
}

Integer prep_node_for_parsing_XML_tree(XML_Node &node){ //https://beta.12dmodel.com/forums/viewtopic.php?t=18614
	XML_Document doc = Create_XML_document();
	XML_Node root = Create_node("root");
	Set_root_node(doc,root);
	Integer ierr = Append_node_final(root,node); // if it errors then node is already in a doc, either way node should be a member of a doc and accessible
	return ierr ? 0 : 1; // 0 means was already in a doc, 1 means hacky doc was used (note: goes out of scope)
}

Text node_to_text_data(XML_Node &node){
	// XML_Document doc = Create_XML_document();
	// XML_Node root = Create_node("root");
	// Set_root_node(doc,root);
	// if(Append_node_final(root,node)){ // if it errors then node is already in a doc, either way node should be a member of a doc and accessible
	// }
	Integer ierr = prep_node_for_parsing_XML_tree(node);
	// Print("RAD ierr="+To_text(ierr)+"\n");
	Text data;
	node_to_text_data(node, data);
	return data;

}

void panel_node_to_text(XML_Node &node, Text &panel_name, Text &data){
	// Integer nnodes = Get_number_of_nodes(node);
	Get_node_text(node,panel_name);
	p(panel_name);
	// Integer depth = 0;
	// for(Integer i=1;i<=nnodes;i++){
	// 	XML_Node child;
	// 	Get_child_node(node,i,child);
	// 	node_to_text(child,depth+1,data);
	// }
	Print("DEBUG================\n");
	// Print(data);Print();	
	data = node_to_text_data(node);
	Print(data+"\n");
	Print("DEBUG================\n");
}


void debug_node(XML_Node &node){
	prep_node_for_parsing_XML_tree(node);
	debug_node(node,1);
}

Integer duplicate_node(XML_Node &node, XML_Node &new_node){
	Integer ok = 0;
	Text name;
	ok+= Get_node_name(node, name);
	if(Is_text_node(node)){
		Text t;
		ok+= Get_node_text(node, t);
		new_node= Create_text_node(name, t);
	}else{
		new_node = Create_node(name);
	}
	return ok;
}


#endif

/*
attribute example

<node att1="xx" att2="yy">
	<body>Example attributes</body>
</node>


*/


/*
// XML calls

XML_Document  Create_XML_document() L_XML_Document_Create;

Integer       Read_XML_document(XML_Document doc, Text file) L_XML_Document_Read;
Integer       Write_XML_document(XML_Document doc, Text file) L_XML_Document_Write;
Integer       Get_XML_declaration(XML_Document doc, Text &version, Text &encoding, Integer &standalone) L_XML_Document_Get_Declaration;
Integer       Set_XML_declaration(XML_Document doc, Text version, Text encoding, Integer standalone) L_XML_Document_Set_Declaration;

Integer       Get_root_node(XML_Document doc, XML_Node &node) L_XML_Document_Get_Root_Node;
Integer       Set_root_node(XML_Document doc, XML_Node &node) L_XML_Document_Set_Root_Node;

Integer       Get_number_of_nodes(XML_Node node) L_XML_Node_Get_Number_Of_Nodes;
Integer       Get_child_node(XML_Node node, Integer index, XML_Node &child_node) L_XML_Node_Get_Child_Node;
Integer       Get_child_node(XML_Node node, Text node_name, XML_Node &child_node) L_XML_Node_Get_Child_Node_By_Name;
Integer       Append_node(XML_Node parent, XML_Node new_node) L_XML_Node_Append_Node;
Integer       Remove_node(XML_Node parent, Integer index) L_XML_Node_Remove_Node;
Integer       Get_parent_node(XML_Node child, XML_Node &parent) L_XML_Node_Get_Parent_Node;
Integer       Get_next_sibling_node(XML_Node node, XML_Node &sibling) L_XML_Node_Get_Next_Sibling_Node;
Integer       Get_prev_sibling_node(XML_Node node, XML_Node &sibling) L_XML_Node_Get_Prev_Sibling_Node;
Integer       Get_node_name(XML_Node node, Text &name) L_XML_Node_Get_Node_Name;
Integer       Get_node_attribute(XML_Node node, Text name, Text &value) L_XML_Node_Get_Attribute_Value;
Integer       Set_node_attribute(XML_Node node, Text name, Text value) L_XML_Node_Set_Attribute;
Integer       Remove_node_attribute(XML_Node node, Text name) L_XML_Node_Remove_Attribute;
XML_Node      Create_node(Text name) L_XML_Node_Create_Node;

Integer       Is_text_node(XML_Node &node) L_XML_Node_Is_Text_Element;
Integer       Get_node_text(XML_Node &node, Text &text) L_XML_Node_Get_Text_Node_Text;
Integer       Set_node_text(XML_Node &node, Text text) L_XML_Node_Set_Text_Node_Text;
XML_Node      Create_text_node(Text name, Text value) L_XML_Node_Create_Text_Node;
*/

/*
Alan Gray example from the forums (file name: read_xml_map_file.4dm)
Integer read_map_data_item(XML_Node node)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  Integer no_nodes = Get_number_of_nodes(node);
  Integer res;

  for(Integer n=1;n<=no_nodes;n++) {

    XML_Node child_node;

    res = Get_child_node(node,n,child_node);
    if(res != 0) return(-1);

    Text child_node_name;

    res = Get_node_name(child_node,child_node_name);
    if(res != 0) return(-2);

    Text child_node_text;

    res = Get_node_text(child_node,child_node_text);

    Print(child_node_name);  Print("=");  Print(child_node_text);  Print();
  }
  return(0);
}
Integer read_map_data(XML_Node node)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  Integer no_nodes = Get_number_of_nodes(node);
  Integer res;

  for(Integer n=1;n<=no_nodes;n++) {

    XML_Node child_node;

    res = Get_child_node(node,n,child_node);
    if(res != 0) return(-1);

    Text child_node_name;

    res = Get_node_name(child_node,child_node_name);
    if(res != 0) return(-5);

    if(child_node_name == "item") {

      res = read_map_data_item(child_node);

    } else {

      Print(child_node_name);  Print(" skipped");  Print();
    }
  }
  return(0);
}
Integer read_mapfile(Text filename)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  XML_Document doc = Create_XML_document();

  Integer res;

  res = Read_XML_document(doc,filename);
  if(res != 0) return(-1);

  XML_Node node;

  res = Get_root_node(doc,node);
  if(res != 0) return(-2);

  Integer no_nodes = Get_number_of_nodes(node);

  for(Integer n=1;n<=no_nodes;n++) {

    XML_Node child_node;

    res = Get_child_node(node,n,child_node);
    if(res != 0) return(-4);

    Text child_node_name;

    res = Get_node_name(child_node,child_node_name);
    if(res != 0) return(-5);

    if(child_node_name == "map_data") {

      res = read_map_data(child_node);

    } else if(child_node_name == "vertex_symbol_data") {

    }
  }
  return(0);
}
void main()
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  read_mapfile("12d NZ V9 TOPO.mapfile");
}

*/