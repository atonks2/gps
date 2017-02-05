#include <iostream>
#include <string.h>
#include <vector>

/* Function to parse a string based on a provided delimiter and return result as a vector of strings.
*	Credit for this code goes to bayusetiaji. The code here is just a slight modification of this code:
*	http://www.cplusplus.com/articles/2wA0RXSz/ 
*/

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

/* Simple test cases */
int main()
{
	std::string data = "$GPGGA,210658.000,XXXX.XXXX,N,XXXXX.XXXX,W,1,10,0.9,844.3,M,-20.5,M,,0000*60";
	std::vector<std::string> parsed_data{split(data, ',')};
	std::cout << "\nTest case: " << data << std::endl << "Result:\n" << "---------------------\n";
	for (auto elem:parsed_data) std::cout << elem << std::endl;
}
