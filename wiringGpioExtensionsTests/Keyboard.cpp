#include <iostream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <string>


#include <iomanip>


#include "Keyboard.h"

using namespace std;

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


float GetFloatFromKeyboard(string message)
{
	string input;
	float number;
	
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
			std::cout << "Please enter a number !" << endl;
		}
		else
			break;
	}
	
	return number;
}	