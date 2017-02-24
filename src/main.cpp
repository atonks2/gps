#include <iostream>
#include <string>
#include "gps.h"
#include "gpstypes.h"

int main()
{
	GPS gps(4800, "/dev/ttyUSB0");

	GPGGA data{ gps.updateGPGGA() };
	gps.printGPGGA(data);

	return 0;
}