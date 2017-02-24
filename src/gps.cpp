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

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "serial.h"
#include "gps.h"

// useful calculations: http://www.movable-type.co.uk/scripts/latlong.html

std::string getGPGGA()
{
    std::string data;
    do {
        gps.serialRead();
        data = gps.getData();
    } while (data.substr(0,6) != "$GPGGA");
    return data;
}

std::vector<std::string> split(const std::string& sentence, const char& delim) /* passing by reference to avoid unnecessary copy of variables */
{
	std::vector<std::string> parsed; /* Vector to hold parsed data */
	std::string buffer = ""; /* A buffer to store each chunck before pushing into the vector */
	for (auto i:sentence)
	{
		if (i != delim) buffer += i; /* if not delimiter, add char to buffer */
		else if (i == delim) /* delimiter found. Add it to the vector */
		{
			parsed.push_back(buffer);
			buffer = ""; /* Reset the buffer */
		}
	}
	if (buffer != "") parsed.push_back(buffer); /* Add last piece of data to vector (NMEA sentences don't end with a comma) */
	return parsed;
}

int checksum(std::string sentence)
{
    int sum = 0;
    sentence = sentence.substr(1,(sentence.length()-4));
    for (auto i:sentence)
    {
        sum ^= i;
    }
    return sum;
}

double formatLat(std::string lat)
{
    double deg = std::stof(lat.substr(0,2));
    double minutes = (std::stof(lat.substr(2,lat.length())) / 60.00);
    return (deg + minutes);
}

double formatLong(std::string longitude)
{
	double deg = std::stof(longitude.substr(0, 3));
	double minutes = (std::stof(longitude.substr(3, longitude.length())) / 60.00);
	return (deg + minutes);
}

UTC formatTime(std::string utc)
{
	UTC time;
	time.rawUTC = std::stoi(utc);
	time.hours = std::stoi(utc.substr(0, 2));
	time.minutes = std::stoi(utc.substr(2, 2));
	time.seconds = std::stoi(utc.substr(4, 2));
	return time;
}

GPGGA assignGGA(const std::vector<std::string> data)
{
    GPGGA nmea;
    nmea.time = formatTime(data[1]);
    nmea.latitude = formatLat(data[2]);
    nmea.latDirection = data[3][0];
    if (nmea.latDirection == 'S') nmea.latitude *= -1.00;
    nmea.longitude = formatLong(data[4]);
    nmea.longDirection = data[5][0];  // data[i] is a string, data[i][0] is the first element of that string.
    if (nmea.longDirection == 'W') nmea.longitude *= -1.00;
    nmea.fix = std::stoi(data[6]);
    nmea.satsInView = std::stoi(data[7]);
    nmea.HDOP = std::stod(data[8]);
    nmea.altitude = std::stod(data[9]);
    nmea.geoid = std::stod(data[11]);
    if (data[13] == "") nmea.lastDGPS = -1.00;
    else nmea.lastDGPS = std::stod(data[13]);
    nmea.DGPSid = std::stoi(data[14].substr(0,4));
    nmea.checksum = std::stoul(data[14].substr(5,2),nullptr,16);
    return nmea;
}

double distance(double lat_a, double long_a, double lat_b, double long_b)
{
	const double radius = 6378.137;  // Radius at equator
	const double to_radians = (3.1415926536 / 180.00);
	lat_a *= to_radians;
	lat_b *= to_radians;
	long_a *= to_radians;
	long_b *= to_radians;
	
	double d_lat = lat_b - lat_a;
	double d_long = long_b - long_a;
	
	double a = pow(sin(d_lat/2),2) + pow(sin(d_long/2),2) * cos(lat_a) * cos(lat_b);
	double c = 2 * atan2(sqrt(a),sqrt(1-a));
	
	return (radius * c);
}

double bearing(double lat_a, double long_a, double lat_b, double long_b)
{
    const double to_radians = (3.1415926536 / 180.00);
    const double to_degrees = (180.00 / 3.1415926536);

    lat_a *= to_radians;
    lat_b *= to_radians;
    long_a *= to_radians;
    long_b *= to_radians;

    double d_lat = lat_b - lat_a;
    double d_long = long_b - long_a;

    double y = sin(d_long) * cos(lat_b);
    double x = (cos(lat_a) * sin(lat_b)) - (sin(lat_a) * cos(lat_b) * cos(d_long));

    return ((atan2(y, x) * to_degrees) + 360.00);
}

void printGGA(GPGGA data)
{
    std::cout << "\nResult:\n" << "---------------------\n";
    std::cout << "Raw UTC time: " << data.time.rawUTC << std::endl;
    std::cout << "Hours: " << data.time.hours << std::endl;
    std::cout << "Minutes: " << data.time.minutes << std::endl;
    std::cout << "Seconds: " << data.time.seconds << std::endl;
    std::cout << "Latitude: " << data.latitude << std::endl;
    std::cout << "Direction: " << data.latDirection << std::endl;
    std::cout << "Longitude: " << data.longitude << std::endl;
    std::cout << "Direction: " << data.longDirection << std::endl;
    std::cout << "Fix type: " << data.fix << std::endl;
    std::cout << "Sats In View: " << data.satsInView << std::endl;
    std::cout << "Horizontal DOP: " << data.HDOP << std::endl;
    std::cout << "Altitude: " << data.altitude << std::endl;
    std::cout << "Geoid: " << data.geoid << std::endl;
    std::cout << "Last DGPS: " << data.lastDGPS << std::endl;
    std::cout << "DGPS Station: " << data.DGPSid << std::endl;
    std::cout << "Checksum: " << std::hex << data.checksum << std::dec << std::endl;
}