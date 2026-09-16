#ifndef mashy_lib_debug_included
#define mashy_lib_debug_included

#ifndef mashy_debug_real_decimals
#define mashy_debug_real_decimals 3
#endif
#ifndef mashy_debug_header
#define mashy_debug_header 1
#endif

//#include "X:\12d\MACROS\_INCLUDES\mashy_lib_element_functions.H"

void p(Text t){

	//Print("DEBUG : ");
	Print("DEBUG : "+t+"\n");
	//Print("\n");
}
void p(Integer i){
	p(To_text(i));
}
void p(Real r){
	p(To_text(r,mashy_debug_real_decimals));
}

void p(Text t, Integer i){
	p(t+", "+To_text(i));
}
void p(Text t, Real r){
	p(t+", "+To_text(r,mashy_debug_real_decimals));
}

void p(Text t, Text t2){
	p(t+", "+t2);
}

void p(Vector2 v){
	Real x,y; Get_vector(v,x,y);
	p( "x="+To_text(x,mashy_debug_real_decimals) +",y="+To_text(y,mashy_debug_real_decimals) );
}

void p(Vector3 v){
	Real x,y,z; Get_vector(v,x,y,z);
	p( "x="+To_text(x,mashy_debug_real_decimals) +",y="+To_text(y,mashy_debug_real_decimals)+",z="+To_text(z,mashy_debug_real_decimals) );
}

void p(Vector4 v){
	Real x,y,z,n; Get_vector(v,x,y,z,n);
	p( "x="+To_text(x,mashy_debug_real_decimals) +",y="+To_text(y,mashy_debug_real_decimals)+",z="+To_text(z,mashy_debug_real_decimals)+",n="+To_text(n,mashy_debug_real_decimals) );
}

void p(Text t, Real r,Text t2, Real r2,Text t3, Real r3,Text t4, Real r4,Text t5, Real r5,Text t6, Real r6){
	p(t+", "+To_text(r,mashy_debug_real_decimals)
	+t2+", "+To_text(r2,mashy_debug_real_decimals)
	+t3+", "+To_text(r3,mashy_debug_real_decimals)
	+t4+", "+To_text(r4,mashy_debug_real_decimals)
	+t5+", "+To_text(r5,mashy_debug_real_decimals)
	+t6+", "+To_text(r6,mashy_debug_real_decimals));
}

void p(Text t, Real r,Text t2, Real r2,Text t3, Real r3,Text t4, Real r4,Text t5, Real r5){
	p(t+", "+To_text(r,mashy_debug_real_decimals)
	+t2+", "+To_text(r2,mashy_debug_real_decimals)
	+t3+", "+To_text(r3,mashy_debug_real_decimals)
	+t4+", "+To_text(r4,mashy_debug_real_decimals)
	+t5+", "+To_text(r5,mashy_debug_real_decimals));
}

void p(Text t, Real r,Text t2, Real r2,Text t3, Real r3,Text t4, Real r4){
	p(t+", "+To_text(r,mashy_debug_real_decimals)
	+t2+", "+To_text(r2,mashy_debug_real_decimals)
	+t3+", "+To_text(r3,mashy_debug_real_decimals)
	+t4+", "+To_text(r4,mashy_debug_real_decimals));
}

void p(Text t, Real r,Text t2, Real r2,Text t3, Real r3){
	p(t+", "+To_text(r,mashy_debug_real_decimals)
	+t2+", "+To_text(r2,mashy_debug_real_decimals)
	+t3+", "+To_text(r3,mashy_debug_real_decimals));
}

void p(Text t, Real r,Text t2, Real r2){
	p(t+", "+To_text(r,mashy_debug_real_decimals)
	+t2+", "+To_text(r2,mashy_debug_real_decimals));
}

void p(Text t, Integer i,Text t2, Integer i2,Text t3, Integer i3,Text t4, Integer i4,Text t5, Integer i5,Text t6, Integer i6){
	p(t+", "+To_text(i)
	+t2+", "+To_text(i2)
	+t3+", "+To_text(i3)
	+t4+", "+To_text(i4)
	+t5+", "+To_text(i5)
	+t6+", "+To_text(i6));
}

void p(Text t, Integer i,Text t2, Integer i2,Text t3, Integer i3,Text t4, Integer i4,Text t5, Integer i5){
	p(t+", "+To_text(i)
	+t2+", "+To_text(i2)
	+t3+", "+To_text(i3)
	+t4+", "+To_text(i4)
	+t5+", "+To_text(i5));
}

void p(Text t, Integer i,Text t2, Integer i2,Text t3, Integer i3,Text t4, Integer i4){
	p(t+", "+To_text(i)
	+t2+", "+To_text(i2)
	+t3+", "+To_text(i3)
	+t4+", "+To_text(i4));
}

void p(Text t, Integer i,Text t2, Integer i2,Text t3, Integer i3){
	p(t+", "+To_text(i)
	+t2+", "+To_text(i2)
	+t3+", "+To_text(i3));
}

void p(Text t, Integer i,Text t2, Integer i2){
	p(t+", "+To_text(i)
	+t2+", "+To_text(i2));
}

void p(Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Text t;
		Get_item(dt,i,t);
		p(t);
	}
	if(items <1){
		p("NO_DATA");
	}
}

void p(Dynamic_Integer &dt){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Integer r;
		Get_item(dt,i,r);
		p("",r);
	}
}


void p(Dynamic_Real &dt){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Real r;
		Get_item(dt,i,r);
		p("",r);
	}
}

void print(Text t){
	Print(t+"\n");
}
void print(Integer i){
	print(To_text(i));
}
void print(Real r){
	print(To_text(r,mashy_debug_real_decimals));
}

void print(Text t, Integer i){
	print(t+", "+To_text(i));
}
void print(Text t, Real r){
	print(t+", "+To_text(r,mashy_debug_real_decimals));
}

void print(Text t, Text t2){
	print(t+", "+t2);
}

void print(Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Text t;
		Get_item(dt,i,t);
		print(t);
	}
}

void print(Dynamic_Integer &di){
	Integer items;
	Get_number_of_items(di,items);
	for(Integer i = 1 ; i<= items ; i++){
		Integer t;
		Get_item(di,i,t);
		Print(To_text(t)+"\n");
	}
}

void print(Element &e){
		Text t,tm,te;
		Model m;
		Get_model(e,m);
		Get_name(m,tm);
		Get_name(e,te);
	t = tm + "->" + te;
	Print(t+"\n");
}

void print(Dynamic_Element &delt){
	Integer items;
	Get_number_of_items(delt,items);
	Print("Dynamic Element Items <"+To_text(items)+">\n");
	for(Integer i = 1 ; i<= items ; i++){
		Element e;
		Get_item(delt,i,e);
		print(e);
	}
}

// print data errors, data goes in <>


void printd(Text t){
	Print("<"+t+">"+"\n");
}
void printd(Integer i){
	Print("<"+To_text(i)+">"+"\n");
}
void printd(Real r){
	Print("<"+To_text(r,mashy_debug_real_decimals)+"<"+"\n");
}

void printd(Text t, Integer i){
	Print(t+" <"+To_text(i)+">"+"\n");
}
void printd(Text t, Real r){
	Print(t+" <"+To_text(r,mashy_debug_real_decimals)+">"+"\n");
}

void printd(Text t, Text t2){
	Print(t+" <"+t2+">"+"\n");
}

void printd(Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Text t;
		Get_item(dt,i,t);
		Print("<"+t+">"+"\n");
	}
}

void debug_widget_msg(Integer &id, Text &cmd, Text &msg, Panel &panel, Message_Box mb[], Integer &panel_shown){
	if(panel_shown==0){
		panel = Create_panel("Debugging wait_on_widgets");
		Horizontal_Group hg[1];
		hg[1] = Create_horizontal_group(0);	Append(hg[1],panel);
		Vertical_Group vg[2];
		vg[1] = Create_vertical_group(0);
		Screen_Text st[3];
		st[1] = Create_screen_text("ID");
		st[2] = Create_screen_text("CMD");
		st[3] = Create_screen_text("MSG");
		Append(st[1],vg[1]);Append(st[2],vg[1]);Append(st[3],vg[1]);Append(vg[1],hg[1]);
		vg[2] = Create_vertical_group(0);
		mb[1] = Create_message_box("");Set_width_in_chars(mb[1],50);
		mb[2] = Create_message_box("");
		mb[3] = Create_message_box("");
		Append(mb[1],vg[2]);Append(mb[2],vg[2]);Append(mb[3],vg[2]);Append(vg[2],hg[1]);
		Show_widget(panel);
		panel_shown++;
	}
	Set_data(mb[1],To_text(id));
	Set_data(mb[2],cmd);
	Set_data(mb[3],msg);
}

void debug(Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Text t;
		Get_item(dt,i,t);
		Print("DEBUG:"+t+"\n");
	}
}

void debug(Dynamic_Text &dt, Dynamic_Text &dt2){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Text t,t2;
		Get_item(dt,i,t);
		Get_item(dt2,i,t2);
		Print("DEBUG:"+t+","+t2+"\n");
	}
}
void debug(Dynamic_Real &list){
	Integer items;
	Get_number_of_items(list,items);
	for(Integer i = 1 ; i<= items ; i++){
		Real t,t2;
		Get_item(list,i,t);
		Print("DEBUG:"+To_text(t,mashy_debug_real_decimals)+"\n");
	}
}

void debug(Dynamic_Real &dt, Dynamic_Real &dt2){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i<= items ; i++){
		Real t,t2;
		Get_item(dt,i,t);
		Get_item(dt2,i,t2);
		Print("DEBUG:"+To_text(t,mashy_debug_real_decimals)+","+To_text(t2,mashy_debug_real_decimals)+"\n");
	}
}

void debug(Uid id){
	Text t="";
	Convert_uid(id,t);
	Print("DEBUG UID:"+t+"\n");
}

#endif