#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include "WiringPiExtensionApi.h"
#include "main.h"

using namespace std;
	
	

void PrintPcaPins()
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
	
		
	}
}


void PrintPcaServoInstructions()
{
	cout << endl;
	cout << "Software PWM" << endl;
	cout << "W to increase PWM" << endl;
	cout << "S to decrease PWM" << endl;
	cout << "E to increase multiple" << endl;
	cout << "D to decrease multiple" << endl;
	cout << "R to turn current PWM pin off" << endl;
	cout << "Q to quit" << endl;
	cout << endl;
	
}


//  PCA servo control sample function
//  Shows how to control Pulse Width Modulation on a PCA chip pin
//  This includes how to interpert the pin's value as current range and pin state
//
int PCA9685ServoSampleMain(int argc, char* argv[])
{
	float millis = 1.5;
	int centerTick = calcTicks(millis, PWM_FREQUENCY);

	int pwmValue = 0;
	int pwmMultiple = 1;

	PrintPcaServoInstructions();
	
	while (true)
	{
		PrintPcaPins();
		
		int pin = GetIntegerFromKeyboard("Enter pin number to control (enter -1 to exit)");
		
		if (pin == -1)
		{
			//  exit
			return 0;
		}
		
		//  set this pin to centre
		PwmWrite(pin, centerTick);
		
		//  loop until break
		while (true)
		{
			// Set terminal to raw mode to process one character at a time
			system("stty raw");

			//  make a string out of the key input
			string input = string(1, getchar());
			cout << '\b';		//  backspace the cursor
			std::transform(input.begin(), input.end(), input.begin(), ::toupper); 
		
	
			if (input.compare("Q") == 0)
			{
				//  Q for quit
				system("stty cooked");
				break;
			}
			else if (input.compare("R") == 0)
			{
				//  E to Turn current pin off
				pwmValue = 0;
				DigitalWrite(pin, pwmValue);
				PwmWrite(pin, 0);
			}
			else if (input.compare("W") == 0)
			{
				//  R to increase pwm value
				
				//  restart at 0?
				if (pwmValue == 0)
					pwmValue = centerTick;
				//  increase by multiple
				pwmValue += pwmMultiple;
				//  don't go over max
				if (pwmValue > MAX_PWM)
					pwmValue = MAX_PWM;
				//  write the value
				PwmWrite(pin, pwmValue);
			}
			else if (input.compare("S") == 0)
			{
				//  T to decrease PWM value
				
				//  restart at 0
				if (pwmValue == 0)
					pwmValue = centerTick;
				pwmValue -= pwmMultiple;
				if (pwmValue < 0)
					pwmValue = 0;
				PwmWrite(pin, pwmValue);
			}
			else if (input.compare("E") == 0)
			{
				//  E to increase multiple
				
				pwmMultiple *= 10;
				if (pwmMultiple > 100)
					pwmMultiple = 100;
			}
			else if (input.compare("D") == 0)
			{
				//  D to decrease multiple
				
				pwmMultiple /= 10;
				if (pwmMultiple <= 0)
					pwmMultiple = 1;	
			}
			else 
			{
				system("stty cooked");
				PrintPcaServoInstructions();
				continue;
			}
			
			// Reset terminal to normal "cooked" mode 
			system("stty cooked");
			int read = DigitalRead(pin);
			int pwm = (read & 0xFFF);
			int fullOff = (read & 0x1000);
			cout << "PWM Value: " << pwmValue << " Read: " << read << " pwm: " << pwm << " fullOff: " << fullOff << " Frequency: " << PWM_FREQUENCY << " Interval: " << pwmMultiple << endl;
		}
	}
}