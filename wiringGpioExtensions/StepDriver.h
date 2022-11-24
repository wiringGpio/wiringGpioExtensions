#pragma once
#include <queue>
#include "Thread.h"
#include <vector>
#include <map>
#include <tuple>
#include <condition_variable>


using namespace std;

class StepDriver : public Thread
{
	
public:
	StepDriver();
	virtual ~StepDriver();
	
	bool LoadFromXml(const char* sequenceXml, const char* pinsXml);
	
	//  Set the step sequence delay in milliseconds (fraction of milliseconds are ok)
	void SetDelay(float delay);
	
	//  Add command to move certain number of steps, positive or negative for direction
	//  this function simply adds command to queue, so will return asynchronously
	void AddStepCommand(int steps);
	
	//  Add command to move continuously in direction, until stop is issued
	void AddSpinCommand(int direction);
	
	//  set stepper speed in percent
	void SetSpeed(float speed);
	
	//  Set stepper sequence interval
	//  set 1 for single step, 2 to do every other steps
	void SetSequenceInterval(int interval);
	
	//  Flushes the command queue and interrupts the current step command to stop the motor
	void Stop();
	
	//  Thread override cancel function
	virtual void Cancel();
	
	//  Thread override run function
	virtual void RunFunction();
	
protected:
	
	//  Sequence Length - number of items in the sequence
	int SequenceLength;
	vector<vector<int>> Sequence;
	
	//  Number of Pins in the sequence
	int NumberOfPins;
	vector<int> Pins;
	
	//  Map of stepper pin to polarity pins
	//  First pin should be IN1, second pin IN2
	//  Polarity will be positive through pin when IN1=1 and IN2=0
	map<int, tuple<int,int>> PinPolarity;
	
	//  Initialization to null state
	void Init();
	
	//  Sequence Index
	//  keeps track of where we are in the sequence
	int SequenceIndex;

	//  Sequence Direction
	//  increments the sequence index in by half intervals (1) or full intervals (2)
	//    Set to 1 or 2 for clockwise
	//    Set to -1 or -2 for anti-clockwise
	int SequenceInterval;
	
	//  Sequence continuous
	//  set this flag to have motor spin until stop command
	bool ContinuousSpin;
	
	
	double MinDelay;
	double MaxDelay;
	
	double PwmScale;
	int PwmValue;
	bool UsePwm;

	//  Sequence Delay
	//  time between each item in the sequence
	int SequenceDelay;
	
	//  Step Interrupt flags
	bool InterruptStepper;
	bool Interrupted;
	
	//  Sequence increment function
	void IncrementSequenceIndex();
	
	//  Move the motor a number of steps (positive or negative for direction)
	void MoveSteps(int steps);
	
	//  Set all pins to value
	void SetAllPins(int value);
	
	
	//  message queue, protected with mutex
	std::mutex QueueMutex;
	std::queue<int> StepQueue;
	
	//  condition variable is used for notification of thread to wake up
	bool Notified;
	std::condition_variable NotifyEventCondition;
	std::mutex NotifyMutex;

	void Notify();
};