//  Encoder Samples
//  some functions for debugging rotary encoder code
//  based on code originally by Andrew Stine from here http://theatticlight.net/posts/Reading-a-Rotary-Encoder-from-a-Raspberry-Pi/
//
//
#include <unistd.h>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <stdio.h>
#include <wiringGpioExtensions.h>

#include "EncoderSamples.h"


using namespace std;

//  Rotary Encoder Struct
#define max_encoders 8
struct encoder
{
	int pin_a;
	int pin_b;
	volatile long value;
	volatile int lastEncoded;
	volatile int lastEncodedMsb;
	volatile int lastEncodedLsb;
	volatile int signalCount;
	
	void Init()
	{
		value = 0;
		lastEncoded = 0;
		lastEncodedLsb = 0;
		signalCount = 0;
	}
};

//Pre-allocate encoder objects on the stack so we don't have to
//worry about freeing them
struct encoder encoders[max_encoders];
int numberofencoders = 0;  


//  Update Encoders function
//  original version - modified to detect redundant signals
//  this version uses rising and falling edges
//  and increments the value for each event, therefore value is 4 * ticks for every full rotation of the shaft
void updateEncodersOriginal()
{
	struct encoder *encoder = encoders;
	for (; encoder < encoders + numberofencoders; encoder++)
	{
		int MSB = DigitalRead(encoder->pin_a);
		int LSB = DigitalRead(encoder->pin_b);
		
		//  modified original code
		//  check if we ever get the same state twice in a row
		if (MSB == encoder->lastEncodedMsb && LSB == encoder->lastEncodedLsb)
		{
			cout << "! redundant signal " << MSB << " " << LSB << endl;
			//  encoder performs with a bit less drift when you continue here
			//	continue;
		}
		
		int encoded = (MSB << 1) | LSB;
		int sum = (encoder->lastEncoded << 2) | encoded;

		if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoder->value++;
		if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoder->value--;

		cout << "updateEncoders " << MSB << " " << LSB << " " << encoder->value << endl;
		
		encoder->lastEncoded = encoded;
		encoder->lastEncodedMsb = MSB;
		encoder->lastEncodedLsb = LSB;
	}
}
//  Setup Encoder using original sample code
struct encoder *setupEncoderOriginal(int pin_a, int pin_b)
{
			
	if (numberofencoders > max_encoders)
	{
		return NULL;
	}

	struct encoder *newencoder = encoders + numberofencoders++;
	newencoder->pin_a = pin_a;
	newencoder->pin_b = pin_b;
	newencoder->Init();

	PinMode(pin_a, PINMODE_INPUT);
	PinMode(pin_b, PINMODE_INPUT);
	PullUpDnControl(pin_a, PULLUPDN_UP);
	PullUpDnControl(pin_b, PULLUPDN_UP);
	WiringGpioISR(pin_a, INTERRUPT_EDGE_BOTH, updateEncodersOriginal);
	WiringGpioISR(pin_b, INTERRUPT_EDGE_BOTH, updateEncodersOriginal);

	return newencoder;
};



//  Update Encoders for test single signal
//  tests signal on just one pin, with rising and falling edges
//  increments value only when signal goes high, therefore value is number of ticks for full rotation of the shaft
void updateEncodersTestOneSignal()
{
	struct encoder *encoder = encoders;
	for (; encoder < encoders + numberofencoders; encoder++)
	{
		int MSB = DigitalRead(encoder->pin_a);
		if (MSB == 1 && encoder->lastEncodedMsb == 0)
		{
			encoder->value++;
			encoder->lastEncodedMsb = MSB;
			encoder->signalCount = 0;
			
			cout << "updateEncoders " << MSB <<  " " << encoder->value << endl;
		}
		else if (MSB == 0 && encoder->lastEncodedMsb == 1)
		{
			encoder->lastEncodedMsb = 0;
			encoder->signalCount = 0;
			
			cout << "updateEncoders " << MSB << " " << encoder->value << endl;
		}
		else
		{
			encoder->signalCount++;
			cout << "! updateEncoders redundant signal " << MSB << " | " << encoder->signalCount <<  endl;
		}
	}
}
//  Setup Encoders for test one signal
struct encoder *setupEncoderTestOneSignal(int pin_a)
{			
	if (numberofencoders > max_encoders)
	{
		return NULL;
	}

	struct encoder *newencoder = encoders + numberofencoders++;
	newencoder->pin_a = pin_a;
	newencoder->pin_b = -1;
	newencoder->Init();

	PinMode(pin_a, PINMODE_INPUT);
	PullUpDnControl(pin_a, PULLUPDN_UP);
	WiringGpioISR(pin_a, INTERRUPT_EDGE_BOTH, updateEncodersTestOneSignal);

	return newencoder;
};


//  Update Encoders for performance
//  tests signal on just one pin, with rising edge only
//  increments value only when signal goes high, therefore value is number of ticks for full rotation of the shaft
void updateEncodersForPerformance()
{
	struct encoder *encoder = encoders;
	for (; encoder < encoders + numberofencoders; encoder++)
	{
		int MSB = DigitalRead(encoder->pin_a);
		int LSB = DigitalRead(encoder->pin_b);
		if (MSB == 1 && encoder->lastEncodedMsb == 0)
		{
			if (LSB == 0)
				encoder->value++;
			else
				encoder->value--;
			
			encoder->lastEncodedMsb = MSB;
			encoder->signalCount = 0;
			
			cout << "updateEncoders " << MSB << " " << LSB << " " << encoder->value << endl;
		}
		else if (MSB == 0 && encoder->lastEncodedMsb == 1)
		{
			encoder->lastEncodedMsb = 0;
			encoder->signalCount = 0;
			
			cout << "updateEncoders " << MSB << " " << LSB << endl;
		}
		else
		{
			encoder->signalCount++;
			cout << "! updateEncoders redundant signal " << MSB << " " << LSB << encoder->signalCount << endl;
		}
	}
}
//
struct encoder* setupEncoderOptimalCpu(int pin_a, int pin_b)
{
	if (numberofencoders > max_encoders)
	{
		return NULL;
	}

	struct encoder *newencoder = encoders + numberofencoders++;
	newencoder->pin_a = pin_a;
	newencoder->pin_b = pin_b;
	newencoder->Init();

	PinMode(pin_a, PINMODE_INPUT);
	PullUpDnControl(pin_a, PULLUPDN_UP);
	PinMode(pin_b, PINMODE_INPUT);
	PullUpDnControl(pin_b, PULLUPDN_UP);
	WiringGpioISR(pin_a, INTERRUPT_EDGE_BOTH, updateEncodersForPerformance);

	return newencoder;
};



//  Test for optimal CUP performance
int TestPerformance(int argc, char *argv[])
{
	int pina = 40;//15
	int pinb = 38;//13
	
	cout << "Test Encoders - Performance sample update on pins " << pina << " and " << pinb << endl;
	
	encoder* testEncoder = setupEncoderOptimalCpu(pina, pinb);
	
	while (true)
	{
		char key =  ' ';
		key = getchar();
		cout << '\b';
		if (key == 'q')
		{
			return 0;
		}
		else if (key == 'r')
		{
			testEncoder->value = 0;
			cout << "Reset Encoder " << testEncoder->value << endl;
		}
		
		usleep(250000);
	}
}


//  Test a single pin
int TestSinglePin(int argc, char *argv[])
{
	int pina = 40;//15
	
	cout << "Test Encoders - One encoder signal on pin " << pina << endl;
	
	encoder* testEncoder = setupEncoderTestOneSignal(pina);
	
	while (true)
	{
		char key =  ' ';
		key = getchar();
		cout << '\b';
		if (key == 'q')
		{
			return 0;
		}
		else if (key == 'r')
		{
			testEncoder->value = 0;
			cout << "Reset Encoder " << testEncoder->value << endl;
		}
		
		
		usleep(250000);
	}
	
	return 0;
}


//  Rotary Encoder test function using original sample code
int TestOriginal(int argc, char *argv[])
{
	int pina = 40;//15
	int pinb = 38;//13
	
	cout << "Test Encoders - Original Sample - Two signal encoder on pins " << pina << " and " << pinb << endl;
	
	encoder* testEncoder = setupEncoderOriginal(pina, pinb);
	
	while (true)
	{
		char key =  ' ';
		key = getchar();
		cout << '\b';
		if (key == 'q')
		{		
			return 0;
		}
		else if (key == 'r')
		{
			testEncoder->value = 0;
			cout << "Reset Encoder " << testEncoder->value << endl;
		}

		usleep(250000);
	}
	
	return 0;
}


//  Rotary Encoder test function
//  play with the functions in this file to see how different encoder code works
int EncoderSampleMain(int argc, char *argv[])
{
	
	//  original sample code
	return TestOriginal(argc, argv);
	
	//  test single pin for count and edge observations
	//return TestSinglePin(argc, argv);
	
	//return TestPerformance(argc, argv);
}