#ifndef practise_version_included
#define practise_version_included

#include "12d/standard_library.H"

void practise_version_panel()
// ----------------------------------------------------------
// ----------------------------------------------------------
{
  Panel            panel   = Create_panel("Practise Version");
  Vertical_Group   vgroup  = Create_vertical_group(0);
  Screen_Text      message = Create_screen_text("This is not available in the practise version");
  Horizontal_Group bgroup  = Create_button_group();

  Append(message,vgroup);

  Button finish = Create_button("OK"  ,"finish");

  Append(finish,bgroup);
  Append(bgroup,vgroup);
  Append(vgroup,panel);

  Show_widget(panel);

  Integer doit = 1;

  while(doit) {

    Integer id;
    Text    cmd;
    Text    msg;
    Integer ret = Wait_on_widgets(id,cmd,msg);  // this processes standard messages first ?

    if(process_messages_for_simple_panel(id,cmd,msg,panel,finish,doit)) continue;
  }
}
Integer is_practise_version()
// ----------------------------------------------------------
// ----------------------------------------------------------
{
  if(Get_module_license("practise") == TRUE) {

    practise_version_panel();
    return(1);
  }
  return(0);
}

#endif

