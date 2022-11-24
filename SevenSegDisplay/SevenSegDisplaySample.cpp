#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "main.h"
#include "wiringGpioExtensions.h"

using namespace std;



void LogFormatted(wiringGpioLogLevel level, const char* sender, const char* function, const char* format, ...);

//  Seven segment display sample function
//  Shows example how to create and control seven segment display driver using two different methods
//    -  wiring defined in XML element
//    -  wiring defined in array arguments
//
int SevenSegDisplaySampleMain(int argc, char *argv[])
{
	vector<int> sevenSegDrivers;
	
	//  display 1 on MCP chip at pin base 700
	string 	segDisplay1 = 
		"<SevenSegDisplay> \
			<SegPins>713,715,711,709,708,714,712,710,</SegPins>\
			<Digits>-700,-701,-702</Digits>\
		</SevenSegDisplay>";
	
	int display = SevenSegDisplayCreateFromXml(segDisplay1.c_str());
	if (display >= 0)
	{
		sevenSegDrivers.push_back(display);
		LogFormatted(LogLevelInfo, "SevenSegDisplaySamnple.cpp", "SevenSegDisplaySampleMain", "Created display %d from XML %s", display, segDisplay1.c_str());
	}
	else
	{
		LogFormatted(LogLevelError, "SevenSegDisplaySamnple.cpp", "SevenSegDisplaySampleMain", "Failed to create display %s", segDisplay1.c_str());
	}

#ifdef DISPLAY2
	//  display 2 on local pins
	string 	segDisplay2 = 
		"<SevenSegDisplay> \
			<SegPins>33,29,36,38,40,31,35,37,</SegPins>\
			<Digits>-11,-13,-15</Digits>\
		</SevenSegDisplay>";
	
	//  create this one with segments and digit array
	int segments[8] = { 33, 29, 36, 38, 40, 31, 35, 37 };
	int digits[3] = { -11, -13, -15 };
	
	display = SevenSegDisplayCreate(segments, 3, digits);
	if (display >= 0)
	{
		sevenSegDrivers.push_back(display);
		LogFormatted(LogLevelInfo, "SevenSegDisplaySamnple.cpp", "SevenSegDisplaySampleMain", "Created display %d from segment/digit arrays", display);
	}
	else
	{
		LogFormatted(LogLevelError, "SevenSegDisplaySamnple.cpp", "SevenSegDisplaySampleMain", "Failed to create display from int arrays. %d", display);
	}
#endif
	
	int index = sevenSegDrivers[sevenSegDrivers.size() - 1];
		
	//  instructions
	cout << "Seven segment display sample: " << endl;
	cout << "M - set index of controller" << endl;
	cout << "D - set delay in milliseconds " << endl;
	cout << "S - set display to off " << endl;
	cout << "Q - quit " << endl;
	cout << endl;
	
	
	string input;
	while (true)
	{
		cout << "Enter number to display: ";
		getline(cin, input);
		std::transform(input.begin(), input.end(), input.begin(),::toupper); 
		
		if (input.compare("Q") == 0)
		{
			break;
		}
		else if (input.compare("D") == 0)
		{
			int delay = GetIntegerFromKeyboard("Input delay in microseconds");
			SevenSegDisplaySetDelay(index, delay);
		}
		else if (input.compare("S") == 0)
		{
			SevenSegDisplayOff(index);
		}
		else if (input.compare("M") == 0)
		{
			bool foundIndex = false;
			while (!foundIndex)
			{
				int newIndex = GetIntegerFromKeyboard("Enter index of display to control");
				for (int i = 0; i < sevenSegDrivers.size(); i++)
				{
					if (sevenSegDrivers[i] == newIndex)
					{
						foundIndex = true;
						index = newIndex;
						break;
					}
				}
			}
		}
		else
		{
			SevenSegDisplaySet(index, input.c_str());
		}
		
	}
	
	for (int i = 0; i < sevenSegDrivers.size(); i++)
		SevenSegDisplayRemove(sevenSegDrivers[i]);
	
	return 0;
}