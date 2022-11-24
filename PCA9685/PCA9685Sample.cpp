#include <iostream>
#include <cstdio>
#include <sstream>
#include <stdlib.h>
#include "WiringPiExtensionApi.h"
#include "main.h"

using namespace std;
	
	
void PrintPcaState()
{
	int pinBase = 0;
	int fd = 0;
	int numPins = 16;
	
	cout << endl;
	
	map<int, int>::iterator it;
	for (it = PcaMap.begin(); it != PcaMap.end(); it++)
	{
		pinBase = it->first;
		fd = it->second;
		
		for (int i = pinBase; i < pinBase + numPins; i++)
		{
			cout << i << " ";
		}
		cout << endl;
	
		for (int i = pinBase; i < pinBase + numPins; i++)
		{
				
			int read = DigitalRead(i);
			//  get the state
			int fullOffState = (read & 0x1000);
			
			string state = (fullOffState == 4096 ? "OFF " : " ON ");
			cout << state;
		}
		cout << endl;
	}
}


//  PCA pin on / off sample function
//  Shows how to turn PCA pin on and off, as well as read the values of PCA pin
//  The PCA pin values are a bit cryptic, so you must do some math to get their 'binary' state
//
int PCA9685SampleMain(int argc, char* argv[])
{
	while (true)
	{
		PrintPcaState();
		
		int pin = GetIntegerFromKeyboard("Enter Pin Number (enter 0 for all off, -1 to quit)");
		
		if (pin == 0)
		{
			PcaAllOff();
			continue;
		}
		else if (pin == -1)
		{
			return 0;
		}
		
		//  Read the state
		int read = DigitalRead(pin);
		//  get the state
		int pwmState = (read & 0xFFF);
		//  Get the full off state
		int fullOffState = (read & 0x1000);
		
		//  write out
		DigitalWrite(pin, fullOffState == 4096 ? 1 : 0);
	}
}