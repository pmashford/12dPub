#ifndef mashy_lib_time_h_included
#define mashy_lib_time_h_included

// http://www.cplusplus.com/reference/ctime/strftime/

//from a mike jensen example (xml calls)
Text local_time()
{
  Text retval;
  Text day,month,year,time;
  Dynamic_Text DT;

  Time(time);

  From_text(time,DT);
  Get_item(DT,2,month);
  Get_item(DT,3,day);
  Get_item(DT,4,time);
  Get_item(DT,5,year);

  switch(month)
  {
    case "Jan": {month = "01"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Feb": {month = "02"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Mar": {month = "03"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Apr": {month = "04"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "May": {month = "05"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Jun": {month = "06"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Jul": {month = "07"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Aug": {month = "08"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Sep": {month = "09"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Oct": {month = "10"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Nov": {month = "11"; retval = day + "-" + month + "-" + year + " " + time;}break;
    case "Dec": {month = "12"; retval = day + "-" + month + "-" + year + " " + time;}break;
    default:    {retval = "";}
  }

  return retval;
}

Text utc_time()
{
  Text retval;
  Integer dd;

  Time(dd);
  retval = To_text(dd);

  return retval;
}

Text get_timestamp(){

   Integer time;   Time(time);
   //time -= (10*60*60);
   Text format = "%d-%m-%Y %H:%M:%S";
   Text time_text;   Convert_time(time,format,time_text);
   return ( time_text );

//   Integer d,m,y;   Date(d,m,y);
//   return ( To_text(d) + "-" + To_text(m) + "-" + To_text(y) + " 00:00:00");
}

Text get_timestamp_adjust(Integer hours){
   //will adjust time by hours
   Integer time;   Time(time);
   time += (hours*60*60);
   Text format = "%d-%m-%Y %H:%M:%S";
   Text time_text;   Convert_time(time,format,time_text);
   return ( time_text );

//   Integer d,m,y;   Date(d,m,y);
//   return ( To_text(d) + "-" + To_text(m) + "-" + To_text(y) + " 00:00:00");
}

Text get_timestamp_adjust_qld(){
	return get_timestamp_adjust(-10);
}

#endif