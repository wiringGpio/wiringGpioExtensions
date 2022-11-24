#include <map>

#include "wiringGpioExtensions.h"
#include "RotaryEncoder.h"

using namespace std;

//  Map of rotary encoder index to rotary encoder object
map<int, RotaryEncoder*> RotaryEncoderMap;


RotaryEncoder* FindRotaryEncoder(int index)
{
	map<int, RotaryEncoder*>::iterator it = RotaryEncoderMap.find(index);
	if (it != RotaryEncoderMap.end())
	{
		return it->second;
	}
	return NULL;
}


void ShutDownEncoders()
{
	//  delete the rotary encoders
	map<int, RotaryEncoder*>::iterator itEncoders;
	for (itEncoders = RotaryEncoderMap.begin(); itEncoders != RotaryEncoderMap.end(); ++itEncoders)
	{
		delete itEncoders->second;
	}
	RotaryEncoderMap.clear();
}



//  Rotary Encoder
//
#pragma region RotaryEncoder

int RotaryEncoderCreate(const int pinA, const int pinB, const int pinIndex, const int countsPerRevolution, EncoderUpdatedCallback callback)
{
	//  get the next available index number
	int index = 0;
	while (true)
	{
		map<int, RotaryEncoder*>::iterator it = RotaryEncoderMap.find(index);
		if (it == RotaryEncoderMap.end())
		{
			break;
		}
		index++;
	}
	
	//  maximum 8 encoders
	if(index > 7)
		return - 1;

	//  make a new one
	RotaryEncoder* newEncoder = new RotaryEncoder(pinA, pinB, pinIndex, countsPerRevolution, callback);
	SetupEncoderWiringPiCallbacks(index, newEncoder);
	//  add the driver to our map
	RotaryEncoderMap[index] = newEncoder;
		
	return index;
}


void RotaryEncoderRemove(int index)
{
	RotaryEncoder* findEncoder = FindRotaryEncoder(index);
	if (findEncoder != NULL)
	{
		RotaryEncoderMap.erase(index);
		delete findEncoder;
	}
}
	

int RotaryEncoderGetCount(int index)
{
	RotaryEncoder* findEncoder = FindRotaryEncoder(index);
	if (findEncoder != NULL)
		return findEncoder->GetCount();
	
	return 0;
}


double RotaryEncoderGetRpm(int index)
{
	RotaryEncoder* findEncoder = FindRotaryEncoder(index);
	if (findEncoder != NULL)
		return findEncoder->GetRpm();
	
	return 0.0;
}


double RotaryEncoderGetFrequency(int index)
{
	RotaryEncoder* findEncoder = FindRotaryEncoder(index);
	if (findEncoder != NULL)
		return findEncoder->GetCountFrequency();
	
	return 0.0;
}


void RotaryEncoderResetCount(int index, int setCount)
{
	RotaryEncoder* findEncoder = FindRotaryEncoder(index);
	if (findEncoder != NULL)
		findEncoder->ResetCounter(setCount);
	
}

#pragma endregion
