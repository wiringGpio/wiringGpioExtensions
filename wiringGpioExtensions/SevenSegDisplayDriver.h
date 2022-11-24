#pragma once

#include <queue>
#include "Thread.h"
#include <list>
#include <array>
#include <vector>
#include <map>
#include <tuple>
#include <condition_variable>


using namespace std;

class SevenSegDisplayDriver : public Thread
{
	
public:
	SevenSegDisplayDriver();
	virtual ~SevenSegDisplayDriver();
	
	bool LoadFromXml(const char* xml);
	
	void SetSegmentPins(int segPins[8]);
	
	void SetDigitPins(int numDigits, int digitPins[]);
	
	void TurnOff();
	
	void SetDisplay(const char* displayCode);
	
	void SetDelay(int delay);
	
	//  Thread override cancel function
	virtual void Cancel();
	
	//  Thread override run function
	virtual void RunFunction();
	
protected:
	
	bool On;
	bool Displaying;
	int SleepDelay;
	
	
	int LastSegmentState;
	int LastDecimalState;
	
			
	//  vector of the segment pins: abcdefg
	array<int,7> SegmentPins;
	
	//  the decimal point pin
	int DecimalPin;
	
	//  the pins controlling each individual digit, there will be one item for each digit in the multiplexed display
	//  digit numbers will be positive for common annode display, and negative for common cathode display
	vector<int>	DigitPins;
	
	//  Init all the pins to be off state
	void InitPinsState();
	
	//  Set all pins to 0
	void SetPinStateAllOff();
	
	
	//  Set Display will put the requested value to display in this queue
	//  the display item is a tuple where first int is index of segments to light up, second digit is decimal point state 0/1
	//  run thread will empty this queue and only show the latest item, others are discared
	std::queue<vector<tuple<int, int>>> DisplayQueue;
	//  queue is protected with mutex
	std::mutex QueueMutex;
	
	
	//  condition variable is used for notification of thread to wake up and display something
	bool Notified;
	std::condition_variable NotifyEventCondition;
	//  notify condition protected with mutex
	std::mutex NotifyMutex;

	void Notify();
};