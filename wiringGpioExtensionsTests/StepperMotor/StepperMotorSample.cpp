#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <wiringGpioExtensions.h>

#include "../Logging.h"
#include "../Keyboard.h"

using namespace std;

void PrintStepperInstructions()
{
	cout << "Enter number of steps to turn, or:" << endl;
	cout << "m - enter the index of the motor to control" << endl;
	cout << "d - set delay" << endl;
	cout << "f - set interval" << endl;
	cout << "i - spin forward" << endl;
	cout << "k - spin reverse" << endl;
	cout << "o - increase speed by 10" << endl;
	cout << "l - decrease speed by 10" << endl;
	cout << "p - stop" << endl;
	cout << "q - quit program" << endl;
}


//  Stepper motor sample main function
//  Shows example how to create and control two stepper motor drivers
//    -  bipolar stepper motor (connected to dual HBridge)
//    -  unipolar stepper motor
//
int StepperMotorSampleMain(int argc, char *argv[])
{
	//  To use the stepper motor, you must create a stepper motor driver object
	//  define the stepper sequence, 4 or 8 steps per revolution
	//  define the 4 stepper pins, use -ve number for polarity in bipolar stepper
	//  define the polarity pins if using bipolar stepper
	
	//  define the stepper sequence
	string sequenceElement;
	
	//  full step sequence
	/*
	sequenceElement = "\
	<StepperSequence> \
		<SequenceItems> \
			<Item>1,1,0,0</Item> \
			<Item>0,1,1,0</Item> \
			<Item>0,0,1,1</Item> \
			<Item>1,0,0,1</Item> \
		</SequenceItems> \
	</StepperSequence>";
	*/
	
	//  half step sequence
	/*
	sequenceElement = "\
	<StepperSequence> \
		<SequenceItems> \
			<Item>1,0,0,0</Item> \
			<Item>1,1,0,0</Item> \
			<Item>0,1,0,0</Item> \
			<Item>0,1,1,0</Item> \
			<Item>0,0,1,0</Item> \
			<Item>0,0,1,1</Item> \
			<Item>0,0,0,1</Item> \
			<Item>1,0,0,1</Item> \
		</SequenceItems> \
	</StepperSequence>";
	*/
	
	//  single step sequence
	//  only one that works for for bipolar stepper ? http://www.8051projects.net/wiki/Stepper_Motor_Tutorial
	sequenceElement = "\
	<StepperSequence> \
		<SequenceItems> \
			<Item>1,0,0,0</Item> \
			<Item>0,1,0,0</Item> \
			<Item>0,0,1,0</Item> \
			<Item>0,0,0,1</Item> \
			</SequenceItems> \
			</StepperSequence>";

	vector<int> motors;
	
	//  create a unipolar stepper motor on four pins
	string pinsElement = "\
		<StepperDriver> \
			<Pins>401,402,403,404</Pins> \
			</StepperDriver>";
	
	int index = StepperCreateFromXml(sequenceElement.c_str(), pinsElement.c_str());
	if (index >= 0)
	{
		motors.push_back(index);
		AddLogFormatted(LogLevelInfo, "StepperMotorSample.cpp", "StepperMotorSampleMain", "Created motor %d. %s", index, pinsElement.c_str());
	}
	else
	{
		AddLogFormatted(LogLevelError, "StepperMotorSample.cpp", "StepperMotorSampleMain", "Failed to create stepper motor %s", pinsElement.c_str());
	}
	
	//  create a bipolar motor on dual HBridge
	pinsElement = "\
		<StepperDriver> \
			<Pins>608,-613,-608, 613</Pins> \
			<MinDelay>5</MinDelay> \
			<MaxDelay>50</MaxDelay> \
			<Polarity>608,610,609</Polarity> \
			<Polarity>613,611,612</Polarity> \
			<PwmScale>.9</PwmScale> \
			</StepperDriver>";
	
	index = StepperCreateFromXml(sequenceElement.c_str(), pinsElement.c_str());
	if (index >= 0)
	{
		motors.push_back(index);
		AddLogFormatted(LogLevelInfo, "StepperMotorSample.cpp", "StepperMotorSampleMain", "Created motor %d. %s", index, pinsElement.c_str());
	}
	else
	{
		AddLogFormatted(LogLevelError, "StepperMotorSample.cpp", "StepperMotorSampleMain", "Failed to create stepper motor %s", pinsElement.c_str());
	}
	
		
	//  print the instructions to the console
	PrintStepperInstructions();
	
	
	
	double currentSpeed = 0;	
	while (true)
	{
		cout << "Command: ";

		string input;
		getline(cin, input);
		std::transform(input.begin(), input.end(), input.begin(),::toupper); 
		
		if (input.compare("") == 0 || input.compare("H") == 0)
		{
			PrintStepperInstructions();
		}
		else if (input.compare("Q") == 0)
		{
			break;
		}
		else if (input.compare("D") == 0)
		{
			//  set sequence increment delay in milliseconds
			float delay = GetFloatFromKeyboard("Enter sequence delay in milliseconds: ");
			StepperSetDelay(index, delay);
		}
		else if (input.compare("F") == 0)
		{
			//  set stepper interval
			int interval = GetIntegerFromKeyboard("Set sequence interval: 1 for full sequence, 2 for half sequence: ");
			StepperSetSequenceInterval(index, interval);
		}
		else if (input.compare("I") == 0)
		{
			StepperSpin(index, 1);	
		}
		else if (input.compare("K") == 0)
		{
			StepperSpin(index, -1);
		}
		else if (input.compare("P") == 0)
		{
			StepperStop(index);
		}
		else if (input.compare("O") == 0)
		{
			currentSpeed += 10;
			if (currentSpeed > 100.0)
				currentSpeed = 100.0;
			StepperSetSpeed(index, currentSpeed / 100.0);
			cout << "Stepper speed " << currentSpeed << endl;
		}
		else if (input.compare("L") == 0)
		{
			currentSpeed -= 10;
			if (currentSpeed < -100)
				currentSpeed = -100;
			StepperSetSpeed(index, currentSpeed / 100.0);
			cout << "Stepper speed " << currentSpeed << endl;
			
		}
		else if (input.compare("M") == 0)
		{
			bool foundIndex = false;
			while (!foundIndex)
			{
				int newMotorIndex = GetIntegerFromKeyboard("Enter index of motor to control");
				for (int i = 0; i < motors.size(); i++)
				{
					if (motors[i] == newMotorIndex)
					{
						foundIndex = true;
						index = newMotorIndex;
						break;
					}
				}
			}
		}
		else
		{
			//  parse this as a number
			int number;		
			stringstream convertor;
			convertor << input;
			convertor >> number;

			if (!convertor.fail())
			{
				//  spin the motor the entered number of steps
				StepperStep(index, number);
			}
			else
			{
				// numberString is not a number!
				std::cout << "Please enter an integer number of steps, or h for help" << endl;
			}
		}	
	}
	
	//  go through the motors and shut them down
	for (int i = 0; i < motors.size(); i++)
	{
		StepperRemove(motors[i]);
	}
	
	return 0;
	
}
	
	