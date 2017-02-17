#include <iostream>
#include <string>
#include <vector>

struct UTC {
    int rawUTC;
    int hours;
    int minutes;
    int seconds;
};

struct GPGGA { /* Global Positioning System Fix Data */
    UTC     time;
    float   latitude;  // ddmm.mmmm
    char    latDirection;
    float   longitude;  //dddmm.mmmm
    char    longDirection;
    int     fix;
    int     satsInView;
    float   HDOP;
    float   altitude;
    float   geoid;
    float   lastDGPS;
    int     DGPSid;
    int     checksum;
};

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

float formatLat(std::string lat)
{
    float deg = std::stof(lat.substr(0,2));
    float minutes = (std::stof(lat.substr(2,lat.length())) / 60.00);
    return (deg + minutes);
}

float formatLong(std::string longitude)
{
	float deg = std::stof(longitude.substr(0, 3));
	float minutes = (std::stof(longitude.substr(3, longitude.length())) / 60.00);
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
	nmea.longitude = formatLong(data[3]);
    return nmea;
}

void printGGA(GPGGA data)
{
    std::cout << "Raw UTC time: " << data.time.rawUTC << std::endl;
    std::cout << "Hours: " << data.time.hours << std::endl;
    std::cout << "Minutes: " << data.time.minutes << std::endl;
    std::cout << "Seconds: " << data.time.seconds << std::endl;
	std::cout << "Formatted latitude: " << data.latitude << std::endl;
	std::cout << "Formatted longitude: " << data.longitude << std::endl;
}

int main()
{
	std::string data = "$GPGGA,210658.000,0302.78469,N,10141.82531,W,1,10,0.9,844.3,M,-20.5,M,,0000*60";
	std::vector<std::string> parsed_data{split(data, ',')};
	std::cout << "\nTest case: " << data << std::endl << "Result:\n" << "---------------------\n";
	for (auto elem:parsed_data) std::cout << elem << std::endl;

    GPGGA test = assignGGA(parsed_data);
    printGGA(test);
    return 0;
}
