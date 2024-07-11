#ifndef mashy_lib_textstyle_data_included
#define mashy_lib_textstyle_data_included

#if 0

Integer Null(Textstyle_Data textdata); // ID = 1639
Integer Null(Textstyle_Data textdata,Integer mode); // ID = 1640
Integer Get_textstyle(Textstyle_Data textdata,Text &style); // ID = 1641
Integer Get_colour(Textstyle_Data textdata,Integer &colour); // ID = 1642
Integer Get_text_type(Textstyle_Data textdata,Integer &type); // ID = 1643
Integer Get_size(Textstyle_Data textdata,Real &size); // ID = 1644
Integer Get_offset(Textstyle_Data textdata,Real &offset); // ID = 1645
Integer Get_raise(Textstyle_Data textdata,Real &raise); // ID = 1646
Integer Get_justify(Textstyle_Data textdata,Integer &justify); // ID = 1647
Integer Get_angle(Textstyle_Data textdata,Real &angle); // ID = 1648
Integer Get_slant(Textstyle_Data textdata,Real &slant); // ID = 1649
Integer Get_x_factor(Textstyle_Data textdata,Real &xfactor); // ID = 1650
Integer Get_name(Textstyle_Data textdata,Text &name); // ID = 1651
Integer Set_textstyle(Textstyle_Data textdata,Text style); // ID = 1652
Integer Set_colour(Textstyle_Data textdata,Integer colour); // ID = 1653
Integer Set_text_type(Textstyle_Data textdata,Integer type); // ID = 1654
Integer Set_size(Textstyle_Data textdata,Real size); // ID = 1655
Integer Set_offset(Textstyle_Data textdata,Real offset); // ID = 1656
Integer Set_raise(Textstyle_Data textdata,Real raise); // ID = 1657
Integer Set_justify(Textstyle_Data textdata,Integer justify); // ID = 1658
Integer Set_angle(Textstyle_Data textdata,Real angle); // ID = 1659
Integer Set_slant(Textstyle_Data textdata,Real slant); // ID = 1660
Integer Set_x_factor(Textstyle_Data textdata,Real xfactor); // ID = 1661
Integer Set_name(Textstyle_Data textdata,Text name); // ID = 1662
Integer Set_super_vertex_textstyle_data(Element element,Integer vert,Textstyle_Data d); // ID = 1663
Integer Get_super_vertex_textstyle_data(Element element,Integer vert,Textstyle_Data &d); // ID = 1664
Integer Set_super_segment_textstyle_data(Element element,Integer seg,Textstyle_Data d); // ID = 1665
Integer Get_super_segment_textstyle_data(Element element,Integer seg,Textstyle_Data &d); // ID = 1666
Integer Set_4d_textstyle_data(Element element,Textstyle_Data d); // ID = 1667
Integer Get_4d_textstyle_data(Element element,Textstyle_Data &d); // ID = 1668
Integer Set_text_textstyle_data(Element element,Textstyle_Data d); // ID = 1669
Integer Get_text_textstyle_data(Element element,Textstyle_Data &d); // ID = 1670
Textstyle_Data_Box Create_textstyle_data_box(Text text,Message_Box box,Integer flags); // ID = 1671
Integer Validate(Textstyle_Data_Box box,Textstyle_Data &data); // ID = 1672
Integer Set_data(Textstyle_Data_Box box,Textstyle_Data data); // ID = 1673
Integer Get_data(Textstyle_Data_Box box,Textstyle_Data &data); // ID = 1674

Integer Get_data(Textstyle_Data_Box box,Text &data); // ID = 2160
Integer Set_data(Textstyle_Data_Box box,Text data); // ID = 2161
Integer Get_data(Textstyle_Data textdata,Text &data); // ID = 2162
Integer Set_data(Textstyle_Data textdata,Text data); // ID = 2163


Integer Get_ttf_underline(Textstyle_Data textdata,Integer &underline); // ID = 2616
Integer Get_ttf_strikeout(Textstyle_Data textdata,Integer &strikeout); // ID = 2617
Integer Get_ttf_italic(Textstyle_Data textdata,Integer &italic); // ID = 2618
Integer Get_ttf_weight(Textstyle_Data textdata,Integer &weight); // ID = 2619
Integer Set_ttf_underline(Textstyle_Data textdata,Integer underline); // ID = 2620
Integer Set_ttf_strikeout(Textstyle_Data textdata,Integer strikeout); // ID = 2621
Integer Set_ttf_italic(Textstyle_Data textdata,Integer italic); // ID = 2622
Integer Set_ttf_weight(Textstyle_Data textdata,Integer weight); // ID = 2623

Integer Set_whiteout(Textstyle_Data textdata,Integer colour); // ID = 2753
Integer Get_whiteout(Textstyle_Data textdata,Integer &colour); // ID = 2754

Integer Set_border(Textstyle_Data textdata,Integer colour); // ID = 2763
Integer Get_border(Textstyle_Data textdata,Integer &colour); // ID = 2764

Integer Set_ttf_outline(Textstyle_Data textdata,Integer outline); // ID = 2773
Integer Get_ttf_outline(Textstyle_Data textdata,Integer &outline); // ID = 2774


Textstyle_Data_Box Create_textstyle_data_box(Text text,Message_Box box,Integer flags,Integer optionals); // ID = 2866

Integer View_get_draw_data_textstyle_merge(View view,Text model_name,Text prefix,Textstyle_Data &d,Integer &internal_return); // ID = 3082
Integer View_get_plot_data_textstyle_merge(View view,Text model_name,Text prefix,Textstyle_Data &d,Integer &internal_return); // ID = 3083
Integer View_get_draw_data_textstyle(View view,Text model_name,Text prefix,Textstyle_Data &d,Integer &internal_return); // ID = 3084
Integer View_get_plot_data_textstyle(View view,Text model_name,Text prefix,Textstyle_Data &d,Integer &internal_return); // ID = 3085
Integer View_set_draw_data_textstyle(View view,Text model_name,Text prefix,Textstyle_Data d,Integer &internal_return); // ID = 3086
Integer View_set_plot_data_textstyle(View view,Text model_name,Text prefix,Textstyle_Data d,Integer &internal_return); // ID = 3087

Integer Set_boder_style(Textstyle_Data textdata,Integer style); // ID = 3138
Integer Get_border_style(Textstyle_Data textdata,Integer &style); // ID = 3139

Integer Get_text_controlbar(Textstyle_Data &d); // ID = 3144
Integer Set_text_controlbar(Textstyle_Data d); // ID = 3145

Integer Set_angle2(Textstyle_Data textdata,Real angle2); // ID = 3564
Integer Set_angle3(Textstyle_Data textdata,Real angle3); // ID = 3565
Integer Get_angle2(Textstyle_Data textdata,Real &angle2); // ID = 3566
Integer Get_angle3(Textstyle_Data textdata,Real &angle3); // ID = 3567

Integer Set_border_style(Textstyle_Data textdata,Integer style); // ID = 7837
Integer Highlight_add_text(Integer id,Integer colour,Real x,Real y,Textstyle_Data ts_data,Text text); // ID = 7871
#endif


Integer set_text_type_pixel(Textstyle_Data &textdata){  return Set_text_type( textdata, 0);    }
Integer set_text_type_world(Textstyle_Data &textdata){  return Set_text_type( textdata, 1);    }
Integer set_text_type_paper(Textstyle_Data &textdata){  return Set_text_type( textdata, 2);    }

Integer set_text_justify_left_bottom(Textstyle_Data &textdata){     return Set_justify( textdata, 1);    }
Integer set_text_justify_left_middle(Textstyle_Data &textdata){     return Set_justify( textdata, 2);    }
Integer set_text_justify_left_top(Textstyle_Data &textdata){        return Set_justify( textdata, 3);    }
Integer set_text_justify_middle_bottom(Textstyle_Data &textdata){   return Set_justify( textdata, 4);    }
Integer set_text_justify_middle_middle(Textstyle_Data &textdata){   return Set_justify( textdata, 5);    }
Integer set_text_justify_middle_top(Textstyle_Data &textdata){      return Set_justify( textdata, 6);    }
Integer set_text_justify_right_bottom(Textstyle_Data &textdata){    return Set_justify( textdata, 7);    }
Integer set_text_justify_right_middle(Textstyle_Data &textdata){    return Set_justify( textdata, 8);    }
Integer set_text_justify_right_top(Textstyle_Data &textdata){       return Set_justify( textdata, 9);    }
Integer set_text_justify_centre(Textstyle_Data &textdata){          return Set_justify( textdata, 5);    }



Textstyle_Data create_textstyle_data(){

    Textstyle_Data textdata;

    Set_textstyle( textdata,"Arial"); // ID = 1652
    //Set_whiteout(textdata,Integer colour); // ID = 2753
    //Set_border(textdata,Integer colour); // ID = 2763
    //Set_border_style(textdata,Integer style); // ID = 7837
    set_text_type_world(textdata);        //Set_text_type( textdata,Integer type); // ID = 1654 
    Set_size( textdata, 2.0); // ID = 1655
    Set_offset( textdata, 0.0); // ID = 1656
    Set_raise( textdata, 0.0); // ID = 1657
    set_text_justify_left_bottom(textdata); //Set_justify( textdata,Integer justify); // ID = 1658
    Set_angle( textdata, 0.0); // ID = 1659
    Set_slant( textdata, 0.0); // ID = 1660
    Set_x_factor( textdata, 1.0); // ID = 1661
    Set_ttf_weight( textdata,400); // ID = 2623
    Set_ttf_underline( textdata, 0); // ID = 2620
    Set_ttf_strikeout( textdata, 0); // ID = 2621
    Set_ttf_italic( textdata, 0); // ID = 2622
    //Set_ttf_outline(textdata,Integer outline); // ID = 2773
    //Set_colour( textdata,"1"); // ID = 1653
    //Set_name( textdata,Text name); // ID = 1662
 
    //Set_angle2(textdata,Real angle2); // ID = 3564
    //Set_angle3(textdata,Real angle3); // ID = 3565

    // border vs boder    Set_boder_style(textdata,Integer style); // ID = 3138

	return textdata;
}


Text get_toolbar_textdata_string(){
    Text data = "";
    Textstyle_Data textdata;
    Get_text_controlbar(textdata); // ID = 3144
    Set_data(textdata,data); // ID = 2163
    Print(data+"\n");
    return (data);
}


#endif