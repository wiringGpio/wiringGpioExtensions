#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <wiringGpioExtensions.h>
#include "HBridgeSample.h"

using namespace std;

using namespace std;

int pwmPin = 12;
int in1Pin = 11;
int in2Pin = 13;

void shutDown()
{
	
//	digitalWrite(in1Pin, 0);
//	digitalWrite(in2Pin, 0);
}

void RunForward()
{
	DigitalWrite(in1Pin, 1);
	DigitalWrite(in2Pin, 0);
	cout << "Run forward" << endl;
}

void RunReverse()
{
	DigitalWrite(in1Pin, 0);
	DigitalWrite(in2Pin, 1);
	cout << "Run reverse" << endl;
}

int HBridgeSampleMain(int argc, char *argv[])
{
	//  This example is hard coded to use Pi's PWM pin (12) and pins 13, 15 for HBridge polarity pins
	int range = 1024;
	int powerLevel = 0;

	PinMode(pwmPin, PWM_OUTPUT);
	PinMode(in1Pin, OUTPUT);
	PinMode(in2Pin, OUTPUT);
	
	//  set the initial direction
	RunForward();
	
	bool repeat = true;
	while (repeat)
	{
		// Set terminal to raw mode 
		system("stty raw");

		char key =  ' ';
		key = getchar();
		cout << '\b';		//  backspace the cursor
		
		if (key == 'q')
		{
			shutDown();
			system("stty cooked");
			return 0;
		}
		else if (key == 'e')
		{
			powerLevel += (int)(range / 100);
			if (powerLevel > range)
				powerLevel = range;
		}
		else if (key == 'd')
		{
			powerLevel -= (int)(range / 100);
			if (powerLevel < 0)
				powerLevel = 0;
			
		}
		else if (key == 'w')
		{
			powerLevel += 1;
			if (powerLevel > range)
				powerLevel = range;
		}
		else if (key == 's')
		{
			powerLevel -= 1;
			if (powerLevel < 0)
				powerLevel = 0;
			
		}
		else if (key == 'x')
		{
			powerLevel = 0;
		}
		else if (key == 'c')
		{
			powerLevel = range;
		}
		else if (key == 'r')
		{
			RunForward();
		}
		else if (key == 'f')
		{
			RunReverse();
		}

		// Reset terminal to normal "cooked" mode 
		system("stty cooked");
		
		cout << endl;

		cout << "HBridge Software PWM: pin " << pwmPin << " range " << range << " power level " << powerLevel << endl;
		PwmWrite(pwmPin, powerLevel);
	}


	return 0;
}
