#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "main.h"
#include "WiringPiExtensionApi.h"
#include "MCP23000Sample.h"

using namespace std;

//  Map of MCP chips, pin base to number of pins
map<int, int> McpMap;





//  Prompt user with message and accept integer input from keyboard
int GetIntegerFromKeyboard(std::string message)
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
	
	//  Setup MCP chip
	Mcp23017Setup(200, 0x23);
	McpMap[200] = 16;
	
	Mcp23008Setup(300, 0x24);
	McpMap[300] =  8;
	
	Mcp23008Setup(400, 0x27);
	McpMap[400] =  8;
	
	Mcp23017Setup(700, 0x22);
	McpMap[700] = 16;
	
	//  Initialize all MCP pins for output in this sample
	map<int, int>::iterator it;
	for (it = McpMap.begin(); it != McpMap.end(); it++)
	{
		for (int i = 0; i < it->second; i++)
			PinMode(i + it->first, PINMODE_OUTPUT);
	}
	
	return Mcp23000SampleMain(argc, argv);
}
