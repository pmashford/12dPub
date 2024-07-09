#ifndef about_panel_included
#define about_panel_included

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

#endif

