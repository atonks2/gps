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
    nmea.latDirection = data[3][0];
    if (nmea.latDirection == 'S') nmea.latitude *= -1.00;
	nmea.longitude = formatLong(data[4]);
    nmea.longDirection = data[5][0];  // data[i] is a string, data[i][0] is the first element of that string.
    if (nmea.longDirection == 'W') nmea.longitude *= -1.00;
    nmea.fix = std::stoi(data[6]);
    nmea.satsInView = std::stoi(data[7]);
    nmea.HDOP = std::stof(data[8]);
    nmea.altitude = std::stof(data[9]);
    nmea.geoid = std::stof(data[11]);
    if (data[13] == "") nmea.lastDGPS = -1.00;
    else nmea.lastDGPS = std::stof(data[13]);
    nmea.DGPSid = std::stoi(data[14].substr(0,4));
    nmea.checksum = std::stoul(data[14].substr(5,2),nullptr,16);
    return nmea;
}

void printGGA(GPGGA data)
{
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
    std::cout << "Checksum: " << data.checksum << std::endl;
}

int main()
{
	std::string data = "$GPGGA,030059.000,3706.7966,N,11332.4206,W,2,11,0.8,842.5,M,-20.5,M,2.0,0000*49";
	std::vector<std::string> parsed_data{split(data, ',')};
	std::cout << "\nTest case: " << data << std::endl << "Result:\n" << "---------------------\n";
    GPGGA test = assignGGA(parsed_data);
    printGGA(test);
    return 0;
}
