#include <iostream>
#include <iomanip>
#include "main.h"
#include "WiringPiExtensionApi.h"
#include "PinOnOffSample.h"

using namespace std;


void PrintPinState()
{
	cout << endl;
	
	//  go through all the pins in four rows
	for (int i = 0; i < 4; i++)
	{	
		int pinBase = 10*i;
			
		for (int i = pinBase + 1; i <= pinBase + 10; i++)
		{
			cout << setfill(' ') << setw(3) << i << " ";
		}
		cout << endl;
	
		for (int i = pinBase + 1; i <= pinBase + 10; i++)
		{
			int read = DigitalRead(i);
			//  get the state
			
			string state = (read ? " ON " : "OFF ");
			cout << state;
		}
		cout << endl;	
	}
}


//  Pin on / off sample function
//  Shows how to control a pin output one one of the Raspberry Pi's pins
//
int PinOnOffSampleMain(int argc, char* argv[])
{
	while (true)
	{
		cout << endl;
		PrintPinState();
		
		int pin = GetIntegerFromKeyboard("Enter Pin Number (-1 to exit)");
		
		if (pin == -1)
		{
			return 0;
		}
		
		PinMode(pin, PINMODE_OUTPUT);
		
		//  Read the state
		int read = DigitalRead(pin);
			
		//  write out
		DigitalWrite(pin, read == 0 ? 1 : 0);
	}
}


