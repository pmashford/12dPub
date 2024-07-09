#ifndef standard_dialogs_included
#define standard_dialogs_included

Integer 4d_file_read_error(Text line1, Integer x, Integer y)
{
  Panel            panel              = Create_panel("File_Box");
  Vertical_Group   vgroup    = Create_vertical_group(0);
  Message_Box      message         = Create_message_box(" ");
  Horizontal_Group button_group = Create_button_group();
  Vertical_Group vgroup2    = Create_vertical_group(0);
//  Set_border(button_group,"-");

  Draw_Box draw_box = Create_draw_box(50,50,0);


// screen text
  Screen_Text screen_txt1 = Create_screen_text(line1);
  Append(screen_txt1,vgroup2);
  

  Append(draw_box,button_group);
  Append(vgroup2,button_group);

  Horizontal_Group button_group2 = Create_button_group();

  Button ok_button = Create_button("   OK   ","OK");
  Append(ok_button,button_group2);

  Append(button_group,vgroup);
  Append(button_group2,vgroup);
  Append(vgroup,panel);
    
  Show_widget(panel,x,y);
  Integer doit=1;

  Integer draw_box_width,draw_box_height;
  Get_size(draw_box,draw_box_width,draw_box_height);

  Text path = replace_lib_4d("$LIB/4Dfile_box.bmp");

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
    Integer ret = Wait_on_widgets(id,cmd,msg);
    if(cmd == "keystroke") continue;

    switch(id) {

      	case Get_id(panel) : {
	  if(cmd == "Panel Quit") doit=0;
	} break;

     	default : {

		if(cmd == "OK")  doit=0;

	}
    }
  }

  return (0);
}

#endif

