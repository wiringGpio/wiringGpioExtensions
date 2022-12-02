//  RotaryEncoder.cpp
//  Implementation of rotary encoder class
#include <functional>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <iomanip>

#include "wiringGpioExtensions.h"
#include "RotaryEncoder.h"


using namespace std;

#define MSNANO (1000000)

//  Rotary Encoder Class
//

//  Default Constructor
//
RotaryEncoder::RotaryEncoder()
{
	Init();
}


//  Construct an encoder with pin values
//
RotaryEncoder::RotaryEncoder(int pinA, int pinB, int pinIndex, int countsPerRevolution, EncoderUpdatedCallback callback)
{
	Init();
	
	SetupEncoderPins(pinA, pinB, pinIndex);
	
	UpdatedCallback = callback;
	
	TicksPerRevolution = countsPerRevolution*4;
	
	LogFormatted(LogLevelInfo, "RotaryEncoder", "RotaryEncoder", "Setup rotary encoder %d. Pin A %d. Pin B %d. Pin index %d", pinA, pinB, pinIndex);
}


//  Destructor
//
RotaryEncoder::~RotaryEncoder()
{
	
}


//  Initialize encoder values
//
void RotaryEncoder::Init()
{
	TickCount = 0;
	IndexCount = 0;
	
	LastEncoded = 0;
	LastEncodedMsb = 0;
	LastEncodedLsb = 0;
	
	TickFrequency = 0.0;
	TickFrequencyLastAverage = 0.0;
	TickFrequencySampleLastTime	= std::chrono::system_clock::now();
	TickFrequencySampleLastTickCount = 0;
	
	UpdatedCallback = NULL;
}


//  Setup the pin mode for the encoder pins
//
void RotaryEncoder::SetupEncoderPins(int pinA, int pinB, int pinIndexCounter)
{
	PinA = pinA;
	PinB = pinB;
	PinIndexCounter = pinIndexCounter;
	
	PinMode(PinA, INPUT);
	PinMode(PinB, INPUT);
	PullUpDnControl(PinA, PUD_UP);
	PullUpDnControl(PinB, PUD_UP);
	
	if (PinIndexCounter > 0)
	{
		PinMode(PinIndexCounter, INPUT);
		PullUpDnControl(PinIndexCounter, PUD_UP);
	}
	
	LogFormatted(LogLevelInfo, "RotaryEncoder", "SetupEncoderPins", "Pin A: %d  Pin B: %d  Pin Index: %d", PinA, PinB, PinIndexCounter);
}


// Register the callback function for encoder updates
//
void RotaryEncoder::RegisterUpdatedClassFnCallback(EncoderUpdatedClassFnCallback updatedFn)
{
	UpdatedClassFunctionCallback = updatedFn;
}


//  Update encoder is called by the encoder pinA/B interrupt function
//
void RotaryEncoder::UpdateEncoder()
{
	int msb = DigitalRead(PinA);
	int lsb = DigitalRead(PinB);
	
	int encoded = (msb << 1) | lsb;
	int sum = (LastEncoded << 2) | encoded;

	if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) 
	{
		TickCount++;
	}
	if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
	{
		TickCount--;
	}
		
	LastEncoded = encoded;
	LastEncodedMsb = msb;
	LastEncodedLsb = lsb;
	
	CalculateTickFrequency();
	
	if (UpdatedCallback != NULL)
	{
		UpdatedCallback(TickCount);
	}
	
	if (UpdatedClassFunctionCallback != NULL)
	{	
		UpdatedClassFunctionCallback(TickCount);
	}
}


//  Update index is called by the encoder index pin interrupt function
//  note:  this is not used or tested
void RotaryEncoder::UpdateIndex()
{
	IndexCount++;
	//  TODO - set state, such as sync the 0 mark for the encoder ?
	
}


//  Calculate the frequency of encoder ticks for speed calculations
//
void RotaryEncoder::CalculateTickFrequency()
{
	std::chrono::system_clock::time_point now = std::chrono::high_resolution_clock::now();
		
	int count = (now - TickFrequencySampleLastTime).count();
	if (count > 100*MSNANO)
	{
		TickFrequency = (double)((TickCount - TickFrequencySampleLastTickCount) / ((now - TickFrequencySampleLastTime).count() / 1000000000.0));
		TickFrequency = (TickFrequency + TickFrequencyLastAverage) / 2.0;
		TickFrequencyLastAverage = TickFrequency;
		TickFrequencySampleLastTime = now;
		TickFrequencySampleLastTickCount = TickCount;
		
		//Log("RotaryEncoder", "CalculateTickFrequency", format("Encoder TickFrequency: %.1lf CountFrequency: %.1lf  RPM %.2lf.", TickFrequency, GetCountFrequency(), GetRpm()), LogLevelTrace);
	}
	
}

//  Get encoder position as a number of rotations
//
double RotaryEncoder::GetCircle()
{	
	return ((double)TickCount / (double)TicksPerRevolution);
}



//  Get encoder position as a normalized rotation value
//
double RotaryEncoder::GetNormalizedCircle()
{
	int normalizeTick = TickCount;
	while (TickCount > TicksPerRevolution/2)
	{
		normalizeTick -= TicksPerRevolution;
	}
	while (TickCount < 0)
	{
		normalizeTick += TicksPerRevolution;
	}
	
	return ((double)normalizeTick / (double)TicksPerRevolution);
}


//  Get the revolutions per minute of the encoder system output axis
//
double RotaryEncoder::GetRpm()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	
	if ((now - TickFrequencySampleLastTime).count() < 110*MSNANO)
	{
		return (TickFrequency / TicksPerRevolution * 60);
	}
	else
	{
		return 0.0;
	}
}




//  Static stuff to interface with wiringPi C library

//  Array of pointers to RotaryEncoder objects
RotaryEncoder* Encoders[8];

//  Callback functions for the encoder pin interrupts, one for each encoder
void EncoderZeroCallback()
{
	Encoders[0]->UpdateEncoder();
}
void EncoderZeroIndexCallback()
{
	Encoders[0]->UpdateIndex();
}

void EncoderOneCallback()
{
	Encoders[1]->UpdateEncoder();
}
void EncoderOneIndexCallback()
{
	Encoders[1]->UpdateIndex();
}

void EncoderTwoCallback()
{
	Encoders[2]->UpdateEncoder();
}
void EncoderTwoIndexCallback()
{
	Encoders[2]->UpdateIndex();
}

void EncoderThreeCallback()
{
	Encoders[3]->UpdateEncoder();
}
void EncoderThreeIndexCallback()
{
	Encoders[3]->UpdateIndex();
}

void EncoderFourCallback()
{
	Encoders[4]->UpdateEncoder();
}
void EncoderFourIndexCallback()
{
	Encoders[4]->UpdateIndex();
}

void EncoderFiveCallback()
{
	Encoders[5]->UpdateEncoder();
}
void EncoderFiveIndexCallback()
{
	Encoders[5]->UpdateIndex();
}

void EncoderSixCallback()
{
	Encoders[6]->UpdateEncoder();
}
void EncoderSixIndexCallback()
{
	Encoders[6]->UpdateIndex();
}

void EncoderSevenCallback()
{
	Encoders[7]->UpdateEncoder();
}
void EncoderSevenIndexCallback()
{
	Encoders[7]->UpdateIndex();
}


//  Setup Encoder
//  Set the RotaryEncoder object in the array for the callback functions
void SetupEncoderWiringPiCallbacks(int index, RotaryEncoder* encoder)
{
	Encoders[index] = encoder;
	
	switch (index)
	{
	case 0:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderZeroCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderZeroCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderZeroIndexCallback);
		break;
		
	case 1:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderOneCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderOneCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderOneIndexCallback);
		break;
		
	case 2:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderTwoCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderTwoCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderTwoIndexCallback);
		break;
		
	case 3:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderThreeCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderThreeCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderThreeIndexCallback);
		break;
		
	case 4:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderFourCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderFourCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderFourIndexCallback);
		break;
		
	case 5:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderFiveCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderFiveCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderFiveIndexCallback);
		break;
		
	case 6:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderSixCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderSixCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderSixIndexCallback);
		break;
		
	case 7:
		WiringGpioISR(encoder->GetPinA(), INT_EDGE_BOTH, EncoderSevenCallback);
		WiringGpioISR(encoder->GetPinB(), INT_EDGE_BOTH, EncoderSevenCallback);
		if (encoder->GetPinIndexCounter() > 0)
			WiringGpioISR(encoder->GetPinIndexCounter(), INT_EDGE_BOTH, EncoderSevenIndexCallback);
		break;
	
	default:
		return;
	}
}




