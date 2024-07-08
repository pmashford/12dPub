#define SELECT_STRING   5509
#define CHECK_VIEW_MUST_EXIST  2
#define VIEW_EXISTS         7


Integer check_do_slide(Real x[], Real y[], Real temp_x, Real temp_y){

    Real dx = x[2] - x[1];
    Real dy = y[2] - y[1];
    Real len = Sqrt(dx*dx + dy*dy);
    Real udx = dx / len;
    Real udy = dy / len;
    Real vdx = temp_x - x[1];
    Real vdy = temp_y - y[1];
    Real d = vdx*udx + vdy*udy;
    dx = temp_x - ( x[1] + d*udx );
    dy = temp_y - ( y[1] + d*udy );

    if( (Absolute(dx)>0.001) || (Absolute(dy)>0.001) ){
        x[1]+=dx;   y[1]+=dy;
        x[2]+=dx;   y[2]+=dy;
        return (1);
    }
    return (0);
}

void rotate_coords_about_midpoint(Real x[], Real y[], Real xb[], Real yb[]){
    Real mx = (x[1] + x[2])/2.0;
    Real my = (y[1] + y[2])/2.0;
    Real r = Half_pi();
    xb[1] = mx+(x[1]-mx)*Cos(r)-(y[1]-my)*Sin(r);
    yb[1] = my+(x[1]-mx)*Sin(r)+(y[1]-my)*Cos(r);
    xb[2] = mx+(x[2]-mx)*Cos(r)-(y[2]-my)*Sin(r);
    yb[2] = my+(x[2]-mx)*Sin(r)+(y[2]-my)*Cos(r);
}

Integer is_same(View v1, View v2){
    Text t[2];
    Get_name(v1,t[1]);
    Get_name(v2,t[2]);
    return (t[1] == t[2]);
}

void dynamic_section_panel(){

    Panel panel = Create_panel("Dxs");
    Vertical_Group vg = Create_vertical_group(0);
    Message_Box message_box = Create_message_box("");
    View_Box view_box = Create_view_box("Section",message_box,CHECK_VIEW_MUST_EXIST);    Set_view_type(view_box,3);
    //View_Box view_red_box = Create_view_box("Red",message_box,CHECK_VIEW_MUST_EXIST);    Set_view_type(view_red_box,3);    // Set_width_in_chars(view_red_box,5);
    View_Box view_cyan_box = Create_view_box("90 deg",message_box,CHECK_VIEW_MUST_EXIST);    Set_view_type(view_cyan_box,3); // Set_width_in_chars(view_cyan_box,5);
    Set_optional(view_cyan_box,1);

    //Append(view_red_box,vg);
    Append(view_box,vg);
    Append(view_cyan_box,vg);



    #if 0
    Button button_1 = Create_button("Start 1pt stretch","start stretch"); // pt 1 is fixed, cursor 2 moves, 2 restarts
    Button button_2 = Create_button("Start 2pt pan","start pan"); // pt 1 is fixed, pt 2 sets up section segment, cursor pans
    Button button_3 = Create_button("Start 2pt slide","start slide"); // pt 1 is fixed, pt 2 sets up section segment, cursor slides the section perpendicular
    Append(button_1,vg);
    Append(button_2,vg);
    Append(button_3,vg);
    #else
    Button button_1 = Create_button("1pt stretch","start stretch"); // pt 1 is fixed, cursor 2 moves, 2 restarts
    Button button_2 = Create_button("2pt pan","start pan"); // pt 1 is fixed, pt 2 sets up section segment, cursor pans
    Button button_3 = Create_button("2pt slide","start slide"); // pt 1 is fixed, pt 2 sets up section segment, cursor slides the section perpendicular
    Horizontal_Group bg = Create_button_group();
    Append(button_1,bg);
    Append(button_2,bg);
    Append(button_3,bg);
    Append(bg,vg);
    #endif
    Append(vg,panel);


    //Vertical_Group vg_all = Create_vertical_group(0);

    Show_widget(panel);

	Select_Box select_box = Create_select_box("Pick pnts then move cursor to view on section view(s)","... do it",SELECT_STRING,message_box);
	Set_data(select_box ,"hidden") ;

#if VERSION_4D >1400
    Integer highlight_id = Highlight_create();
    Real highlight_weight = 4.0;
    Highlight_set_weight(highlight_id, highlight_weight);
    //Text highlight_style = "DASH2-1";
    Text highlight_style = "CL10-1";
    Highlight_set_linestyle(highlight_id,highlight_style);
    Integer highlight_colour=1;
    Integer highlight_cyan=4;
#endif

    Integer doit = 1;

    Real x[2],y[2],z[2];   Null(z[1]);  Null(z[2]);     Real r[2];  r[1] = 0.0; r[2] = r[1]; Integer f[2];   f[1] = 0;   f[2] = f[1];
    Real xb[2],yb[2];
    Integer p1_defined = 0, p2_defined = 0;
    Element e = Create_super(0,x,y,z,r,f,2);
    Element eb = Create_super(0,xb,yb,z,r,f,2);
    Model m = Get_model_create("12d no show");
    Set_model(e,m);
    Set_model(eb,m);

    Integer pan_mode = 1;
    Integer slide_mode = 1;


    while(doit){
		Integer id;
		Text cmd,msg;
        Wait_on_widgets(id,cmd,msg);
        //Print("id<"+To_text(id) +"> <"+ cmd +"> msg<"+ msg+">");Print();
        if(cmd == "Panel Quit"){
            doit = 0;
        }
        if(cmd == "CodeShutdown"){
#if VERSION_4D > 1400
            Set_exit_code(cmd);
#endif
            doit = 0;
            break;
        }
        if(cmd == "start stretch" || cmd == "start pan" || cmd == "start slide"){
            p1_defined = 0; p2_defined = 0;
            Select_start(select_box);
            pan_mode = (cmd=="start pan") ? 1 : 0;
            slide_mode = (cmd=="start slide") ? 1 : 0;
        }
        if(cmd == "accept select"){
            Real xf,yf,zf,chf,htf;
            Get_select_coordinate(select_box,xf,yf,zf,chf,htf);
            if(!p1_defined){
                p1_defined = 1;
                x[1] = xf;  y[1] = yf;
            }else{
                x[2] = xf;  y[2] = yf;
                if(p2_defined || (!pan_mode && !slide_mode)){
                    p1_defined = 0; p2_defined = 0;
                    Select_start(select_box);
                    continue;
                }
                p2_defined = 1;
            }
            Select_start(select_box);
        }
        if(cmd == "motion select" && p1_defined){

            Dynamic_Text msgs;
			From_text(msg,msgs);
			Text t;
            Real temp_x,temp_y;
			Get_item(msgs,1,t);From_text(t,temp_x);
            Get_item(msgs,2,t);From_text(t,temp_y);
            if(!p2_defined){
                x[2] = temp_x;      y[2] = temp_y;
            }
            Real dx = temp_x - x[2];    Real dy = temp_y - y[2];

            if(pan_mode){
                x[1]+=dx;   y[1]+=dy;
            }
            if(slide_mode){
                check_do_slide(x, y, temp_x, temp_y);
                dx=0.0;dy=0.0;
            }
            x[2]+=dx;   y[2]+=dy;


#if VERSION_4D > 1400
            Highlight_reset(highlight_id);
            Highlight_add_line(highlight_id, highlight_colour,x[1],y[1],x[2],y[2]);
#endif
            View view,viewb;
            Integer check = 0;
            if(Validate(view_box,CHECK_VIEW_MUST_EXIST,view) == VIEW_EXISTS){
                Set_super_data(e,x,y,z,r,f,2);
                Calc_extent(e);
                Section_view_profile(view,e,1);
                if(Validate(view_cyan_box,CHECK_VIEW_MUST_EXIST,viewb) == VIEW_EXISTS){
                    if(!is_same(view,viewb)){
                        rotate_coords_about_midpoint(x, y, xb, yb);
                        Set_super_data(eb,xb,yb,z,r,f,2);
                        Calc_extent(eb);
                        Section_view_profile(viewb,eb,1);
#if VERSION_4D > 1400
                        Highlight_add_line(highlight_id, highlight_cyan,xb[1],yb[1],xb[2],yb[2]);
#endif
                    }
                }
            }
#if VERSION_4D > 1400
            Highlight_redraw(highlight_id);
#endif
        }
    }
    Element_delete(e);
#if VERSION_4D > 1400
    Highlight_remove(highlight_id);
#endif
}


void main(){
    dynamic_section_panel();
    Print("Dynamic_Section_Panel.4do by Paul Mashford\n");
}