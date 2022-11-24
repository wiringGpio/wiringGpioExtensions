#include <iostream>
#include <algorithm>
#include <sstream>
#include "WiringPiExtensionApi.h"
#include "main.h"
#include "PinOnOffSample.h"

using namespace std;

//  Prompt user with message and accept integer input from keyboard
int GetIntegerFromKeyboard(string message)
{
	string input;
	int number;
	
	while (true)
	{
		cout << message << " ";
		getline(cin, input);

		stringstream convertor;
		
		convertor << input;
		convertor >> number;

		if (convertor.fail())
		{
			// numberString is not a number!
			cout << "Please enter an integer ! " << endl;
		}
		else
			break;
	}
	
	return number;
}	



int main(int argc, char *argv[])
{
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = SetupWiringPiExtension();
	if (retVal != 0)
		return retVal;
	
	return PinOnOffSampleMain(argc, argv);
}