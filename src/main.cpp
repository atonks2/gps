#include <iostream>
#include <string>
#include "gps.h"
#include "gpstypes.h"

int main()
{
	GPS gps(4800, "/dev/ttyUSB0");

	std::cout << "Reading from GPS...";
	GPGGA data = gps.updateGPGGA();
	std::cout << "done.\n";
	gps.printGPGGA(data);

	// lat long of known location (the Whitehouse)
	double latitude = 38.8977;
	double longitude = -77.0365;

	std::cout << "Distance between current location and the White House: "
		<< gps.distance(data.latitude, data.longitude, latitude, longitude) << std::endl;
	std::cout << "Bearing from current location to the White House: "
		<< gps.bearing(data.latitude, data.longitude, latitude, longitude) << std::endl;

	return 0;
}