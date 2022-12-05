//  StepDriver.cpp
//  Implementation of a simple software driven stepper motor driver
//  
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wiringGpioExtensions.h"
#include "StepDriver.h"
#include "tinyxml2.h"
#include "StringExtensionMethods.h"

using namespace tinyxml2;

//  Constructor
//
StepDriver::StepDriver()
{
	Init();
}


//  Destructor
//
StepDriver::~StepDriver()
{
	if (ThreadRunning)
	{
		Cancel();
	}
	
	while (!StepQueue.empty())
		StepQueue.pop();
}



//  Init
//
void StepDriver::Init()
{
	Sequence.clear();
	SequenceLength = 0;
	Pins.clear();
	NumberOfPins = 0;
	PinPolarity.clear();
	
	SequenceInterval = 1;
	SequenceIndex = 0;
	SequenceDelay = 2000;	// 2 ms default
	
	//   queue notification
	Notified = false;
	
	//  Interrupt flags
	InterruptStepper = false;
	Interrupted = false;
	
	ContinuousSpin = false;
	
	MinDelay = 1.0;
	MaxDelay = 50;
	
	PwmScale = 1.0;
	UsePwm = false;
}


//  Load Stepper driver from XML element for sequence and pins
//
bool StepDriver::LoadFromXml(const char* sequenceXml, const char* pinsXml)
{
	Init();
	
	//  Load the Driver Pins
	/*
	<!--Stepper Driver Unipolar--> 
	<StepperDriver>
		<Sequence>1</Sequence>
		<Pins>100, 101, 102, 103</Pins>
	</StepperDriver>
	<!--Stepper Driver BiPolar--> 
	<StepperDriver>
		<Sequence>1</Sequence>
		<Pins>100, -100, 101, -101</Pins>
		<PwmScale>0.5</PwmScale>   
		<HBridge>100,104,105</HBridge>
		<HBridge>101,106,107</HBridge>
	</StepperDriver>
	*/
	
	XMLDocument doc;
	if (doc.Parse(pinsXml) != XML_SUCCESS)
		return false;
	
	//  root element
	//  <StepperDriver>
	XMLElement* rootElement = doc.GetDocument()->FirstChildElement();
	if (rootElement == NULL)
		return false;
	
	//  get the Pins element
	//  <Pins>
	XMLElement* pinsElement = rootElement->FirstChildElement("Pins");
	if (pinsElement == NULL || pinsElement->GetText() == NULL)
		return false;
	
	//  parse this text into the pin numbers
	if (!ParseIntVector(pinsElement->GetText(), Pins, ','))
		return false;
	
	//  set the number of pins
	NumberOfPins = Pins.size();
	
	//  see if we are using PWM
	XMLElement* pwmElement = rootElement->FirstChildElement("PwmScale");
	if (pwmElement != NULL)
	{
		if (pwmElement->QueryDoubleText(&PwmScale) != XML_SUCCESS)
			PwmScale = 1.0;
		else
		{
			//  this is PWM driver
			UsePwm = true;
			//  get the pwm value for this scale, based on PWM range
			//  TODO this is hard coded for PCA chips at the moment
			PwmValue = (int)(PwmScale * 4096);
		}
	}
	
	MinDelay = 1.0;
	XMLElement* minDelayElement = rootElement->FirstChildElement("MinDelay");
	if (minDelayElement != NULL)
	{
		if (minDelayElement->QueryDoubleText(&MinDelay) != XML_SUCCESS)
			MinDelay = 1.0;
	}
	
	MaxDelay = 50.0;
	XMLElement* maxDelayElement = rootElement->FirstChildElement("MaxDelay");
	if (maxDelayElement != NULL)
	{
		if (maxDelayElement->QueryDoubleText(&MaxDelay) != XML_SUCCESS)
			MaxDelay = 50.0;
	}
	
	//  see if we have polarity elements
	XMLElement* polarityElement = rootElement->FirstChildElement("HBridge");
	while (polarityElement != NULL)
	{
		vector<int> polarityPins;
		if (!ParseIntVector(polarityElement->GetText(), polarityPins, ','))
			return false;
		if (polarityPins.size() != 3)
			return false;
		PinPolarity[polarityPins[0]] =  tuple<int, int>(polarityPins[1], polarityPins[2]);
		polarityElement = polarityElement->NextSiblingElement("HBridge");
	}
	
	//  check to see if we have polarity pins we need
	for (int i = 0; i < Pins.size(); i++)
	{
		if (Pins[i] < 0)
		{
			//  need to find this one in the polarity elements
			map<int, tuple<int, int>>::iterator it;
			it = PinPolarity.find(abs(Pins[i]));
			if (it == PinPolarity.end())
				return false;
		}
	}
	
	for (int i = 0; i < NumberOfPins; i++)
	{
		if (UsePwm)
		{
			PinMode(Pins[i], PWM_OUTPUT);
			PwmWrite(abs(Pins[i]), 0);
		}
		else
		{
			PinMode(Pins[i], OUTPUT);
			DigitalWrite(abs(Pins[i]), 0);
		}
	}

		
	//  Parse the sequence element
	/*
	 <StepperSequence>
     <Id>1</Id>
		 <SequenceItems>
		  <Item>1,1,0,0</Item>
		  <Item>0,1,1,0</Item>
		  <Item>0,0,1,1</Item>
		  <Item>1,0,0,1</Item>
		 </SequenceItems>
	  </StepperSequence>
	*/
	
	doc.Clear();
	if (doc.Parse(sequenceXml) != XML_SUCCESS)
		return false;
	
	//  Get the rootElement
	//  <StepperSequence>
	rootElement = doc.GetDocument()->FirstChildElement();
	if (rootElement == NULL)
		return false;
	
	//  Get the sequence items element
	//  <SequenceItems>
	XMLElement* sequenceItemsElement = rootElement->FirstChildElement("SequenceItems");
	if (sequenceItemsElement == NULL)
		return false;
	
	//  Get all the items in the sequence
	//  <Item>
	XMLElement* nextItem = sequenceItemsElement->FirstChildElement("Item");
	while (nextItem != NULL)
	{
		vector<int> item;
		if (!ParseIntVector(nextItem->GetText(), item, ','))
			return false;
		//  this vector must be same length as number of pins
		if (item.size() != Pins.size())
			return false;
		
		//  add this vector to the sequence
		Sequence.push_back(item);
		
		//  go again
		nextItem = nextItem->NextSiblingElement("Item");
	}
	
	SequenceLength = Sequence.size();
	
	return true;
}


// Add command to the queue to turn specific number of steps
//
void StepDriver::AddStepCommand(int steps)
{
	//  can not add additional step commands when we are spinning
	if (ContinuousSpin)
		return;
	{
		LockMutex lockQueue(QueueMutex);
		StepQueue.push(steps);
	}
	
	Notify();
}


//  Add command to the queue to spin continously in a direction (1 or -1)
//
void StepDriver::AddSpinCommand(int direction)
{
	//  dummy up a number of steps
	int numberOfSteps = 999;
	if (direction < 0)
		numberOfSteps *= -1;
	
	//  purge all existing commands
	Stop();
	ContinuousSpin = true;
	{
		LockMutex lockQueue(QueueMutex);
		StepQueue.push(numberOfSteps);
	}
	
	Notify();
}


//  Set stepper speed with unit vector between -1.0 and 1.0
//  delay will be set based on main and max delay
//
void StepDriver::SetSpeed(float speed)
{
	//  determine delay from speed value
	float speedReverse = 1.0 - fabs(speed);
	float delay = MinDelay + speedReverse*(MaxDelay - MinDelay);
	SetDelay(delay);
	
	//  are we spinning already
	if (ContinuousSpin)
	{
		if (speed > 0)
			SequenceInterval = abs(SequenceInterval);
		else 
			SequenceInterval = abs(SequenceInterval) * -1;
	}
	else
	{
		AddSpinCommand((speed > 0.0) ? 1 : -1);
	}
}


// Set sequence interval
// sets the direction and any sequence skip
//
void StepDriver::SetSequenceInterval(int interval)
{
	interval = abs(interval);
	//  check interval for valid range
	if (interval > 2 || interval <= 0)
		return;
	
	//  find out sign of current interval
	int sign = (SequenceInterval > 0) ? 1 : -1;
	
	//  set the new sequence interval preserving current turning direction
	SequenceInterval = interval * sign;
}


//  Stop the stepper motor, emptying the queue of any pending commands
//
void StepDriver::Stop()
{
	//  empty the command queue
	{
		LockMutex lockQueue(QueueMutex);
		while (!StepQueue.empty())
			StepQueue.pop();
	}
	
	//  stop the driver
	InterruptStepper = true;
	int interruptCount = 0;
	Notify();
	while (!Interrupted && interruptCount < 100)
	{
		usleep(100 * 1000);
		interruptCount++;
	}
	
	//  reset flags
	InterruptStepper = false;
	Interrupted = false;
	ContinuousSpin = false;
}


//  Set Delay in milliseconds
//
void StepDriver::SetDelay(float delay)
{
	//  sequence delay is in microseconds
	SequenceDelay = (int)(delay * 1000);
}


//  Stop the stepper and end the thread
//  all pins in the stepper will be turned off
//
void StepDriver::Cancel()
{
	ThreadRunning = false;
	Notify();
	Thread::Cancel();
	SetAllPins(0);
}


//  Notify the command queue that something has been added
//
void StepDriver::Notify()
{
	Notified = true;
	NotifyEventCondition.notify_one();
}


//  Stepper thread run function
//
void StepDriver::RunFunction()
{
	if (StepQueue.size() == 0 && ThreadRunning)
	{
		//  wait for messages
		std::unique_lock<std::mutex> lockNotify(NotifyMutex);

		//  avoid spurious wakeups
		while (!Notified)
			NotifyEventCondition.wait(lockNotify);

		//  check to see if we were woken up because of shutdown
		if (!ThreadRunning)
			return;
		
		//  check to see if we were woken up by interrupt steps request
		if (InterruptStepper)
		{
			Notified = false;
			Interrupted = true;
			return;
		}
	}

	//  service the queue
	int nextSteps = 0;
	bool move = false;
	{
		LockMutex lockQueue(QueueMutex);
		if (!StepQueue.empty())
		{
			nextSteps = StepQueue.front();
			StepQueue.pop();
			move = true;
		}
	}
	//  if we got a command from the queue, move on it here
	if (move)
		MoveSteps(nextSteps);
		
	Notified = false;

	return;
}


//  Increment sequence index
//  change by 1 or 2 depending on if you are skipping items in the sequence
//
void StepDriver::IncrementSequenceIndex()
{
	SequenceIndex += SequenceInterval;
	 
	// If we reach the end of the sequence
	// start again - rolling in the correct turn direction
	if (SequenceIndex >= SequenceLength)
		SequenceIndex = 0;
	else if (SequenceIndex < 0)
		SequenceIndex = SequenceLength + SequenceInterval;
}


//  Move a specific number of steps
//
void StepDriver::MoveSteps(int steps)
{
	//  set the sequence direction for the step direction
	int numberOfSteps = steps;
	if (steps > 0)
	{
		SequenceInterval = abs(SequenceInterval);
	}
	else 
	{
		SequenceInterval = abs(SequenceInterval) * -1;
		numberOfSteps = steps * -1;
	}
	
	// calculate the number of times to execute the sequence for each step
	int numSequences = SequenceLength / abs(SequenceInterval);
	
	//  count down the steps
	int doSteps = numberOfSteps;
	
	map<int, tuple<int, int>>::iterator it;
	map<int, int> pinsOnThisSequence;
	map<int, int>::iterator findPin;
	
	//  perform the number of steps
	while (doSteps != 0)
	{
		//  do each sequence this step
		for (int i = 0; i < numSequences; i++)
		{
			//  check for interruption
			if (InterruptStepper)
			{
				SetAllPins(0);
				Interrupted = true;
				return;
			}
			
			//  set all pins this sequence
			for (int pinIndex = 0; pinIndex < NumberOfPins; pinIndex++)
			{
				int thisPin = Pins[pinIndex];
				bool turnOn = (Sequence[SequenceIndex][pinIndex] > 0);
				
				//  do we need to deal with polarity
				if (PinPolarity.size() != 0) 
				{	
					it = PinPolarity.find(abs(thisPin));
					if (it != PinPolarity.end())
					{
						if (turnOn)
						{
						//  we have polarity, we need to deal with it
							if (thisPin > 0)
							{
								//  set polarity positive
								DigitalWrite(abs(get<0>(it->second)), 1);
								DigitalWrite(abs(get<1>(it->second)), 0);
							}
							else
							{
								//  set polarity negative
								DigitalWrite(abs(get<0>(it->second)), 0);
								DigitalWrite(abs(get<1>(it->second)), 1);
							}
						}
					}
				}
				
				findPin = pinsOnThisSequence.find(abs(thisPin));
				if (turnOn || findPin == pinsOnThisSequence.end())
				{
					if (UsePwm)
					{
						bool pwmWrite = (Sequence[SequenceIndex][pinIndex] == 1);
						PwmWrite(abs(thisPin), pwmWrite ? PwmValue : 0);
					}
					else
					{
						DigitalWrite(abs(thisPin), Sequence[SequenceIndex][pinIndex]);	
					}
					
					if (turnOn)
						pinsOnThisSequence[abs(thisPin)] = 1;
				}
					
			}
			
			//  pause before the next item in the sequence
			usleep(SequenceDelay);
			pinsOnThisSequence.clear();
			IncrementSequenceIndex();
		}
		
		//  done one step
		if (!ContinuousSpin)
			doSteps -= 1;
	}
	
	//  set all pins to 0
	SetAllPins(0);
	
}


//  Set all pins in the stepper to the value (1 or 0)
void StepDriver::SetAllPins(int value)
{
	for (int pin = 0; pin < NumberOfPins; pin++)
	{
		DigitalWrite(abs(Pins[pin]), value);
	}
}