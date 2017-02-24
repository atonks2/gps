#include <iostream>
#include <string>
#include "gps.h"

int main()
{
	GPS(4800, "/dev/ttyUSB0");

	gps.updateGPGGA();
	gps.printGPGGA();

	return 0;
}