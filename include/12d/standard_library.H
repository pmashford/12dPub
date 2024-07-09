#ifndef standard_library_included
#define standard_library_included

#include "12d/set_ups.H"

Integer xeqy(Real x,Real y)
// -------------------------------------------------------
// function to test if x=y within a set tolerence
// -------------------------------------------------------
{
  Real mindif = 1.0e-6;
  if(Absolute(x - y) < mindif) return(1);

  return(0);
}
Integer xeqy(Real x,Real y,Real tolerance)
// -------------------------------------------------------
// function to test if x=y within a set tolerence
// -------------------------------------------------------
{
  if(Absolute(x - y) < tolerance) return(1);

  return(0);
}
Integer xney(Real x,Real y)
// -------------------------------------------------------
// function to test if x=y within a set tolerence
// -------------------------------------------------------
{
  Real mindif = 1.0e-6;
  if(Absolute(x - y) < mindif) return(0);

  return(1);
}
Integer xney(Real x,Real y,Real tolerance)
// -------------------------------------------------------
// function to test if x=y within a set tolerence
// -------------------------------------------------------
{
  if(Absolute(x - y) < tolerance) return(0);

  return(1);
}

Integer xeqy(Point &p1, Point &p2){
	Real x1,y1,x2,y2;
	x1 = Get_x(p1);
	y1 = Get_y(p1);
	x2 = Get_x(p2);
	y2 = Get_y(p2);
	Integer ok = 0;
	ok+=xeqy(x1,x2);
	ok+=xeqy(y1,y2);
	return ok;
}



Integer xgey(Real x,Real y)
// -------------------------------------------------------
// function to test if x>=y within a set tolerence
// -------------------------------------------------------
{
  Real mindif = 1.0e-6;
  if(x >= (y - mindif)) return(1);

  return(0);
}
Integer xgty(Real x,Real y)
// -------------------------------------------------------
// function to test if x>y within a set tolerence
// -------------------------------------------------------
{
  Real mindif = 1.0e-6;
  if(x > (y + mindif)) return(1);

  return(0);
}
Integer xley(Real x,Real y)
// -------------------------------------------------------
// function to test if x<=y within a set tolerence
// -------------------------------------------------------
{
  Real mindif = 1.0e-6;
  if(x <= (y + mindif)) return(1);

  return(0);
}
Integer xlty(Real x,Real y)
// -------------------------------------------------------
// function to test if x<y within a set tolerence
// -------------------------------------------------------
{
  Real mindif = 1.0e-6;
  if(x < (y - mindif)) return(1);

  return(0);
}





Integer xlty(Real x,Real y,Real tol)
// ---------------------------------------------------------------------------
// test if x<y within a set tolerance
// ---------------------------------------------------------------------------
{
  if(x < (y - tol)) return(1);

  return(0);
}
Integer xley(Real x,Real y,Real tol)
// ---------------------------------------------------------------------------
// test if x <= y within a set tolerance
// ---------------------------------------------------------------------------
{
  if(x <= (y + tol)) return(1);

  return(0);
}
Integer xgty(Real x,Real y,Real tol)
// ---------------------------------------------------------------------------
// test if x > y within a set tolerance
// ---------------------------------------------------------------------------
{
  if(x > (y + tol)) return(1);

  return(0);
}
Integer xgey(Real x,Real y,Real tol)
// ---------------------------------------------------------------------------
// test if x >= y within a set tolerance
// ---------------------------------------------------------------------------
{
  if(x >= (y - tol)) return(1);

  return(0);
}





Real minimum(Real x,Real y)
// -------------------------------------------------------
// -------------------------------------------------------
{
  if(x <= y) return(x);

  return(y);
}
Real maximum(Real x,Real y)
// -------------------------------------------------------
// -------------------------------------------------------
{
  if(x >= y) return(x);

  return(y);
}
Real signn(Real value)
// -------------------------------------------------------
// function to return sign of a value
// -------------------------------------------------------
{
  if(value < 0.0) {
    return(-1.0);
  } else if(value > 0.0) {
    return( 1.0);
  }
  return( 0.0);
}
Integer get_centre(Real xs,Real ys,Real xe,Real ye,Real r,Integer f,Real &xc,Real &yc)
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
{
  if(xeqy(r,0.0)) return(-2);

  Real dx = xe - xs;
  Real dy = ye - ys;
  Real l  = Sqrt(dx*dx + dy*dy);

  if(xeqy(xs,xe) && xeqy(ys,ye)) return(-3);

  Real l2 = 0.5*l;

  if(Absolute(r) < l2) return(-4);

  Real s       = signn(r)*((f == 0) ? 1 : -1);
  Real rminusa = Sqrt(r*r - 0.25*l*l)*s;

  xc = 0.5*(xs + xe) + rminusa*(dy/l);
  yc = 0.5*(ys + ye) - rminusa*(dx/l);

  return(0);
}
Real atan3(Real dy,Real dx)
// -------------------------------------------------------
// return angle from 0 to 2 pi
// -------------------------------------------------------
{
  Real a , leng = Sqrt(dx*dx + dy*dy);

  if(leng < 1.0e-06) {
    a = 0.0;
  } else {
    a = Atan2(dy,dx);
    if(a < 0.0) {
      a += Two_pi();
    }
  }
  return(a);
}
Real change_of_angle(Real x1,Real y1,Real x2,Real y2,Real x3,Real y3)
// -----------------------------------------------------------------------------------------
// calculate the change of angle of the two vertors
// -----------------------------------------------------------------------------------------
{
  Real dx = x2 - x1;
  Real dy = y2 - y1;
  Real l2 = dx*dx + dy*dy;

  if(l2 < 1.0e-6) return(0.0);

  Real s  = ((x3 - x1)*dx + (y3 - y1)*dy)/l2;
  Real t  = ((y3 - y1)*dx - (x3 - x1)*dy)/l2;

  Real xn = x1 + s*dx;
  Real yn = y1 + s*dy;

  Real xa = xn - x2 , ya = yn - y2;
  Real x  = Sqrt(xa*xa + ya*ya);
  Real xb = x3 - xn , yb = y3 - yn;
  Real y  = Sqrt(xb*xb + yb*yb);

  Real a  = Absolute(atan3(y,x));

  Real angle;
  if(t > 0.0) {
    if(s > 1.0) { angle =  a; } else { angle = Pi() - a; }
  } else {
    if(s > 1.0) { angle = -a; } else { angle = a - Pi(); }
  }
  return(angle);
}
Real plan_distance(Real x1,Real y1,Real x2,Real y2)
//----------------------------------------------------------------------------------
// -----------------------------------------------------------------------
{
 	Real dx = x2 - x1;
	Real dy = y2 - y1;

 	return(Sqrt(dx*dx + dy*dy));
}
Text replace_env_4d(Text text,Text name)
//----------------------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  Text    env    = "$" + name;
  Text    env4d  = name + "_4D";
  Integer envlen = Text_length(env);

  Text envpath = Getenv(env4d);
  while(1) {

    Integer pos = Find_text(text,env);
    if(pos <= 0) break;

    if(pos == 1) {
      text = envpath + Get_subtext(text,pos+envlen,9999);
    } else {
      text = Get_subtext(text,1,pos-1) + envpath + Get_subtext(text,pos+envlen,9999);
    }
  }
  return(text);
}
Text replace_lib_4d(Text text)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  return(replace_env_4d(text,"LIB"));
}
Text replace_user_lib_4d(Text text)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  return(replace_env_4d(text,"USER_LIB"));
}
Text replace_user_4d(Text text)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  return(replace_env_4d(text,"USER"));
}
Text replace_all(Text text)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  text = replace_user_lib_4d(text);
  text = replace_user_4d    (text);
  text = replace_lib_4d     (text);

  return(text);
}
Text get_macro_name()
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
{

  Text help = Get_macro_name();

// strip off the .4do

  help = Get_subtext(help,1,Text_length(help) - 4);

// we only want the bit after the last \ or /

  Integer start = Find_text(help,"\\");
  if(start == 0) start = Find_text(help,"/");

  while(start) {

    help  = Get_subtext(help,start+1,Text_length(help));
    start = Find_text(help,"\\");
    if(start == 0) start = Find_text(help,"/");
  }
  return(help);
}
Integer fix_date(Text &date)
//----------------------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  date = "  " + Get_subtext(date,5,6) + " " + Get_subtext(date,1,3) + " " + Get_subtext(date,8,11) + "  ";
  return(0);
}
Integer about_panel(Panel panel)
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
{
  Hide_widget(panel);

  Text macro_name = get_macro_name();

#if defined(__BASE_FILE_DATE__)
  Text date       = __BASE_FILE_DATE__;
#else
  Text date       = __DATE__;
#endif

  fix_date(date);

  Panel            panel3          = Create_panel("About Macro");
  Vertical_Group   vgroup          = Create_vertical_group(0);
  Vertical_Group   vgroup2         = Create_vertical_group(0);
  Message_Box      main_message    = Create_message_box(" ");

  Horizontal_Group bgroup          = Create_button_group();

  Set_border(bgroup,"Macro Name");
  Draw_Box draw_box = Create_draw_box(180,180,0);

  Screen_Text screen_txt1 = Create_screen_text(macro_name);
  Append(screen_txt1,bgroup);

  Append(bgroup,vgroup);

  Horizontal_Group bgroup2          = Create_button_group();

  Set_border(bgroup2,"Date Compiled");

  Input_Box text_box2 =   Create_input_box  ("",main_message);
  Set_data(text_box2,date);
  Set_width_in_chars(text_box2,10);
  Use_browse_button(text_box2,0);

  Append(text_box2,bgroup2);

  Append(bgroup2,vgroup);

  Horizontal_Group bgroup4          = Create_button_group();
  Append(draw_box,bgroup4);
  Append(vgroup,bgroup4);
  Append(bgroup4,vgroup2);

  Horizontal_Group bgroup3          = Create_button_group();

  Button okbut = Create_button  ("OK" ,"OK");
  Set_width_in_chars(okbut,5);

  Append(okbut  ,bgroup3);

  Append(bgroup3,vgroup2);
  Append(vgroup2,panel3);

  Show_widget(panel3,350,300);
  Integer doit = 1;

  Integer draw_box_width,draw_box_height;
  Get_size(draw_box,draw_box_width,draw_box_height);

  Text path = replace_lib_4d("$LIB/4dlogo2.bmp");

  Start_batch_draw(draw_box);

////the following RGB values match my screen setup
////set it to Clear(draw_box,-1,0,0) to see if you can get the window default
////or if that doesn't work set it to your RGB values

  Clear(draw_box,192,192,192);
  Draw_transparent_BMP(draw_box,path,0,draw_box_height-0);
  End_batch_draw(draw_box);

  while(doit) {

    Integer id;
    Text    cmd;
    Text    msg;
    Integer ret = Wait_on_widgets(id,cmd,msg);  // this processes standard messages first ?

    switch(id) {

      case Get_id(panel3) : {

        if(cmd == "Panel Quit") {
          doit = 0;
        }
      } break;

      case Get_id(okbut) : {

        doit = 0;

      } break;
    }
  }
  Show_widget(panel);

  return(0);
}
Integer process_messages_for_simple_panel(Integer id,Text cmd,Text msg,Panel &panel,Button &finish,Integer &doit)
// -----------------------------------------------------------------------
// this routine handles all basic panel messages in the default fashion
//
// normally this fragment of code is duplicated for every message loop of a macro
//
// if you want any different event handling , replace the call "process_messages_for_simple_panel"
// with the code below !
//
// then you may want to monitor keystrokes , set_focus , kill_focus
// in a non default fashion
// -----------------------------------------------------------------------

#if 0

    if(process_messages_for_simple_panel(id,cmd,msg,panel,finish,doit)) continue;

    switch(id) {

#endif

#if 0

    switch(cmd) {

      case "keystroke" : {

        continue;

      } break;

      case "set_focus"  :
      case "kill_focus" : {

        continue;

      } break;

      case "Help" : {

        Winhelp(panel,"4d.hlp",'a',msg);
        continue;

      } break;
    }
    switch(id) {

      case Get_id(panel) : {

        if(cmd == "Panel Quit")  doit = 0;
        if(cmd == "Panel About") about_panel(panel);

      } break;

      case Get_id(finish) : {

        if(cmd == "finish") doit = 0;

      } break;

#endif

// -----------------------------------------------------------------------
{
  switch(cmd) {

    case "keystroke" : {

      return(1);

    } break;

    case "set_focus"  :
    case "kill_focus" : {

      return(1);

    } break;

    case "Help" : {

      Winhelp(panel,"4d.hlp",'a',msg);
      return(1);

    } break;
  }
  switch(id) {

    case Get_id(panel) : {

      if(cmd == "Panel Quit") {

        doit = 0;
        return(1);

      } else if(cmd == "Panel About") {

        about_panel(panel);
        return(1);
      }
    } break;

    case Get_id(finish) : {

      if(Text_lower(cmd) == "finish") {

        doit = 0;
        return(1);
      }
    } break;
  }
  return(0);
}

#if VERSION_4D < 410

Text Language(Text a,Text b)
// ------------------------------------------------------------------------------
// this is a standard function in v4.10/v5.00
// ------------------------------------------------------------------------------
{

#if defined(CODEPAGE_4D)
  return(b);
#else
  return(a);
#endif

}

#endif

#endif