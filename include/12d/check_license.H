#ifndef check_license_included
#define check_license_included

#include "12d/standard_library.H"

void license_not_available_panel(Text option_name)
// ----------------------------------------------------------
// ----------------------------------------------------------
{
  Panel            panel   = Create_panel(option_name);
  Vertical_Group   vgroup  = Create_vertical_group(0);
  Screen_Text      message = Create_screen_text("no licence available");
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
Integer is_license_available(Text option_name,Text license)
// ----------------------------------------------------------
// ----------------------------------------------------------
{
  if(Get_module_license(license) == TRUE) return(1);

  license_not_available_panel(option_name);
  return(0);
}
Integer is_license_available(Text option_name,Integer no_licenses,Text license[])
// ----------------------------------------------------------
// ----------------------------------------------------------
{
  for(Integer i=1;i<=no_licenses;i++) {
    if(Get_module_license(license[i]) == TRUE) return(1);
  }
  license_not_available_panel(option_name);
  return(0);
}
Integer is_drainage_sewer_license_available(Text option_name)
// ----------------------------------------------------------
// ----------------------------------------------------------
{
  Text modules[2];

  modules[1] = "drainage";
  modules[2] = "sewer";

  return(is_license_available(option_name,2,modules));
}

#endif

