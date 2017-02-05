#include <iostream>
#include <string.h>
#include <vector>

/*
* Change split() to use traditional for loop and substrings.
*/

std::vector<std::string> split(const std::string sentence, const char d)
{
	std::vector<std::string> parsed;
	std::string buffer = "";
	for (auto i:sentence)
	{
		if (i != d) buffer += i;
		else if (i == d)
		{
			parsed.push_back(buffer);
			buffer = "";
		}
	}
	return parsed;
}

int main()
{
	std::string data = "$GPRMC, ,044954.000,A,3706.7967,N,11332.4225,W,0.00,11.76,050217,,,D*4F";
	std::vector<std::string> parsed_data{split(data, ',')};
	for (int i = 0; i <= parsed_data.size(); i++)
	{
		std::cout << parsed_data[i] << ",";
	}
	std::cout << std::endl;
}
