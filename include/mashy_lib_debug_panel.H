#ifndef mashy_debug_panel_included
#define mashy_debug_panel_included

//#include "Set_ups.H"

{
  Integer debug_global_variable = 1;
}

void debug_print(Text title,Text data)
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
{
  if(debug_global_variable == 0) return;

  Panel            panel   = Create_panel(title);
  Vertical_Group   vgroup  = Create_vertical_group(0);
  Message_Box      message = Create_message_box(" ");
  Text_Edit_Box    text_box = Create_text_edit_box("Text",message,10);

Set_width_in_chars(text_box,50);
  //Set_width_in_chars(message,20);
  Set_data          (message,data);
  Set_data          (text_box,data);

// message box

Append(text_box,vgroup);
  //Append(message,vgroup);


  Horizontal_Group bgroup = Create_button_group();

  Button finish  = Create_finish_button("OK"  ,"Finish" );

  Append(finish,bgroup);

  Append(bgroup,vgroup);
  Append(vgroup,panel);

  Show_widget(panel);

// now wait on events

  Integer doit = 1;

  while(doit) {

    Integer id;
    Text    cmd;
    Text    msg;
    Integer ret = Wait_on_widgets(id,cmd,msg);  // this processes standard messages first ?

    if(cmd == "keystroke") continue;

    switch(id) {

      case Get_id(panel) : {

        if(cmd == "Panel Quit") {
          doit = 0;
        }
      } break;

      case Get_id(finish) : {

        doit = 0;

      } break;
    }
  }
}

void debug_print(Text title,Integer data)
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
{
  debug_print(title,To_text(data));
}
void debug_print(Text title,Real data)
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
{
  debug_print(title,To_text(data));
}


void debug_print(Text file,Integer line,Text title,Integer data)
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
{
  Text msg = "File " + file + To_text(line," %ld: ") + To_text(data,"%ld");

  debug_print(title,msg);
}
void debug_print(Text file,Integer line,Text title,Real data)
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
{
  Text msg = "File " + file + To_text(line," %ld: ") + To_text(data,"%lf");

  debug_print(title,msg);
}
void debug_print(Text file,Integer line,Text title,Text data)
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
{
  Text msg = "File " + file + To_text(line," %ld: ") + data;

  debug_print(title,msg);
}

#define DEBUG_PRINT(expression) debug_print(__FILE__,__LINE__,#expression,expression)

#endif

