#pragma once

#include <array>
#include <list>
#include <chrono>
#include <functional>
#include "wiringGpioExtensions.h"
#include "Thread.h"


//  Rotary Encoder class
//  implemented based on the example here  http://theatticlight.net/posts/Reading-a-Rotary-Encoder-from-a-Raspberry-Pi/
//

//  Encoder Callback Function
//
typedef std::function<void(int)> EncoderUpdatedClassFnCallback;



//
class RotaryEncoder
{
public:
	
	RotaryEncoder();

	//  Construct a rotary encoder
	//  pinA = first encoder pin - triggered first when running forward
	//  pinB = second encoder pin  - triggered first when running backwards
	//  pinIndex = optional index pin (-1 if NA)
	//  countsPerRevolution = number of encoder 'windows' per single turn of system output axis
	//  callback is a function pointer supplied by client application to receive call on encoder tick
	RotaryEncoder(int pinA, int pinB, int pinIndex, int countsPerRevolution, EncoderUpdatedCallback callback);

	virtual ~RotaryEncoder();
	
	//  Get pin A number
	int GetPinA()	{ return PinA; }
	
	//  Get pin B number
	int GetPinB() {	return PinB; }
	
	//  Get index pin number
	int GetPinIndexCounter() { return PinIndexCounter; }
	
	//  Reset the counter to a count value
	void ResetCounter(int value = 0) { TickCount = value * 4;}
	
	//  Get the number of ticks per revolution, it will be four times the number of counts per revolution
	int GetTicksPerRevolution()	{ return TicksPerRevolution; }
	
	//  Get the number of encoder counts per revolution
	int GetCountsPerRevolution()	{ return TicksPerRevolution / 4; }
	
	//  Get encoder position in tick count
	int GetTickCount() {return TickCount;}
	
	//  Get encoder position count
	int GetCount()	{ return TickCount / 4;	}
	
	//  Get encoder position as a number of rotations
	double GetCircle();
	
	//  Get encoder position on the circle between -.5 and +.5 rotations
	double GetNormalizedCircle();
	
	//  Get revolutions per minute of encoder output axis
	double GetRpm();
	
	//  Get the tick frequency in Hz
	double GetTickFrequency() {return TickFrequency;}
	
	//  Get the count frequency in Hz
	double GetCountFrequency()	{return TickFrequency / 4.0;}
	
	
	//  Update functions for the C callback to invoke
	void UpdateEncoder();
	void UpdateIndex();
	
	//  C++ class register a callback with the encoder
	void RegisterUpdatedClassFnCallback(EncoderUpdatedClassFnCallback cbf);
	
		
protected:
	
	void Init();
	
	//  encoder pins
	int PinA;
	int PinB;
	int PinIndexCounter;
	
	void SetupEncoderPins(int pinA, int pinB, int pinIndex);

	
	//  encoder tick count (because our algorithm does quadrature count, this is twice the number of counts per revolution of the motor output
	int TicksPerRevolution;
	
	//  current count
	int TickCount;
	volatile long IndexCount;
	
	//  counter variables
	volatile int LastEncoded;
	volatile int LastEncodedMsb;
	volatile int LastEncodedLsb;
	//int LastMsb;
	//int LastLsb;
	
	double TickFrequency;
	double TickFrequencyLastAverage;
	int TickFrequencySampleLastTickCount;
	std::chrono::system_clock::time_point TickFrequencySampleLastTime;
	
	void CalculateTickFrequency();
	
	
	//  call back for extension class object
	EncoderUpdatedClassFnCallback UpdatedClassFunctionCallback;
	
	//  call back for client application
	EncoderUpdatedCallback UpdatedCallback;
	
}
;



void SetupEncoderWiringPiCallbacks(int index, RotaryEncoder* encoder);