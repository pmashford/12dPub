#ifndef mashy_lib_carto_h_included
#define mashy_lib_carto_h_included

#include "mashy_lib_files.H"

#ifndef DEG2RAD
#define DEG2RAD (Pi()/180.0)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0/Pi())
#endif


//read carto.4d
Integer read_carto_dot_4d_file(Dynamic_Text &file_contents){
	Text path = Find_system_file ("carto.4d","","");
	if(path =="")return(1);
#if DEBUG
	Print(path);Print();
#endif
	return( open_file_save_list(path,file_contents) );
}

Integer get_carto_definition_names(Dynamic_Text &dt, Dynamic_Text &names){
	Integer items,count=0;
	Get_number_of_items(dt,items);
	for(Integer i=1;i<=items;i++){
		Text t;
		Dynamic_Text parts;
		Get_item(dt,i,t);
		if( !From_text(t,parts) )continue;
		Get_item(parts,1,t);
		Append(t,names);
		count++;
	}

	return (count);
}

#define CARTO_PROJ	1
#define CARTO_A		2
#define CARTO_RF	4
#define CARTO_K		8
#define CARTO_ZONE	16
#define CARTO_HEMISPHERE	32
#define CARTO_X_0	64
#define CARTO_Y_0	128
#define CARTO_LON_0	256
#define CARTO_LAT_0	512

//"Irish National Grid" "Transverse Mercator" "+proj=tmerc +a=6377340.189 +rf=299.324951384 +x_0=200000 +y_0=250000 +lon_0=-8.0000000000 +lat_0=53.5000000000 +k=1.000035"
//"MGA94 Zone 50" "UTM" "+proj=utm +a=6378137.0 +rf=298.257222101 +k=0.9996 +zone=50 +south"
Integer get_data_from_carto(Dynamic_Text &haystack, Text needle, Text &proj, Real &a, Real &rf, Real &k, Integer &zone, Text &hemisphere, Real &x_0, Real &y_0, Real &lon_0, Real &lat_0){
	Integer items;
	Get_number_of_items(haystack,items);
	for(Integer i=1;i<=items;i++){
		Text text;
		Get_item(haystack,i,text);
		Dynamic_Text parts;
		From_text(text,parts);
		Integer j;
		Get_number_of_items(parts,j);
		if(j<1)continue;
		Text name;
		Get_item(parts,1,name);
		if(name==needle){
			Text grid_type;
			Get_item(parts,2,grid_type);
			Text t;
			Get_item(parts,3,t);
			Dynamic_Text fields;
			if( t=="" || !From_text(t,fields)){
				Print("Error!");
				Print("<"+text+">");
				Print("Problem with the fields <"+t+">");Print();continue;
			}
			Integer supplied_fields = 0;
			Integer num_fields;
			Get_number_of_items(fields,num_fields);
			for(Integer jj=1;jj<=num_fields;jj++){
				Text data;
				Get_item(fields,jj,data);
				//all field names start with a +, find and trim it
				Integer pos = Find_text(data,"+");
				if(pos == 1){
					data=Get_subtext(data,2,Text_length(data));
					pos = Find_text(data,"=");
					Text field,value;
					if(pos == 0){
						field = data;	value = "";
					}else{
						field = Get_subtext(data,1,pos-1);
						value = Get_subtext(data,pos+1,Text_length(data));
					}
					Integer ierr=0;
					switch(field)
					{
						case "proj" :	{
							proj = value;
							supplied_fields+=CARTO_PROJ;
						}break;

						case "a" :	{
							ierr = From_text(value,a);
							supplied_fields+=CARTO_A;
						}break;

						case "rf" :	{
							ierr = From_text(value,rf);
							supplied_fields+=CARTO_RF;
						}break;

						case "k" :	{
							ierr = From_text(value,k);
							supplied_fields+=CARTO_K;
						}break;

						case "zone" :	{
							ierr = From_text(value,zone);
							supplied_fields+=CARTO_ZONE;
						}break;

						case "north" :	{
							hemisphere = field;
							supplied_fields+=CARTO_HEMISPHERE;
						}break;

						case "south" :	{
							hemisphere = field;
							supplied_fields+=CARTO_HEMISPHERE;
						}break;

						case "x_0" :	{
							ierr = From_text(value,x_0);
							supplied_fields+=CARTO_X_0;
						}break;

						case "y_0" :	{
							ierr = From_text(value,y_0);
							supplied_fields+=CARTO_Y_0;
						}break;

						case "lon_0" :	{
							ierr = From_text(value,lon_0);
							supplied_fields+=CARTO_LON_0;
						}break;

						case "lat_0" :	{
							ierr = From_text(value,lat_0);
							supplied_fields+=CARTO_LAT_0;
						}break;

						default :{

						}break;

					}
					if(ierr){
						Print("Switch error!");Print();
					}


				}else{	grid_type = "+ error";	break;	}			//force a break and error message, no +
			}

			if(grid_type == "Transverse Mercator"){
				if ( supplied_fields != (CARTO_PROJ+CARTO_A+CARTO_RF+CARTO_X_0+CARTO_Y_0+CARTO_LON_0+CARTO_LAT_0+CARTO_K) ){
					Print("Transverse Mercator format error! <"+text+">");Print();continue;
				}else{
					return (0);
				}
			}else if (grid_type == "UTM"){
				if ( supplied_fields != (CARTO_PROJ+CARTO_A+CARTO_RF+CARTO_K+CARTO_ZONE+CARTO_HEMISPHERE) ){
					Print("UTM format error! <"+text+">");Print();continue;
				}else{
					return (0);
				}



			}else if(grid_type=="+ error"){
				Print("Error!");
				Print("<"+text+">");
				Print("field name must be prefixed with +");Print();continue;
			}else{ //unknown -> error!
				Print("Error!");
				Print("<"+text+">");
				Print("Unknown grid type <"+grid_type+">");Print();continue;
			}
		}else{
			continue;
		}
	}

	return (-1);
}

void UTMtoLL(Real UTMEasting, Real UTMNorthing, Real a, Real rf, Real k, Integer zone, Text hemisphere, Real &lat, Real &lon)
{
//converts UTM coords to lat/long.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative.
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees.
	//Written by Chuck Gantz- chuck.gantz@globalstar.com

	//Modified by Paul Mashford for 4dm
	//k (scale factor)
	//a (the ellipsoids semi major axis) = half the length of the major axis
	//rf = reciprical of flattening => // F = 1 / rf;
	// http://en.wikipedia.org/wiki/Geodetic_system

	Real F = 1 / rf; //
	Real b = a*(1-F); // semi minor axis
	Real eccSquared = 1-b/a*b/a;
	Real eccPrimeSquared;
	Real e1 = (1-Sqrt(1-eccSquared))/(1+Sqrt(1-eccSquared));
	Real N1, T1, C1, R1, D, M;
	Real LongOrigin;
	Real mu, phi1, phi1Rad;
	Real x, y;
	Integer ZoneNumber;
	Text ZoneLetter;
	Integer NorthernHemisphere; //1 for northern hemispher, 0 for southern

	x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
	y = UTMNorthing;

	ZoneNumber = zone;

	if(hemisphere == "north"){
		NorthernHemisphere = 1;
	}else{
		NorthernHemisphere = 0;
		y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
	}

	// There are 60 zones with zone 1 being at West -180 to -174
	LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone

	eccPrimeSquared = (eccSquared)/(1-eccSquared);

	M = y / k;
	mu = M/(a*(1-eccSquared/4-3*eccSquared*eccSquared/64-5*eccSquared*eccSquared*eccSquared/256));

#if DEBUG
	Print("--------------------------------------");Print();
	Print("DEBUG");Print();
	Print("UTMEasting<"+To_text(UTMEasting,DEBUG_DEC)+"> UTMNorthing <"+To_text(UTMNorthing,DEBUG_DEC)+">");Print();
	p("a",a);
	p("b",b);
	p("k",k);
	p("zone",zone);
	p(hemisphere);
	p("eccSquared",eccSquared);
	p("M",M);
	p("mu",mu);
#endif

	phi1Rad = mu	+ (3*e1/2-27*e1*e1*e1/32)*Sin(2*mu)
				+ (21*e1*e1/16-55*e1*e1*e1*e1/32)*Sin(4*mu)
				+(151*e1*e1*e1/96)*Sin(6*mu);
	Real local_rad2deg = 180.0 / Pi();
	phi1 = phi1Rad*local_rad2deg;

	N1 = a/Sqrt(1-eccSquared*Sin(phi1Rad)*Sin(phi1Rad));
	T1 = Tan(phi1Rad)*Tan(phi1Rad);
	C1 = eccPrimeSquared*Cos(phi1Rad)*Cos(phi1Rad);
	R1 = a*(1-eccSquared)/Pow(1-eccSquared*Sin(phi1Rad)*Sin(phi1Rad), 1.5);
	D = x/(N1*k);

#if DEBUG
	p("phi1Rad",phi1Rad);
	p("phi1",phi1);
	p("N1",N1);
	p("T1",T1);
	p("C1",C1);
	p("R1",R1);
	p("D",D);
#endif

	lat = phi1Rad - (N1*Tan(phi1Rad)/R1)*(D*D/2-(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
					+(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared-3*C1*C1)*D*D*D*D*D*D/720);
	lat = lat * local_rad2deg;

	lon = (D-(1+2*T1+C1)*D*D*D/6+(5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)
					*D*D*D*D*D/120)/Cos(phi1Rad);
	lon = LongOrigin + lon * local_rad2deg;

#if DEBUG
	p("LongOrigin",LongOrigin);
	Print("Calculated Results");Print();
	Print("lat<"+To_text(lat,DEBUG_DEC)+"> lon <"+To_text(lon,DEBUG_DEC)+">");Print();
	Print("--------------------------------------");Print();
#endif
}

Dynamic_Element translate_elements(Dynamic_Element &delt,Real a, Real rf, Real k, Integer zone,Text hemisphere){
	Dynamic_Element new_delt;
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element original,e;
		Get_item(delt,i,original);
		//Text type;
		//Get_type(original,type);
		//Print(type);Print();
		Convert(original,"Super",e);
		Integer points;
		Get_points(e,points);
		for(Integer j=1;j<=points;j++){
			Real x,y,z,r;
			Integer f;
			Get_super_data(e,j,x,y,z,r,f);
			Real lat,lon;
			UTMtoLL(x, y, a, rf, k, zone, hemisphere, lat, lon);
			//Print("OLD "+To_text(x  ,4)+" "+To_text(y  ,4));Print();
			//Print("NEW "+To_text(lat,4)+" "+To_text(lon,4));Print();
			Set_super_data(e,j,lat,lon,z,r,f);
		}
		Append(e,new_delt);
	}

	return(new_delt);
}

#endif