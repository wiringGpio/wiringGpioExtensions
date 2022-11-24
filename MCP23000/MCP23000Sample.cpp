#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include "main.h"
#include "WiringPiExtensionApi.h"
#include "MCP23000Sample.h"

using namespace std;


void PrintMcpState()
{
	cout << endl;
	
	int pinBase = 0;
	int numPins = 0;
	//  Initialize MCP as all output for this sample
	map<int, int>::iterator it;
	for (it = McpMap.begin(); it != McpMap.end(); it++)
	{
		pinBase = it->first;
		numPins = it->second;
		
		for (int i = pinBase; i < pinBase + numPins; i++)
		{
			cout << i << "   ";
		}
		cout << endl;
	
		for (int i = pinBase; i < pinBase + numPins; i++)
		{
			string state = (DigitalRead(i) == 0) ? "OFF" : " ON";
			cout << state << "   ";
		}
		cout << endl;
	}
}


	

//  MCP 23000 chip sample function
//  Shows how to control a pin output one MCP chip pins
//
int Mcp23000SampleMain(int argc, char *argv[])
{
	cout << "MCP Chip Pin On / Off" << endl;
	cout << "Enter pin number to change state, 0 for all off, -1 to quit" << endl;
	
	string input;
	while (true)
	{
		PrintMcpState();		
		cout << ">> ";

		int pinNumber = GetIntegerFromKeyboard("");
	
		if (pinNumber == -1)
		{
			break;
		}
		else if (pinNumber == 0)
		{	
			//  turn everything off
			map<int, int>::iterator it;
			for (it = McpMap.begin(); it != McpMap.end(); it++)
			{
				for (int i = it->first; i < it->first + it->second; i++)
					DigitalWrite(i, 0);
			}
		}
		else
		{
			//  is this a valid pin number
			if ((pinNumber <= 40 && pinNumber >= 1) || (GetPinBaseForMcpPin(pinNumber) != 0))
			{
				//  set the pin to the opposite state
				int state = DigitalRead(pinNumber);
				DigitalWrite(pinNumber, state == 0 ? 1 : 0);
			}
			else
			{
				cout << "Invalid pin number" << endl;
			}
		}
	}
	
	return 0;
}