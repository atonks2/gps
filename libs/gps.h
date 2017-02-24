/*
MIT License

Copyright (c) 2017 Alan Tonks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef GPS_H
#define GPS_H

#include <string>
#include <vector>
#include "serial.h"
#include "gpstypes.h"

class GPS : public Serial {
public:
	GPS() : Serial(4800, "/dev/ttyUSB0") {};
	GPS(speed_t baud, std::string port) : Serial(baud, port) {};

	// Used to calculate checksum of sentence to be sent
	// std::string sentence is a nmea format string without the checksum
	// ex: "$PSRF100,1,4800,8,1,0" (nmea mode, 4800 baud, 8N1)
	int calculateChecksum(std::string sentence);

	// Used to verify checksum of nmea sentence received
	// std::string sentence is full nmea sentence sent by GPS
	// returns 0 if sums match
	int verifyChecksum(std::string sentence);

	// Calculate bearing to destination
	// lat/long A is the starting point, lat/long B is destination
	double bearing(double latA, double longA, double latB, double longB);

	// Determine Great Circle distance to given point
	// lat/long A is the starting point, lat/long B is destination
	double distance(double latA, double longA, double latB, double longB);

	GPGGA updateGPGGA();

	// Prints GPGGA struct in easily readable format
	void printGPGGA(GPGGA data);

private:
	// Reads from device in loop until GPGGA sentence is received
	std::string getGPGGA();

	// Parses a string into a vector based on given delimiter
	std::vector<std::string> splitNmea(const std::string& sentence, const char& delim);

	// Takes in nmea formatted latitude (as a string) and returns decimal latitude
	double formatLat(std::string lat);

	// Takes in nmea formatted longitude (as string) and returns decimal longitude
	double formatLong(std::string longitude);

	// Takes UTC time as string and returns struct containing hours, minutes, and seconds
	UTC formatTime(std::string utc);

	// Takes a nmea sentence parsed into a vector and assigns values to members of struct GPGGA
	GPGGA assignGPGGA(std::vector<std::string> sentence);

	GPGGA currentGPGGA;
};

#endif  // GPS_H