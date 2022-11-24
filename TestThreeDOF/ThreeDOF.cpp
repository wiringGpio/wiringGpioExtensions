#include <iostream>
#include <cstdio>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include "WiringPiExtensionApi.h"


using namespace std;



void PrintThreeDofInstructions(bool rawMode)
{
	system("clear");
	cout << " Axis\t\t-\to\t+" << endl;
	cout << "Green\t\tR\tT\tY" << endl;
	cout << " Blue\t\tF\tG\tH" << endl;
	cout << "  Red\t\tV\tB\tN" << endl;
	cout << " Mode: " << (rawMode ? " Raw " : " Input ") << endl;
	cout << " " << endl;
}


int ThreeDOFSampleMain(int argc, char *argv[])
{
	
	
	//  define the stepper sequence
	string sequenceElement = "<StepperSequence> \
								<SequenceItems> \
		 <Item>1, 0, 0, 1</Item> \
		<Item>1, 0, 0, 0</Item> \
		<Item>1, 1, 0, 0</Item> \
		<Item>0, 1, 0, 0 </Item> \
		<Item>0, 1, 1, 0</Item> \
		<Item> 0, 0, 1, 0</Item> \
		<Item> 0, 0, 1, 1</Item> \
		<Item>0, 0, 0, 1</Item> \
		</SequenceItems> \
		</StepperSequence>";
	
	//  stepper pins
	int  blueStepper, greenStepper, redStepper = 0;
	
	//  blue axis
	string pinsElement = "<StepperDriver><Pins>100,101,102,103</Pins></StepperDriver>";
	blueStepper = StepperCreateFromXml(sequenceElement.c_str(), pinsElement.c_str());
	StepperSetDelay(blueStepper, 10);
	
	//  green axis
	pinsElement = "<StepperDriver><Pins>104,105,106,107</Pins></StepperDriver>";
	greenStepper = StepperCreateFromXml(sequenceElement.c_str(), pinsElement.c_str());
	StepperSetDelay(greenStepper, 10);
	
	//  red axis
	pinsElement = "<StepperDriver><Pins>112,113,114,115</Pins></StepperDriver>";
	redStepper = StepperCreateFromXml(sequenceElement.c_str(), pinsElement.c_str());
	StepperSetDelay(redStepper, 10);
	
	//  Print the instructions to the console
	PrintThreeDofInstructions(false);
	
	//  Start the loop
	string input;
	int lastAxis = blueStepper;
	
	bool rawMode = false;
	bool continueLoop = true;
	while (continueLoop)
	{
		if (rawMode)
		{
			char key = ' ';
			key = getchar();
			//cout << '\b';		//  backspace the cursor
			input = string(1, key);
			printf("%c", 8);
		}
		else
		{
			getline(cin, input);
			fputs("\033[A\033[2K", stdout);
			rewind(stdout);
		}
		
		std::transform(input.begin(), input.end(), input.begin(),::toupper); 
	
		//  Help
		if (input.compare("") == 0 || input.compare("P") == 0)
		{
			if (rawMode)
				system("stty cooked");
			
			PrintThreeDofInstructions(rawMode);
			
			if (rawMode)
				system("stty cooked");

			continue;
		}
		//  Switch Mode
		else if (input.compare("W") == 0)
		{
			if (rawMode)
			{
				system("stty cooked");
				rawMode = false;
				PrintThreeDofInstructions(rawMode);
			}
			else
			{
				rawMode = true;
				PrintThreeDofInstructions(rawMode);
				system("stty raw");
			}
		}
		//  Quit
		else if (input.compare("Q") == 0)
		{
			if (rawMode)
			{
				system("stty cooked");
				rawMode = false;
			}
			
			continueLoop = false;
		}
		//  All Stop
		else if (input.compare(" ") == 0)
		{
			StepperStop(greenStepper);
			StepperStop(blueStepper);
			StepperStop(redStepper);
			
		}
		
		//  Green Axis
		else if (input.compare("R") == 0)
		{
			StepperSpin(greenStepper, -1);
			lastAxis = greenStepper;
			continue;
		}
		else if (input.compare("T") == 0)
		{
			StepperStop(greenStepper);
			lastAxis = greenStepper;
			continue;
		}
		else if (input.compare("Y") == 0)
		{
			StepperSpin(greenStepper, 1);
			lastAxis = greenStepper;
			continue;
		}
		else if (input.compare("E") == 0)
		{
			StepperStep(greenStepper, -1);
			lastAxis = greenStepper;
			continue;
		}
		else if (input.compare("U") == 0)
		{
			StepperStep(greenStepper, 1);
			lastAxis = greenStepper;
			continue;
		}
		
		//  Blue Axis
		else if (input.compare("F") == 0)
		{
			StepperSpin(blueStepper, -1);
			lastAxis = blueStepper;
			continue;
		}
		else if (input.compare("G") == 0)
		{
			StepperStop(blueStepper);
			lastAxis = blueStepper;
			continue;
		}
		else if (input.compare("H") == 0)
		{
			StepperSpin(blueStepper, 1);
			lastAxis = blueStepper;
			continue;
		}
		else if (input.compare("D") == 0)
		{
			StepperStep(blueStepper, -1);
			lastAxis = blueStepper;
			continue;
		}
		else if (input.compare("J") == 0)
		{
			StepperStep(blueStepper, 1);
			lastAxis = blueStepper;
			continue;
		}
		
		//  Red Axis
		else if (input.compare("V") == 0)
		{
			StepperSpin(redStepper, -1);
			lastAxis = redStepper;
			continue;
		}
		else if (input.compare("B") == 0)
		{
			StepperStop(redStepper);
			lastAxis = redStepper;
			continue;
		}
		else if (input.compare("N") == 0)
		{
			StepperSpin(redStepper, 1);
			lastAxis = redStepper;
			continue;
		}
		else if (input.compare("C") == 0)
		{
			StepperStep(redStepper, -1);
			lastAxis = redStepper;
			continue;
		}
		else if (input.compare("M") == 0)
		{
			StepperStep(redStepper, 1);
			lastAxis = redStepper;
			continue;
		}
		
		//  turn the last axis the requested number of steps
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
				StepperStep(lastAxis, number);
			}
			else
			{
				// numberString is not a number!
				std::cout << "Please enter an integer number of steps, or h for help" << endl;
			}
		}
		
		
	}
	
	return 0;
	
}