//  SevenSegDisplayDriver.cpp
//  Implementation of software multiplexing to drive N digit seven segment display
#include <iostream>
#include <sstream>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "wiringGpioExtensions.h"
#include "SevenSegDisplayDriver.h"
#include "tinyxml2.h"
#include "StringExtensionMethods.h"




using namespace tinyxml2;

//  Define the pin states for each segment to make numbers 0-9 and minus sign
array<int,7> segmentStateOne		=	{ 0, 1, 1, 0, 0, 0, 0 };
array<int,7> segmentStateTwo		=	{ 1, 1, 0, 1, 1, 0, 1 };
array<int,7> segmentStateThree		=	{ 1, 1, 1, 1, 0, 0, 1 };
array<int,7> segmentStateFour		=	{ 0, 1, 1, 0, 0, 1, 1 };
array<int,7> segmentStateFive		=	{ 1, 0, 1, 1, 0, 1, 1 };
array<int,7> segmentStateSix		=	{ 1, 0, 1, 1, 1, 1, 1 };
array<int,7> segmentStateSeven		=	{ 1, 1, 1, 0, 0, 0, 0 };
array<int,7> segmentStateEight		=	{ 1, 1, 1, 1, 1, 1, 1 };
array<int,7> segmentStateNine		=   { 1, 1, 1, 0, 0, 1, 1 };
array<int,7> segmentStateZero		=	{ 1, 1, 1, 1, 1, 1, 0 };
array<int, 7> segmentStateMinus		=	{ 0, 0, 0, 0, 0, 0, 1 };
//  all pins off
array<int, 7> segmentStateOff = { 0, 0, 0, 0, 0, 0, 0 };

//  Array of all segment states
//  index 0  = off
//  index 1 - 10 = numbers 1 to 9 and zero
//  index 11 = minus sign
array<array<int, 7>, 12> SegmentStates = { segmentStateOff, segmentStateOne, segmentStateTwo, segmentStateThree, segmentStateFour, segmentStateFive, segmentStateSix, segmentStateSeven, segmentStateEight, segmentStateNine, segmentStateZero, segmentStateMinus };


//  Constructor
//
SevenSegDisplayDriver::SevenSegDisplayDriver()
{
	SleepDelay = 5500;
	Displaying = false;
}


//  Desctuctor
SevenSegDisplayDriver::~SevenSegDisplayDriver()
{
}
	

//  Load from XML
//  definition of the segment and digit pins
bool SevenSegDisplayDriver::LoadFromXml(const char* xml)
{
	//  load the seven segment display
	/*
	<SevenSegDisplay>
		<!-- pins associated with segments a,b,c,d,e,f,g and decimal point -->
		<SegPins>1,2,3,4,5,6,7,8</SegPins>
		<!-- pins associated with each segment cathode/annode -->
		<!-- you will get one digit display for each pin specified here -->
		<!-- use -ve numbers if this is common cathode, positive number for common annode -->
		<Digits>10,11,12</Digits>
	</SevenSegDisplay>
	*/
	
	XMLDocument doc;
	if (doc.Parse(xml) != XML_SUCCESS)
		return false;
	
	//  root element
	//  <SevenSegDisplay>
	XMLElement* rootElement = doc.GetDocument()->FirstChildElement();
	if (rootElement == NULL)
		return false;
	
	//  get the SegPins element
	//  <SegPins>
	XMLElement* segPinsElement = rootElement->FirstChildElement("SegPins");
	if (segPinsElement == NULL || segPinsElement->GetText() == NULL)
		return false;
	
	//  get the segment pins
	vector<int> segPins;
	if (!ParseIntVector(segPinsElement->GetText(), segPins, ','))
		return false;
	for (int i = 0; i < 8; i++)
	{
		if (i < segPins.size())
			SegmentPins[i] = segPins[i];
	}
	
	//  get the digit pins
	//  <Digits>
	XMLElement* digPinsElement = rootElement->FirstChildElement("Digits");
	if (digPinsElement == NULL || digPinsElement->GetText() == NULL)
		return false;
	
	vector<int> digitPins;
	DigitPins.clear();
	if (!ParseIntVector(digPinsElement->GetText(), digitPins, ','))
		return false;
	for (int i = digitPins.size() - 1; i >= 0; i--)
	{
		DigitPins.push_back(digitPins[i]);
	}
	
	//  init the segment pins
	for (int i = 0; i < 7; i++)
	{
		PinMode(SegmentPins[i], PINMODE_OUTPUT);
		DigitalWrite(SegmentPins[i], 0);
	}
	//  init the digit pins
	for (int i = 0; i < DigitPins.size(); i++)
	{
		PinMode(abs(DigitPins[i]), PINMODE_OUTPUT);
		DigitalWrite(abs(DigitPins[i]), 0);
	}
	
	
	return true;
}


//  Set segment pins and initialize pin mode 
//
void SevenSegDisplayDriver::SetSegmentPins(int segPins[8])
{
	for (int i = 0; i < 8; i++)
		SegmentPins[i] = segPins[i];
	
	for (int i = 0; i < 7; i++)
	{
		PinMode(SegmentPins[i], PINMODE_OUTPUT);
		DigitalWrite(SegmentPins[i], 0);
	}
}



//  Set digit Pins and initialize pin mode 
void SevenSegDisplayDriver::SetDigitPins(int numDigits, int digitPins[])
{
	
	for (int i = numDigits - 1; i >= 0; i--)
		DigitPins.push_back(digitPins[i]);
	
	for (int i = 0; i < DigitPins.size(); i++)
	{
		PinMode(abs(DigitPins[i]), PINMODE_OUTPUT);
		DigitalWrite(abs(DigitPins[i]), 0);
	}
}



//  Set delay function 
//  used for testing to tune the flicker
void SevenSegDisplayDriver::SetDelay(int delay)
{
	SleepDelay = delay;
}


//  Turn Off
//  stop displaying anything, but don't stop the driver
void SevenSegDisplayDriver::TurnOff()
{
	//  set the flag to turn off
	On = false;
	if (Displaying)
	{
		while (Displaying)
			usleep(1000);
	}
	
	InitPinsState();
	
}
	

//  Set Display
//  displays the display code string
//  valid characters are 0-9, minus sign and decimal point
//  will only display the number of characters you have digits in the seven segment display, right justified
void SevenSegDisplayDriver::SetDisplay(const char* displayCode)
{
	int nextDigit = -1;
	
	vector<tuple<int, int>> digitsToWrite;
	
	//  parse the char array into digits and decimal points
	int i = 0;
	int nextChar = displayCode[i];
	while (nextChar != 0x00)
	{
		//  see if we are ready to write a digit
		switch (nextChar)
		{
			//  if we are waiting to write digit, and next char is a digit then write it
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:
		case 45:
			{
				if (nextDigit != -1)
					digitsToWrite.insert(digitsToWrite.begin(), tuple<int, int>(nextDigit, 0));
				nextDigit = -1;
				
			}
			break;
			
		case 46:
			break;	//  decimal point, just continue
			
		default:
			return;	//  invalid char for this display
		}
		
		//  process the next char
		switch (nextChar)
		{
		case 48:	//  zero digit is at ten index
			nextDigit = 10;
			break;
		case 49:	//  One
			nextDigit = 1;
			break;
		case 50:	// Two
			nextDigit = 2;
			break;
		case 51:	//  Three
			nextDigit = 3;
			break;
		case 52:	//  Four
			nextDigit = 4;
			break;
		case 53:	//  Five
			nextDigit = 5;
			break;
		case 54:	//  Six
			nextDigit = 6;
			break;
		case 55:	//  Seven
			nextDigit = 7;
			break;
		case 56:	//  Eight
			nextDigit = 8;
			break;
		case 57:	//  Nine
			nextDigit = 9;
			break;
		case 45:	//  Minus sign
			//  minus sign
			nextDigit = 11;
			break;
		case 46:
			//  decimal point, this will end a digit
			if (nextDigit == -1)
			{
				//  decimal point only
				digitsToWrite.insert(digitsToWrite.begin(), tuple<int, int>(11, 1));
			}
			else
			{
				digitsToWrite.insert(digitsToWrite.begin(), tuple<int, int>(nextDigit, 1));
			}
			nextDigit = -1;
			
		}
		
		nextChar = displayCode[++i];
	}
	
	if ( nextDigit != -1 )
		digitsToWrite.insert(digitsToWrite.begin(), tuple<int, int>(nextDigit, 0));
		
	//  break out of current display loop
	On = false;
	while (Displaying)
		usleep(1000);
	
	//  add this to the display queue
	{
		LockMutex lockQueue(QueueMutex);
		DisplayQueue.push(digitsToWrite);
	}
	
	//  notify the thread
	Notify();
}



//  Stop the driver and end the thread
void SevenSegDisplayDriver::Cancel()
{
	ThreadRunning = false;
	Notify();
	Thread::Cancel();
	SetPinStateAllOff();
}
	

//  Notify the thread wait function that something needs to happen
//
void SevenSegDisplayDriver::Notify()
{
	Notified = true;
	NotifyEventCondition.notify_one();
}


//  Thread override run function
//
void SevenSegDisplayDriver::RunFunction()
{
	
	if (DisplayQueue.size() == 0 && ThreadRunning)
	{
		//  wait for messages
		std::unique_lock<std::mutex> lockNotify(NotifyMutex);

		//  avoid spurious wakeups
		while (!Notified)
			NotifyEventCondition.wait(lockNotify);
	}
	Notified = false;
	
	//  check to see if we were woken up because of shutdown
	if (!ThreadRunning)
		return;
	
	//  also check to see if the queue is empty (double notify)
	if (On && DisplayQueue.size() == 0)
		return;
	
	//  get rid of all the items in the queue except the last one
	vector<tuple<int, int>> displayItem;
	{
		LockMutex lockQueue(QueueMutex);
		displayItem = DisplayQueue.back();
		while (!DisplayQueue.empty())
			DisplayQueue.pop();
	}
	
	//  get the on / off state depending on display type (common cathode or common annode)
	int digitPinOnState = DigitPins[0] > 0 ? 0 : 1;
	int digitPinOffState = DigitPins[0] > 0 ? 1 : 0;
	
	int segPinOnState = DigitPins[0] > 0 ? 1 : 0;
	int segPinOffState = DigitPins[0] > 0 ? 0 : 1;
	
	// start with everything off
	InitPinsState();
		
	//  run the display loop
	On = true;
	
	while (On && ThreadRunning)
	{
		//  flag for when we are in this loop
		Displaying = true;
		
		int numPins = min(displayItem.size(), DigitPins.size());
		for (int i = 0; i < numPins; i++)
		{			
			//  turn off the last Digit pin
			if (i == 0)
				DigitalWrite(abs(DigitPins[numPins - 1]), digitPinOffState);
			else
				DigitalWrite(abs(DigitPins[i-1]), digitPinOffState);
				
			//  get the next segment state, its index is the first part of the display item tuple
			array<int, 7>& nextSegmentState = SegmentStates[get<0>(displayItem[i])];
			//  get the next decimal state, its value is the second part of the display item tuple
			int nextDecimalState = get<1>(displayItem[i]);
			
			//  enable the pins we need to
			for (int j = 0; j < 7; j++)
			{
				if (nextSegmentState[j] != SegmentStates[LastSegmentState][j])
					DigitalWrite(abs(SegmentPins[j]), nextSegmentState[j] > 0 ? segPinOnState : segPinOffState);
			}
			
			//  decimal point
			if ( nextDecimalState != LastDecimalState )
				DigitalWrite(DecimalPin, nextDecimalState > 0 ? segPinOnState : segPinOffState);
				
			//  turn on this digit pin
			DigitalWrite(abs(DigitPins[i]), digitPinOnState);
	
			LastSegmentState = get<0>(displayItem[i]);
			LastDecimalState = nextDecimalState;
		
			//  do we only need to do this once
			if (displayItem.size() == 1)
			{
				Displaying = false;
				return;
			}
			
			usleep((useconds_t)SleepDelay);
		}
	}
	
	//  if we are no longer displaying, turn off
	if (!On)
		InitPinsState();
	
	//  exit this loop
	Displaying = false;
}


void SevenSegDisplayDriver::InitPinsState()
{
	int digitPinOffState = DigitPins[0] > 0 ? 1 : 0;
	
	int segPinOffState = DigitPins[0] > 0 ? 0 : 1;
	
	//  turn segment pins to off
	for (int i = 0; i < 7; i++)
	{
		DigitalWrite(SegmentPins[i], segPinOffState);
	}
	DigitalWrite(DecimalPin, segPinOffState);
	LastDecimalState = 0;
	
	//  turn all digit pins to off
	for (int i = 0; i < DigitPins.size(); i++)
	{
		DigitalWrite(abs(DigitPins[i]), digitPinOffState);
		
	}
	
	LastSegmentState = 0;
}
 


void SevenSegDisplayDriver::SetPinStateAllOff()
{	
	//  turn segment pins to off
	for (int i = 0; i < 7; i++)
	{
		DigitalWrite(SegmentPins[i], 0);
	}
	DigitalWrite(DecimalPin, 0);
	LastDecimalState = 0;
	
	//  turn all digit pins to off
	for (int i = 0; i < DigitPins.size(); i++)
	{
		DigitalWrite(abs(DigitPins[i]), 0);
	}
	
	LastSegmentState = 0;
}
 