#include <iostream>
#include <cstdio>
#include <sstream>
#include <stdlib.h>
#include <wiringGpioExtensions.h>

#include "../Keyboard.h"
#include "../PwmHelpers.h"

using namespace std;
	
	


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