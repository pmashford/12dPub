#ifndef mashy_lib_time_h_included
#define mashy_lib_time_h_included


Integer is_leap_year(Integer YYYY){ // tested
  if( ( (YYYY % 4 == 0) && (YYYY % 100 != 0) ) || (YYYY % 400 == 0) ){
    return 1;
  }
  return 0;
}

// Given a date + time, return seconds since Jan 1 1970 ... TESTED OK
Integer time_from(Integer year, Integer month, Integer day, Integer hrs, Integer mins, Integer secs){
  if(year < 1970) return -1; // crap data, return error
  Integer days=0;
  days = (year - 1970) * 365; // handle years

  switch ( month) {              // handle months
    case 1  :{ days += 0;}break;
    case 2  :{ days += 31;}break;
    case 3  :{ days += 59;}break;
    case 4  :{ days += 90;}break;
    case 5  :{ days += 120;}break;
    case 6  :{ days += 151;}break;
    case 7  :{ days += 181;}break;
    case 8  :{ days += 212;}break;
    case 9  :{ days += 243;}break;
    case 10 :{ days += 273;}break;
    case 11 :{ days += 304;}break;
    case 12 :{ days += 334;}break;
  }
  // handle leap years... how many 29th of Febs have been
  if(month <= 2){
    year = year - 1;  // workings for leap days
  }
  for(Integer i = 1970 ; i <= year ; i++){
    days += is_leap_year(i);
  }
  days += day-1;

  //return (days * 24 * 60 * 60) + (hrs * 60 * 60) + (mins * 60) + secs;
  return (days * 86400) + (hrs * 3600) + (mins * 60) + secs;
}


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

Text get_timestamp_for_element_name(){
   Integer time;   Time(time);
   //time -= (10*60*60);
   Text format = "%y-%m-%d %H-%M-%S";
   Text time_text;   Convert_time(time,format,time_text);
   return ( time_text );
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