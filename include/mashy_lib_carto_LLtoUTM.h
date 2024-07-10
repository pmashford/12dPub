#ifndef mashy_lib_carto_LLtoUTM_included
#define mashy_lib_carto_LLtoUTM_included

// http://mahi.ucsd.edu/class233/data/Basic/LatLongUTMconversion.py

// *** also see http://ereimer.net/programs/LatLong-UTM.cpp
//              http://utexas-art-ros-pkg.googlecode.com/svn/trunk/stacks/art_vehicle/art_common/include/art/UTM.h

// what all the values mean http://www.arsitech.com/mapping/geodetic_datum/

Real get_square_of_eccentricity(Real a, Real rf){
	// a is the major axis (b)
	// rf is flattening inverse (f)
	Real f = 1.0 / rf;
	// need to work out minor axis (b)
	Real b = a - (f * a);
	// first eccentricity          (e)   = sqrt(1-(b^2/a^2))
	//Real e = Sqrt( 1 - ( Pow(b,2.0) / Pow(a,2.0) ) );
	// first eccentricity squared  (e2)  = (a^2-b^2)/a^2
	Real e2 = ( Pow(a,2.0) -  Pow(b,2.0) ) / Pow(a,2.0) ;
	return e2;
}

Text UTMLetterDesignator(Real Lat){
	Text LetterDesignator;

	if     ((84 >= Lat) && (Lat >= 72))  LetterDesignator = "X";
	else if ((72 > Lat) && (Lat >= 64))  LetterDesignator = "W";
	else if ((64 > Lat) && (Lat >= 56))  LetterDesignator = "V";
	else if ((56 > Lat) && (Lat >= 48))  LetterDesignator = "U";
	else if ((48 > Lat) && (Lat >= 40))  LetterDesignator = "T";
	else if ((40 > Lat) && (Lat >= 32))  LetterDesignator = "S";
	else if ((32 > Lat) && (Lat >= 24))  LetterDesignator = "R";
	else if ((24 > Lat) && (Lat >= 16))  LetterDesignator = "Q";
	else if ((16 > Lat) && (Lat >= 8))   LetterDesignator = "P";
	else if (( 8 > Lat) && (Lat >= 0))   LetterDesignator = "N";
	else if (( 0 > Lat) && (Lat >= -8))  LetterDesignator = "M";
	else if ((-8 > Lat) && (Lat >= -16)) LetterDesignator = "L";
	else if((-16 > Lat) && (Lat >= -24)) LetterDesignator = "K";
	else if((-24 > Lat) && (Lat >= -32)) LetterDesignator = "J";
	else if((-32 > Lat) && (Lat >= -40)) LetterDesignator = "H";
	else if((-40 > Lat) && (Lat >= -48)) LetterDesignator = "G";
	else if((-48 > Lat) && (Lat >= -56)) LetterDesignator = "F";
	else if((-56 > Lat) && (Lat >= -64)) LetterDesignator = "E";
	else if((-64 > Lat) && (Lat >= -72)) LetterDesignator = "D";
	else if((-72 > Lat) && (Lat >= -80)) LetterDesignator = "C";
        // "Z" is an error flag, the Latitude is outside the UTM limits
	else LetterDesignator = "Z";
	return LetterDesignator;
}

//_deg2rad = pi / 180.0
//_rad2deg = 180.0 / pi

Real deg2rad(){
	return (Pi()/180.0);
}

Real rad2deg(){
	return (180.0/Pi());
}

#ifndef DEG2RAD
#define DEG2RAD (Pi()/180.0)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0/Pi())
#endif

//"MGA94 Zone 50" "UTM" "+proj=utm +a=6378137.0 +rf=298.257222101 +k=0.9996 +zone=50 +south"
//from formula page
// ID   where                 eq radius , square of eccentricity
//[ 2, "Australian National", 6378160, 0.006694542],


//def LLtoUTM(ReferenceEllipsoid, Lat, Long):
//#converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
//#East Longitudes are positive, West longitudes are negative.
//#North latitudes are positive, South latitudes are negative
//#Lat and Long are in decimal degrees
//#Written by Chuck Gantz- chuck.gantz@globalstar.com

//void LLtoUTM(ReferenceEllipsoid, Lat, Long){
Integer LLtoUTM(Real Lat, Real Long, Real a, Real eccSquared, Real k0, Text &UTMZone, Real &UTMEasting, Real &UTMNorthing){
// these should be passed in
    //a = _ellipsoid[ReferenceEllipsoid][_EquatorialRadius]
    //eccSquared = _ellipsoid[ReferenceEllipsoid][_eccentricitySquared]
    //k0 = 0.9996
	//#Make sure the longitude is between -180.00 .. 179.9
    //LongTemp = (Long+180)-int((Long+180)/360)*360-180 # -180.00 .. 179.9
	Real LongTemp = (Long+180)-Floor((Long+180)/360)*360-180; //# -180.00 .. 179.9

	Real LatRad = Lat*deg2rad();
	Real LongRad = LongTemp*deg2rad();

	//Integer ZoneNumber = int((LongTemp + 180)/6) + 1
	Integer ZoneNumber = Floor((LongTemp + 180)/6) + 1;

	if (Lat >= 56.0 && Lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0){
		ZoneNumber = 32;
	}

    //# Special zones for Svalbard
	if (Lat >= 72.0 && Lat < 84.0){
		if(LongTemp >= 0.0  && LongTemp <  9.0){
			ZoneNumber = 31;
		}else if(LongTemp >= 9.0  && LongTemp < 21.0){
			ZoneNumber = 33;
		}else if(LongTemp >= 21.0 && LongTemp < 33.0){
			ZoneNumber = 35;
		}else if(LongTemp >= 33.0 && LongTemp < 42.0){
			ZoneNumber = 37;
		}
	}

	Real LongOrigin = (ZoneNumber - 1)*6 - 180 + 3; //#+3 puts origin in middle of zone
	Real LongOriginRad = LongOrigin * deg2rad();

    //#compute the UTM Zone from the latitude and longitude
    //UTMZone = "%d%c" % (ZoneNumber, _UTMLetterDesignator(Lat))
	UTMZone = To_text(ZoneNumber)+UTMLetterDesignator(Lat);

	Real eccPrimeSquared = (eccSquared)/(1-eccSquared);
	Real N = a/Sqrt(1-eccSquared*Sin(LatRad)*Sin(LatRad));
	Real T = Tan(LatRad)*Tan(LatRad);
	Real C = eccPrimeSquared*Cos(LatRad)*Cos(LatRad);
	Real A = Cos(LatRad)*(LongRad-LongOriginRad);

	Real M = a*((1
            - eccSquared/4
            - 3*eccSquared*eccSquared/64
            - 5*eccSquared*eccSquared*eccSquared/256)*LatRad
           - (3*eccSquared/8
              + 3*eccSquared*eccSquared/32
              + 45*eccSquared*eccSquared*eccSquared/1024)*Sin(2*LatRad)
           + (15*eccSquared*eccSquared/256 + 45*eccSquared*eccSquared*eccSquared/1024)*Sin(4*LatRad)
           - (35*eccSquared*eccSquared*eccSquared/3072)*Sin(6*LatRad));

    UTMEasting = (k0*N*(A+(1-T+C)*A*A*A/6
                        + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
                  + 500000.0);

    UTMNorthing = (k0*(M+N*Tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                                        + (61
                                           -58*T
                                           +T*T
                                           +600*C
                                           -330*eccPrimeSquared)*A*A*A*A*A*A/720)));

	if(Lat < 0){
        	UTMNorthing = UTMNorthing + 10000000.0; //#10000000 meter offset for southern hemisphere
        }
//    return (UTMZone, UTMEasting, UTMNorthing)
	return 0;
}

#endif