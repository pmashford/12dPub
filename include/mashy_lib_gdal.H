#ifndef mashy_lib_gdal_H_INCLUDED
#define mashy_lib_gdal_H_INCLUDED
#include "mashy_lib_files.H"

/* 
    WKT well known text
    https://en.wikipedia.org/wiki/Well-known_text_representation_of_geometry
    POLYGON (( 10 10, 20 10, 20 20, 10 20, 10 10))

    https://gdal.org/programs/gdalinfo.html

    https://gdal.org/programs/gdalwarp.html

    https://www.perrygeo.com/lazy-raster-processing-with-gdal-vrts.html
    -s_srs EPSG:xxxxx
  
    VRT VRTs
    https://www.perrygeo.com/lazy-raster-processing-with-gdal-vrts.html

    https://medium.com/@robsimmon/a-gentle-introduction-to-gdal-part-5-shaded-relief-ec29601db654

*/

//https://gdal.org/programs/raster_common_options.html


Text GDAL_find_exe_path(Text base_path_no_slash, Text file_name_with_ext)
{
    Dynamic_Text file_names_full_path;
    if(get_list_of_files_incl_search_of_subfolders(base_path_no_slash , "*"+file_name_with_ext, file_names_full_path))
    {
        p(file_names_full_path);
        Integer items;
        Get_number_of_items(file_names_full_path,items);
        Text t,path,file;
        Get_item(file_names_full_path,items,t);
        get_file_path_filename(t,path,file);
        return path;
        //p(file_names_full_path);
    }else
    {
        Text t = "ERROR searching for Basepath:<"+base_path_no_slash+"> 2ndpart<"+file_name_with_ext+">";
        Print(t);Print(); 
        return t;
    }

   // return( "\"C:\\Program Files\\QGIS 3.34.1\\bin\\"+gdal_exe_name_no_extension+".exe\"");
   return "ERROR - unknown! (InTeRsTinG?!)";
}

Text GDAL_find_exe_path()
{
    //return( GDAL_find_exe_path("C:\\Program Files","gdalinfo.exe") );
    return( GDAL_find_exe_path("C:\\Program Files","gdalwarp.exe") );
}


Dynamic_Text GDAL_gdalinfo(Text gdalpath, Text filename, Text options)
{
    Text gdalfile;
    valid_path(gdalpath+"\\"+"gdalinfo.exe",gdalfile);
    Text command = quotes_add(gdalfile) + " " + quotes_add(filename) + " " + options;
    Dynamic_Text dt = run_system_command_get_output(command);
    // p(dt);
    return dt;
}

Integer GDAL_get_coords_from_info(Text line, Real &value1, Real &value2){
    Integer ret = 0;
    Dynamic_Text parts;
    From_text(line, '"', ' ' , parts);
    Integer items;  Get_number_of_items(parts,items);
    //Print(line + " ->"+To_text(items));Print();
    if(items >= 5){
        Text t1,t2;
        Get_item(parts,1,t1);
        Get_item(parts,2,t2);
        t1+= " " + t2;
        switch (Text_lower(t1)){
            case ("upper left") : {
                ret = 1;
            }break;
            case ("upper right") : {
                ret = 2;
            }break;
            case ("lower right") : {
                ret = 3;
            }break;
            case ("lower left") : {
                ret = 4;
            }break;
            default :{
                return 0;
            }
        }
        Get_item(parts,4,t1);
        Get_item(parts,5,t2);
        t1 = Get_subtext(t1,1,Text_length(t1)-1);
        t2 = Get_subtext(t2,1,Text_length(t2)-1);
        Print(t1);Print();
        Print(t2);Print();
        if( From_text(t1,value1) ||From_text(t2,value2) ) return 0; // if an error abort
    }
    return ret; // all good here
}

Integer GDAL_get_coords_from_info(Dynamic_Text &info, Real x[], Real y[]){ // x[4],y[4] expected, and they will draw a bounding box UL -> UR -> LR -> LL
    for(Integer i=1;i<=4;i++){
        Null(x[i]); Null(y[i]); // init
    }
    Text line;
    Integer items;  Get_number_of_items(info,items);
    Integer ret = 0;
    for(i=1;i<=items;i++){
        Text t;
        Get_item(info,i,t);
        Real v1,v2;
        Integer ierr = GDAL_get_coords_from_info(t,v1,v2);
        if(ierr){
            x[ierr] = v1;   y[ierr] = v2;
            ret+=ierr;
        }
    }
    if(ret == 10){
        Print("...coords found\n");
        return 0; // 10 means we found 1 + 2 + 3 + 4 indexes in the for loop
    }else{
        Print("NO COORDS FOUND!\n");
    }
    return 1;
}

Integer GDAL_get_coords(Text gdalpath, Text filename, Real x[], Real y[]){
    Text options ;
    Dynamic_Text info = GDAL_gdalinfo(gdalpath, filename, options);
    return GDAL_get_coords_from_info(info, x, y);
}

Text element_coords_WKT(Element &e, Integer dec){

    Integer npts;
    Get_points(e,npts);
    Real x1,y1;
    Text t = "";
    for(Integer i=1;i<=npts;i++)
    {
        Real x,y,z;
        Get_data(e,i,x,y,z);
        if(i==1){
            x1 = x; y1 = y;
        }
        if(i>1)t+=",";
        t+=To_text(x,dec) + " " + To_text(y,dec);
        
        if(i==npts){
            if( (To_text(x1) != To_text(x))  && (To_text(y1) != To_text(y)) ) { // make sure it's closed (go back to the start point)
                t+="," + To_text(x1,dec) + " " + To_text(y1,dec);
            }
        }
    }
    return(t);
}

Text element_with_hole_to_WKT_polygon(Element &e, Element &eh)
{
/*
    /////// -> returns a piece of text with new line being the WKT format, makes sure string is closed to whatever (hardcoded) decimal points (1st draft had 3 decimals)
    id,WKT 
    1,"POLYGON ((365400 8136500, 365700 8136500, 365700 8136700, 365400 8136700, 365400 8136500) (365400 8136500, 365400 8136700, 365400 8136500))" 
*/
    Integer dec = 3;
    Text t = "id,WKT\n" + "1,\"POLYGON ((" + element_coords_WKT(e, dec)  + "),(" + element_coords_WKT(eh, dec)  + "))\"";
#if DEBUG
    p("element_with_hole_to_WKT_polygon(e,e)",t);
#endif
    return (t);
}

Text element_to_WKT_polygon(Element &e)
/*
    /////// -> returns a piece of text with new line being the WKT format, makes sure string is closed to whatever (hardcoded) decimal points (1st draft had 3 decimals)
    id,WKT 
    1,"POLYGON ((365400 8136500, 365700 8136500, 365500 8136600, 365700 8136700, 365400 8136700, 365400 8136500))" 
*/
{
    Integer dec = 3;
    Text t = "id,WKT\n" + "1,\"POLYGON ((" + element_coords_WKT(e, dec)  + "))\"";
#if DEBUG
    p("element_to_WKT_polygon(e)",t);
#endif
    return (t);
}

#endif