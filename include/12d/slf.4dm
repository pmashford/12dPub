#include "12d/slf.H"

void new_test()
// --------------------------------------------------------------------
// --------------------------------------------------------------------
{
  Text data;
  Text prefix = "Active Polygons";

  pad = 2;

  data += group("Widget_Pages","Advanced");

    data += field("Current_Page","1");
    data += group("Widget_Page" ,"1");

      data += group("Source_Box",prefix);

        data += field(prefix,"Mode"        ,"Source_Box_Filter");
        data += group(prefix,"Widget_Pages","Groups");

          data += field("Current_Page","2");
          data += group("Widget_Page" ,"1");

            data += field(prefix,"View Name" ,"1");

          data += group();
          data += group("Widget_Page" ,"2");

            data += field(prefix,"Model Name","a");

          data += group();
        data += group();
      data += group();
    data += group();
  data += group();

  Text    name        = "Polygon Logic Panel";
  Integer interactive = 1;

  Panel_prompt(name,interactive,data);
}
void carto_test()
// --------------------------------------------------------------------
// --------------------------------------------------------------------
{
  Text data;
  Text prefix;

  pad = 2;

// program the source box

  prefix = "Data to change";

  data += group("Source_Box",prefix);

    data += field(prefix,"Mode","Source_Box_View");
    data += field(prefix,"View","1");

  data += group();

// program the transformation fields

  data += field("X/Y Coordinates to Long/Lat", "BORNEO RSO");
  data += field("Long/Lat to X/Y Coordinates", "");
  data += field("Long/Lat stored as"         ,"radians");

// program the target box

  prefix = "Target";

  data += group("Target_Box",prefix);
    
    data += field(prefix,"Mode"                ,"Target_Box_Copy_To_Many_Models");
    data += field(prefix,"Copy to model prefix","data lat long");

  data += group();

// start the panel

  Text    name        = "Cartographic";
  Integer interactive = 1;

  Panel_prompt(name,interactive,data);
}
void main()
{
  if(1) {
    carto_test();
  } else {
    new_test();
  }
}


