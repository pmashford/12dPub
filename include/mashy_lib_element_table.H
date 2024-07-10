#ifndef mashy_lib_element_table_H_included
#define mashy_lib_element_table_H_included

#if 0

// here's where I doco'd some info 

http://forums.12dmodel.com/viewtopic.php?t=4387&start=30

// 12d table data

/*
//STRING ATTRIBUTES
_TABLE	text	_TABLE
_TITLE	integer	0
_NROWS	integer	2
_NCOLS	integer	2
_HEIGHT	real	16
*/

//text settings are in the first rows verticies




/*
row1 cells have the following vertex attributes
_WIDTH	real	20
_OFFSET	real	0
_RAISE	real	0
_VISIBLE	integer	0
*/		


//heading is the last vertex when string att _TITLE integer 1

//
#endif

Integer get_table_element_verticies(Integer ncols, Integer nrows,
					Integer &first_text_vertex, Integer &last_text_vertex,
					Integer &first_hidden_segment, Integer &last_hidden_segment){
	first_text_vertex = (ncols*2)+(nrows*2) + 5;
	last_text_vertex = first_text_vertex + (ncols * nrows)-1;
	
	first_hidden_segment = first_text_vertex-1;
	last_hidden_segment = last_text_vertex;
	
	return 0;
}

Integer get_table_attributes(Element &e, Integer &do_title, Integer &rows, Integer &cols, Real &row_height){
	Integer ok = 0;
	Text text;
	ok+=Get_attribute(e,"_TABLE",text);
	if(text != "_TABLE"){
		ok++;
	}
	ok+=Get_attribute(e,"_TITLE",do_title);
	ok+=Get_attribute(e,"_NROWS",rows);
	ok+=Get_attribute(e,"_NCOLS",cols);
	ok+=Get_attribute(e,"_HEIGHT",row_height);
	
	return ok;
}

Integer set_table_attributes(Element &e, Integer do_title, Integer rows, Integer cols, Real row_height){
	Integer ok = 0;
	Text text = "_TABLE";
	ok+=Set_attribute(e,"_TABLE",text);

	ok+=Set_attribute(e,"_TITLE",do_title);
	ok+=Set_attribute(e,"_NROWS",rows);
	ok+=Set_attribute(e,"_NCOLS",cols);
	ok+=Set_attribute(e,"_HEIGHT",row_height);
	
	return ok;
}


Integer text_to_table_element(Text title, Text text, Integer cell_break, Integer row_break, Integer cols, Integer rows, Element &e){
	
	if(rows < 1 || cols < 1)return 1;
	//work out the number of verticies required
	Integer do_title =0;
	if(title!="")do_title=1;
	
	Integer first_text_vertex, last_text_vertex, first_hidden_segment, last_hidden_segment;
	get_table_element_verticies(cols, rows,first_text_vertex, last_text_vertex,first_hidden_segment, last_hidden_segment);

	Integer title_text_vertex = last_text_vertex + 5;
	Integer n_vertex_count = last_text_vertex;
	if(do_title) n_vertex_count = title_text_vertex;

	//create the element
	e = Create_super(1,n_vertex_count);
	Set_super_use_vertex_text_array(e,1);
	Set_super_use_vertex_attribute(e,1);
	//Set_super_use_vertex_annotation_value(e,1);
	Set_super_use_vertex_annotation_array(e,1);
	Set_super_use_visibility(e,1);
	
	Set_super_use_vertex_visibility_value(e,1);
	Set_super_vertex_visibility(e,1,2);

	//pump the text into the string
	
	Integer cell_count=first_text_vertex-1;
	Integer row_count = 1;	//need to start at 1
	Integer col_count = 0;
	
	Integer p1 = 1;
	Integer size = Text_length(text);
	
	Integer max_cols=0;
	
	for(Integer i = 1;i<=size;i++){
		Integer c1; Get_char(text,i,c1);
		Text t="";
		Integer new_row = 0;
		if(c1 == cell_break){
			if(i>p1)t = Get_subtext(text,p1,i-1);
			col_count++;	//increment column
		}else if(c1 == row_break){
			if(i>p1)t = Get_subtext(text,p1,i-1);
			col_count++;
			new_row = 1;
		}else{
			if(i!=size)continue;
			if(i>=p1)t = Get_subtext(text,p1,i);
		}
		cell_count++;
		if(cell_count > last_text_vertex){
			i=size+1;
		}else{
			if(col_count <= cols){
				Set_super_vertex_text(e,cell_count,t);
				Set_super_vertex_attribute(e,cell_count,"row",row_count);
				Set_super_vertex_attribute(e,cell_count,"col",col_count);
				//Print("c"+To_text(col_count) + " , r" + To_text(row_count)+" ["+t+"]\n");
			}else{
				cell_count--;
			}
			if(new_row){
				if(col_count > max_cols)col_count = max_cols;
				col_count=0;
				row_count++;
				cell_count = first_text_vertex-1 + (row_count-1) * cols;
			}
			p1 = i+1;
		}
	
	}
	if(do_title){
		Set_super_vertex_text(e,n_vertex_count,title);
	}
		

	//STRING ATTRIBUTES
	Real row_height = (5.0);
	Set_attribute(e,"_TABLE","_TABLE");
	Set_attribute(e,"_TITLE",do_title);
	Set_attribute(e,"_NROWS",rows);
	Set_attribute(e,"_NCOLS",cols);
	Set_attribute(e,"_HEIGHT",row_height);

	Calc_extent(e);
	
	return 0;
}

Integer format_table_element(Element &e, Textstyle_Data title_style, Textstyle_Data heading_style[], Textstyle_Data cell_style[], Real column_widths[], Integer column_widths_count, Real title_height, Real row_height, Real h_pad){
	Integer rows,cols;
	Integer do_heading;
	Integer ok=0;
	
	ok+=Get_attribute(e,"_TITLE",do_heading);
	ok+=Get_attribute(e,"_NROWS",rows);
	ok+=Get_attribute(e,"_NCOLS",cols);

	if(rows < 1 || cols < 1)return 2;
	if(column_widths_count < cols)return 3;	
	ok+=Set_attribute(e,"_HEIGHT",row_height);	//N

	Integer first_text_vertex, last_text_vertex, first_hidden_segment, last_hidden_segment;
	get_table_element_verticies(cols, rows,first_text_vertex, last_text_vertex,first_hidden_segment, last_hidden_segment);
	
	Integer heading_text_vertex = last_text_vertex + 5;
	Integer n_vertex_count = last_text_vertex;
	if(do_heading) n_vertex_count+=5;
	
	Real x[n_vertex_count],y[n_vertex_count],z[n_vertex_count],r[n_vertex_count];
	Integer f[n_vertex_count];

	Get_super_data(e,x,y,z,r,f,n_vertex_count,n_vertex_count);
	Real col_x[cols+1];
	Real row_y[rows];
	Integer i;

	for(i=1;i<=cols+1;i++){
		if(i==1)col_x[i] =0;
		else{
			col_x[i] = col_x[i-1] + column_widths[i-1];
		}
		
	}
	Real table_width = col_x[cols]+column_widths[column_widths_count];
	
	for(i=1;i<=rows;i++){
		if(i==1)row_y[i] =0;
		else row_y[i] = row_y[i-1] - row_height;
	}
	Real table_height = rows * row_height;

	Real dy = 0.0;
	Integer count = (rows+1)*2;
	for(i=1;i<=count;i++){
		Set_super_segment_visibility(e,i,1);
		x[i] = 0.0;
		y[i] = dy;
		i++;
		x[i] = table_width;
		y[i] = dy;
		//set segment i invisible
		Set_super_segment_visibility(e,i,0);
		dy-=row_height;
	}
	
	Real dx;
	Integer this_row=1;
	for(i=count+1;i<first_text_vertex;i++){
		Set_super_segment_visibility(e,i,1);
		Integer pos = this_row;
		if(pos > cols+1)pos = cols+1;
		dx=col_x[1]+col_x[pos];
		x[i] = dx;
		y[i] = 0.0;
		i++;
		x[i] = dx;
		y[i] = (-1.0)*table_height;
		//set segment i invisible
		Set_super_segment_visibility(e,i,0);
		this_row++;
	}


	//position the text
	this_row = 1;
	Integer this_col = 1;
	
	for(i=first_text_vertex;i<=last_text_vertex;i++){
		Set_super_segment_visibility(e,i,0);
		Real x_offset = 0.0;
		Real width = column_widths[this_col];
		Real half_text_height=0.0;
		Real y_offset = row_height /2;
		Integer justify = 2;
		if(this_row == 1){
			//VERTEX ATTRIBUTES
			Set_super_vertex_attribute(e,i,"_WIDTH",column_widths[this_col]);
			Set_super_vertex_attribute(e,i,"_OFFSET",(0.0));
			Set_super_vertex_attribute(e,i,"_RAISE",(0.0));
			Set_super_vertex_attribute(e,i,"_VISIBLE",0);
			Set_super_vertex_textstyle_data(e,i,heading_style[this_col]);
			Get_justify(heading_style[this_col],justify);
			Get_size(heading_style[this_col],half_text_height);
		}else{
			Set_super_vertex_textstyle_data(e,i,cell_style[this_col]);
			Get_justify(cell_style[this_col],justify);
			Get_size(cell_style[this_col],half_text_height);
		}
		half_text_height/=2.0;
		switch (justify)  {
			case (1) : case (2) : case (3) : {
				x_offset = h_pad;
				if(justify==1)y_offset+=half_text_height;
				if(justify==3)y_offset-=half_text_height;
			}break;
			case (4) : case (5) : case (6) : {
				x_offset = width/2;
				if(justify==4)y_offset+=half_text_height;
				if(justify==6)y_offset-=half_text_height;
			}break;
			case (7) : case (8) : case (9) : {
				x_offset = width - h_pad;
				if(justify==7)y_offset+=half_text_height;
				if(justify==9)y_offset-=half_text_height;
			}break;
		}
		x[i] = col_x[this_col]+x_offset;
		y[i] = row_y[this_row]-y_offset;
		this_col++;
		if(this_col > cols){
			this_col=1;
			this_row++;
		}
		if(this_row > rows)i=last_text_vertex+1; //break
	}
	if(do_heading){
		i = last_text_vertex+1;
		Set_super_segment_visibility(e,i,1);
		x[i] = 0.0;y[i] = 0.0;
		i++;
		Set_super_segment_visibility(e,i,1);
		x[i] = 0.0;y[i] = title_height;
		i++;
		Set_super_segment_visibility(e,i,1);
		x[i] = table_width;y[i] = title_height;
		i++;
		Set_super_segment_visibility(e,i,0);
		x[i] = table_width;y[i] = 0.0;

		Integer justify;
		Get_justify(title_style,justify);
		Real half_text_height=0.0;
		Get_size(title_style,half_text_height);
		half_text_height/=2;
		Real y_offset =title_height/(-2.0);
		Real x_offset = 0.0;
		switch (justify)  {
			case (1) : case (2) : case (3) : {
				x_offset = h_pad;
				if(justify==1)y_offset+=half_text_height;
				if(justify==3)y_offset-=half_text_height;
			}break;
			case (4) : case (5) : case (6) : {
				x_offset = table_width/2;
				if(justify==4)y_offset+=half_text_height;
				if(justify==6)y_offset-=half_text_height;
			}break;
			case (7) : case (8) : case (9) : {
				x_offset = table_width - h_pad;
				if(justify==7)y_offset+=half_text_height;
				if(justify==9)y_offset-=half_text_height;
			}break;
		}
		x[n_vertex_count] = x_offset;
		y[n_vertex_count] = y_offset*(-1.0);
		Set_super_vertex_textstyle_data(e,n_vertex_count,title_style);
	}

	Set_super_data(e,x,y,z,r,f,n_vertex_count);
	Calc_extent(e);
	
	return 0;
}

Integer set_textstyle_data_table_element_default(Textstyle_Data &textdata){
	Integer ok=0;
	ok+=Set_textstyle    (textdata,"ISO")     ;
	ok+=Set_colour       (textdata,5)    ;
	ok+=Set_text_type    (textdata,1)      ;	//1==world
	ok+=Set_size         (textdata,3.5)      ;
	ok+=Set_offset       (textdata,0.0)    ;
	ok+=Set_raise        (textdata,0.0)     ;
	ok+=Set_justify      (textdata,5)   ;
	ok+=Set_angle        (textdata,0.0)     ;
	ok+=Set_slant        (textdata,0.0)     ;
	ok+=Set_x_factor     (textdata,1.0)   ;
	ok+=Set_name         (textdata,"")      ;
	ok+=Set_ttf_underline(textdata,0) ;
	ok+=Set_ttf_strikeout(textdata,0) ;
	ok+=Set_ttf_italic   (textdata,0)    ;
	ok+=Set_ttf_weight   (textdata,0)    ;
	return (ok);							  
}

Integer create_table_element(Text title, Text text, Integer cell_break, Integer row_break, Integer cols, Integer rows, Real column_width[], Element &e){
	if(cols < 1 || rows < 1)return 1;
	text_to_table_element(title,text,cell_break, row_break, cols,rows,e);
	Set_colour(e,1);
	Textstyle_Data title_data;
	Textstyle_Data heading_data[cols];
	Textstyle_Data cell_data[cols];
	set_textstyle_data_table_element_default(title_data);		Set_colour(title_data,1);		Set_size(title_data,5.0);
	set_textstyle_data_table_element_default(heading_data[1]);
	set_textstyle_data_table_element_default(cell_data[1]);
	for(Integer i=2;i<=cols;i++){
		heading_data[i]	= heading_data[1];
		cell_data[i]	= cell_data[1];
	}
	Real title_ht = 9.66666;
	Real cell_ht = 7.66666;
	Real h_pad = 2.0;
	format_table_element(e,title_data,heading_data,cell_data,column_width,cols,title_ht,cell_ht,h_pad);
	return 0;
}

Integer create_table_element(Text title, Text text, Integer cell_break, Integer row_break, Integer cols, Integer rows, Real default_column_width, Element &e){
	if(cols < 1 || rows < 1)return 1;
	Real column_width[cols];
	for(Integer i=1;i<=cols;i++){
		column_width[i]	= default_column_width;
	}
	return create_table_element(title, text, cell_break, row_break, cols, rows, column_width, e);
}

Integer create_table_element(Text title, Text text, Integer cols, Integer rows, Real column_width[], Element &e){
	Integer cell_break='\t', row_break='\n';
	return create_table_element(title, text, cell_break, row_break, cols, rows, column_width, e);
}

Integer create_table_element(Text title, Text text, Integer cols, Integer rows, Real default_column_width, Element &e){
	Integer cell_break='\t', row_break='\n';
	return create_table_element(title, text, cell_break, row_break, cols, rows, default_column_width, e);
}

Integer create_table_element(Text title, Dynamic_Text dtext, Integer cols, Real default_column_width, Element &e){
	Integer items;
	Get_number_of_items(dtext,items);
	Text text="";
	Integer rows = 0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(dtext,i,t);
		text+=t;
		if(i<items)text+="\n";
		rows++;
	}
	Integer cell_break='\t', row_break='\n';
	return create_table_element(title, text, cell_break, row_break, cols, rows, default_column_width, e);
}

Integer set_table_element_acad_compatible(Element &e,Integer compatible){
	Text value = "none";
	if(compatible)value = "_TABLE";
	return(Set_attribute(e,"_TABLE",value));
}




Integer get_table_column_text_verticies(Element &e, Integer column, Integer &start, Integer &end){
	Integer ok=0;
	Integer do_title, rows, cols;
	Real row_height;
	ok+=get_table_attributes(e, do_title, rows, cols, row_height);

	Integer first_text_vertex, last_text_vertex, first_hidden_segment, last_hidden_segment;
	ok+=get_table_element_verticies(cols, rows, first_text_vertex, last_text_vertex, first_hidden_segment, last_hidden_segment);
	
	if(column < 1 || column > cols){
#if DEBUG
		Print( "ERROR IN: get_table_column_text_verticies(), column "+To_text(column)+" supplied outside range "+To_text(1)+"-"+To_text(cols)+"\n");
#endif
		return 900; // dodgey column supplied
	}
	
	Integer offset = (column - 1) * rows;
	
	start = offset + first_text_vertex;
	end = start + rows - 1;
	
	return 0;
}

Integer set_table_title_style(Element &e, Textstyle_Data title){

	return 0;
}


Integer set_table_heading_style(Element &e, Textstyle_Data heading, Integer column){
	Integer start,end;
	if(get_table_column_text_verticies(e, column, start, end)){
		return 800;
	}
	for(Integer i=start;i<=end;i++){
		
	}
	return 0;
}


Integer set_table_cell_style(Element &e, Textstyle_Data title, Integer column){
	Integer start,end;
	if(get_table_column_text_verticies(e, column, start, end)){
		return 800;
	}
	for(Integer i=start;i<=end;i++){
		
	}
	return 0;
}

Integer set_table_column_width(Element &e, Integer column, Real width){
	
	return 0;
}

Integer set_table_row_height(Element &e, Real row_height){
	
	return 0;
}

#endif