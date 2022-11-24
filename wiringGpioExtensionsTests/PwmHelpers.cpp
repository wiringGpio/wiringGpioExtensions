#include <map>
#include <iostream>

#include <wiringGpioExtensions.h>

#include "PwmHelpers.h"



using namespace std;



void PcaAllOff()
{
	//  turn everything off
	map<int, int>::iterator it;
	for (it = PcaMap.begin(); it != PcaMap.end(); it++)
	{
		//  write the all off command (pin base + 16)
		DigitalWrite(it->first + 16, 0);
	}
}


int calcTicks(float impulseMs, float hertz) 
{ 
	float cycleMs = 1000.0f / hertz; 
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f); 
} 


void PrintPcaState()
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
	
		for (int i = pinBase; i < pinBase + numPins; i++)
		{
				
			int read = DigitalRead(i);
			//  get the state
			int fullOffState = (read & 0x1000);
			
			string state = (fullOffState == 4096 ? "OFF " : " ON ");
			cout << state;
		}
		cout << endl;
	}
}

extern std::map<int, int> PcaMap;