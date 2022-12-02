#include <map>

#include "wiringGpioExtensions.h"
#include "SevenSegDisplayDriver.h"


using namespace std;

//  Map of seven seg display index to seven seg display object
map<int, SevenSegDisplayDriver*> SevenSegDisplayMap;


SevenSegDisplayDriver* SevenSetDisplayFind(int index)
{
	map<int, SevenSegDisplayDriver*>::iterator it = SevenSegDisplayMap.find(index);
	if (it != SevenSegDisplayMap.end())
	{
		return it->second;
	}
	return NULL;
}


void SevenSegDisplayShutDown()
{
	//  shut down the seven segment display drivers
	map<int, SevenSegDisplayDriver*>::iterator itSegDisplays;
	for (itSegDisplays = SevenSegDisplayMap.begin(); itSegDisplays != SevenSegDisplayMap.end(); ++itSegDisplays)
	{
		itSegDisplays->second->Cancel();
		delete itSegDisplays->second;
	}
	SevenSegDisplayMap.clear();
}


//  Seven Segment Delay
//
#pragma region SevenSegDisplay

int SevenSegDisplayCreateFromXml(const char* xml)
{
	//  get the next available index number
	int index = 1;
	while (true)
	{
		map<int, SevenSegDisplayDriver*>::iterator it = SevenSegDisplayMap.find(index);
		if (it == SevenSegDisplayMap.end())
		{
			break;
		}
		index++;
	}
	
	SevenSegDisplayDriver* newDriver = new SevenSegDisplayDriver();
	
	if (!newDriver->LoadFromXml(xml))
	{
		delete newDriver;
		return -1;
	}
	
	SevenSegDisplayMap[index] = newDriver;
	newDriver->Start();
	
	return index;
}


int SevenSegDisplayCreate(int segPins[8], int numDigits, int digitPins[])
{
	//  get the next available index number
	int index = 0;
	while (true)
	{
		map<int, SevenSegDisplayDriver*>::iterator it = SevenSegDisplayMap.find(index);
		if (it == SevenSegDisplayMap.end())
		{
			break;
		}
		index++;
	}
	
	//  create the driver
	SevenSegDisplayDriver* newDriver = new SevenSegDisplayDriver();
	SevenSegDisplayMap[index] = newDriver;
	
	//  setup and start the driver the driver
	newDriver->SetSegmentPins(segPins);
	newDriver->SetDigitPins(numDigits, digitPins);
	newDriver->Start();
	
	return index;
}


void SevenSegDisplayRemove(int index)
{
	SevenSegDisplayDriver* findDriver = SevenSetDisplayFind(index);
	if (findDriver != NULL)
	{
		findDriver->Cancel();
		SevenSegDisplayMap.erase(index);
		delete findDriver;
	}
}


void SevenSegDisplayOff(int index)
{
	SevenSegDisplayDriver* findDriver = SevenSetDisplayFind(index);
	if (findDriver != NULL)
		findDriver->TurnOff();
}
	

void SevenSegDisplaySetDelay(int index, int delay)
{
	SevenSegDisplayDriver* findDriver = SevenSetDisplayFind(index);
	if (findDriver != NULL)
		findDriver->SetDelay(delay);
}


void SevenSegDisplaySet(int index, const char* display)
{
	SevenSegDisplayDriver* findDriver = SevenSetDisplayFind(index);
	if (findDriver != NULL)
		findDriver->SetDisplay(display);
}

#pragma endregion