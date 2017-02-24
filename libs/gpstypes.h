#ifndef GPSTYPES_H
#define GPSTYPES_H

struct UTC {
	int rawUTC;
	int hours;
	int minutes;
	int seconds;
};

struct GPGGA { /* Global Positioning System Fix Data */
	UTC     time;
	double  latitude;  // ddmm.mmmm
	char    latDirection;
	double  longitude;  //dddmm.mmmm
	char    longDirection;
	int     fix;
	int     satsInView;
	double  HDOP;
	double  altitude;
	double  geoid;
	double  lastDGPS;
	int     DGPSid;
	int     checksum;
};

#endif GPSTYPES_H