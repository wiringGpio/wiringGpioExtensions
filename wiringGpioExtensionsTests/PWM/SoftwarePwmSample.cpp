#include <iostream>
#include <stdio.h>
#include <string>

#include <wiringGpioExtensions.h>

#include "SoftwarePwmSample.h"
#include "../Keyboard.h"

using namespace std;


int PromptSoftwareSetupPwm(int& pinNumber, int& range)
{
	bool valid = false;
	string input;

	while (!valid)
	{
		cout << "Enter Pin Number, q to quit" << endl;
	
		getline(cin, input);

		if (input.compare("q") == 0)
			return false;

		pinNumber = strtol(input.c_str(), NULL, 10);

		if (pinNumber != 0)
			valid = true;
		else
			cout << "Invalid pin number" << endl;
	}

	valid = false;

	while (!valid)
	{
		cout << "Enter Range, q to quit" << endl;

		getline(cin, input);

		if (input.compare("q") == 0)
			return false;

		range = strtol(input.c_str(), NULL, 10);

		if (range <= 200 && range >= 0)
			valid = true;
		else
			cout << "Invalid range" << endl;
	}

	return true;
}


void PrintSoftwarePwmInstructions()
{
	cout << endl;
	cout << "Pulse Width Modulation Control" << endl;
	cout << "W to increase PWM by 10%" << endl;
	cout << "S to decrease PWM by 10%" << endl;
	cout << "E to increase PWM by 1" << endl;
	cout << "D to decrease PWM by 1" << endl;
	cout << "R to change pin" << endl;
	cout << "X to input a value in range" << endl;
	cout << "Q to quit" << endl;
	cout << endl;
	
}



//  Software PWM sample function
//  Shows how to setup and control software pulse width modulation on any pin
//
int SoftwarePwmSampleMain(int argc, char *argv[])
{
	
	int range = 0;
	int powerLevel = 0;
	int pinNumber = 0;

	if (!PromptSoftwareSetupPwm(pinNumber, range))
		return 0;
	
	SoftPwmCreate(pinNumber, 0, range);
	cout << "Software PWM: pin " << pinNumber << " range " << range << " power level " << powerLevel << endl;

	bool repeat = true;
	while (repeat)
	{
		system("stty raw");	//  characters will be read as soon as they are input

		char key =  ' ';
		key = getchar();
		key = toupper(key);
		cout << '\b';		//  backspace the cursor
		
		//  handle key
		if (key == 'Q')
		{
			//  quit
			SoftPwmStop(pinNumber);
			system("stty cooked");	// reset terminal to normal "cooked" mode 
			return 0;
		}
		else if (key == 'W')
		{
			//  increase value 10%
			powerLevel += (int)(range / 10);
		}
		else if (key == 'S')
		{
			//  decrease value 10%
			powerLevel -= (int)(range / 10);
		}
		else if (key == 'E')
		{
			//  increase value by 1
			powerLevel += 1;
		}
		else if (key == 'D')
		{
			//  decrease value by 1
			powerLevel -= 1;
		}
		else if (key == 'X')
		{	
			//  enter specific PWM value
			system("stty cooked");	
			powerLevel = GetIntegerFromKeyboard("Enter PWM value");
		}
		else if (key == 'R')
		{
			system("stty cooked");	
			int previousPin = pinNumber;
			if (!PromptSoftwareSetupPwm(pinNumber, range))
			{
				SoftPwmStop(previousPin);
				return 0;
			}
			SoftPwmStop(previousPin);
			PinMode(pinNumber, PINMODE_OUTPUT);
			SoftPwmCreate(pinNumber, 0, range);
		}
		
		else
		{
			system("stty cooked");  
			PrintSoftwarePwmInstructions();
			continue;
		}
		
		//  adjust for valid range
		if (powerLevel <= 0)
			powerLevel  = 0;
		if (powerLevel > range)
			powerLevel = range;	

		system("stty cooked");	
		cout << "Software PWM: pin " << pinNumber << " range " << range << " power level " << powerLevel << endl;
		SoftPwmWrite(pinNumber, powerLevel);
	}


	return 0;
}