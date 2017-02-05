#include <iostream>
#include <string.h>
#include <vector>

std::vector<std::string> split(const std::string sentence, const char delim)
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

/* Simple test cases */
int main()
{
	std::string data = "$GPGGA,210658.000,3706.7984,N,11332.4191,W,1,10,0.9,844.3,M,-20.5,M,,0000*60";
	std::vector<std::string> parsed_data{split(data, ',')};
	std::cout << "\nTest case: " << data << std::endl << "Result:\n" << "---------------------\n";
	for (auto elem:parsed_data) std::cout << elem << std::endl;
}
