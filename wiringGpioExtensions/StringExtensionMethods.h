#pragma once

#include <string>
#include <cstdarg>
#include <vector>
#include <memory>
#include <functional>
#include <sstream>





//  Parse comma delimited string into vector of ints
//
inline bool ParseIntVector(std::string input, std::vector<int>& result, char delimiter)
{
	result.clear();
	
	std::istringstream ss(input);
	std::string nextItem;
	while (getline(ss, nextItem, delimiter))
	{
		int number;
		std::stringstream convertor;
	
		convertor << nextItem;
		convertor >> number;

		if (convertor.fail())
		{
			return false;
		}
		result.push_back(number);
	}
	return true;
}


//	std::string format()
//  replaces missing string printf   (this is safe and convenient but not exactly efficient )
//  can be used like:  std::string mystr = format("%s %d %10.5f", "omg", 1, 10.5);
//  see http://stackoverflow.com/users/642882/piti-ongmongkolkul
inline std::string format(const char* fmt, ...) {
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];
	va_list vl;
	va_start(vl, fmt);
	int nsize = vsnprintf(buffer, size, fmt, vl);
	if (size <= nsize) {
		//fail delete buffer and try again
	   delete[] buffer;
		buffer = 0;
		buffer = new char[nsize + 1];   //+1 for /0
		nsize = vsnprintf(buffer, size, fmt, vl);
	}
	std::string ret(buffer);
	va_end(vl);
	delete[] buffer;
	return ret;
}