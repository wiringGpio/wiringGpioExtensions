
//  System Includes
#include <map>
#include <iostream>

#include <wiringGpioLogging.h>

//  wiring* Includes (Platform dependant)
#ifdef JETSON
#include <wiringJet.h>
#include <wiringJetI2C.h>
#include <wiringJetSPI.h>
#else
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#endif

//  wiringGpio Includes
#include "wiringGpioExtensions.h"



using namespace std;

//  TODO - move this to its own file
map<int, int> SoftPwmThreads;


//  Setup
//
#pragma region Setup

int WiringGpioSetup()
{
	return wiringPiSetup();
}
	

int WiringGpioSetupGpio()
{
	return wiringPiSetupGpio();
}
	

int WiringGpioSetupSys()
{
	return wiringPiSetupSys();
}


int WiringGpioSetupPhys()
{
	// Test logging callback for all log levels
	Log(LogLevelTrace,"WiringGpioExtensions.cpp", "WiringGpioSetupPhys", "LogLevelTrace");
//	Log(LogLevelDebug,"WiringGpioExtensions.cpp", "WiringGpioSetupPhys", "LogLevelDebug");
//	Log(LogLevelInfo,"WiringGpioExtensions.cpp", "WiringGpioSetupPhys", "LogLevelInfo");
//	Log(LogLevelWarn,"WiringGpioExtensions.cpp", "WiringGpioSetupPhys", "LogLevelWarn");
//	Log(LogLevelError,"WiringGpioExtensions.cpp", "WiringGpioSetupPhys", "LogLevelError");
//	Log(LogLevelFatal,"WiringGpioExtensions.cpp", "WiringGpioSetupPhys", "LogLevelFatal");
	
	return wiringPiSetupPhys();
}


void WiringGpioTerminate()
{
	//  shut down soft pwm threads
	map<int, int>::iterator itPwmThreads;
	for (itPwmThreads = SoftPwmThreads.begin(); itPwmThreads != SoftPwmThreads.end(); ++itPwmThreads)
	{
		softPwmStop(itPwmThreads->second);
	}  
	//  clean up the map
	SoftPwmThreads.clear();
	
	StepperShutDown();
	
	SevenSegDisplayShutDown();
	
	RotaryEncoderShutDown();
	
	MotorWithRotaryEncoderShutDown();
	
	wiringPiTerminate();
}

#pragma endregion



//  GPIO Pin Control
//
#pragma region GPIO

void PinModeAlt(int pin, int mode)
{
	pinModeAlt(pin, mode);
}

void PinMode(int pin, int mode)
{
	pinMode(pin, mode);
}

void PullUpDnControl(int pin, int pud)
{
	pullUpDnControl(pin, pud);
}

int  DigitalRead(int pin)
{
	return digitalRead(pin);
}

void DigitalWrite(int pin, int value)
{
	digitalWrite(pin, value);
}

int  AnalogRead(int pin)
{
	return analogRead(pin);
}

void AnalogWrite(int pin, int value)
{
	analogWrite(pin, value);
}

void PwmWrite(int pin, int value)
{
	pwmWrite(pin, value);
}

extern void PwmWriteUnit(int pin, float value)
{
	pwmWriteUnit(pin, value);
}

void GpioClockSet(int pin, int freq)
{
	gpioClockSet(pin, freq);
}
	
void PwmSetMode(int mode)
{
	pwmSetMode(mode);
}	

void PwmSetClock(int divisor)
{
	pwmSetClock(divisor);
}

extern void PwmSetFrequency(int pin, float frequency)
{
	pwmSetFrequency(pin, frequency);
}
	
extern void PwmSetRange(unsigned int range)
{
	pwmSetRange(range);
}

int PwmGetRange(int pin)
{
	return pwmGetRange(pin);
}

#pragma endregion


//  Software PWM
//
#pragma region SoftPwm

int  SoftPwmCreate(int pin, int value, int range)	
{
	//  see if you have created this one already
	map<int, int>::iterator it = SoftPwmThreads.find(pin);
	if (it != SoftPwmThreads.end())
	{
		//  shut down the old one
		SoftPwmStop(pin);
		SoftPwmThreads.erase(pin);
	}
	
	// create the new one
	int ret = softPwmCreate(pin, value, range);
	//  remember it
	SoftPwmThreads[pin] = pin;
	
	return ret;
}

void SoftPwmWrite(int pin, int value)	
{
	softPwmWrite(pin, value);
}

void SoftPwmStop(int pin)	
{
	//  clean up the map
	map<int, int>::iterator it = SoftPwmThreads.find(pin);
	if (it != SoftPwmThreads.end())
		SoftPwmThreads.erase(pin);
	
	softPwmStop(pin);
}

#pragma endregion


//  Interrupts
//
#pragma region Interrupts

extern int WaitForInterrupt(int pin, int mS)
{
	return waitForInterrupt(pin, mS);
}	
	
int WiringGpioISR(int pin, int mode, void(*function)())
{
	return wiringPiISR(pin, mode, function);
}
	
#pragma endregion


//  SPI
//
#pragma region SPI

int WiringGpioSPISetup(int channel, int speed)
{
	return wiringPiSPISetup(channel, speed);
}


int WiringGpioSPIDataRW(int channel, unsigned char* data, int len)
{
	return wiringPiSPIDataRW(channel, data, len);
}

#pragma endregion


//  I2C
//
#pragma region I2C

int WiringGpioI2CSetup(int bus, int devId)
{
	return wiringPiI2CSetup(bus, devId);
}

int WiringGpioI2CWrite(int fd, int data)
{
	return wiringPiI2CWrite(fd, data);
}

int WiringGpioI2CRead(int fd)
{
	return wiringPiI2CRead(fd);
}

int WiringPiI2CWriteReg8(int fd, int reg, int data)
{
	return wiringPiI2CWriteReg8(fd, reg, data);
}

int WiringPiI2CWriteReg16(int fd, int reg, int data)
{
	return wiringPiI2CWriteReg16(fd, reg, data);
}

int WiringPiI2CReadReg8(int fd, int reg)
{
	return wiringPiI2CReadReg8(fd, reg);
}

int WiringPiI2CReadReg16(int fd, int reg)
{
	return wiringPiI2CReadReg16(fd, reg);
}

#pragma endregion 


//  Extension Node Management
//
#pragma region ExtensionNodeManagement

int WiringGpioGetPinBaseForNode(int pin)
{
	return wiringPiGetPinBaseForNode(pin);
}
	
int WiringGpioGetFileDescriptorForNode(int pin)
{
	return wiringPiGetFileDescriptorForNode(pin);
}

#pragma endregion

//  Logging
//
#pragma region Logging

void WiringGpioSetLoggingCallback(wiringGpioLoggingCallback function)
{
	//  set our callback function
	wiringPiSetLoggingCallback(function);
}

void wiringGpioSetLoggingLevel(wiringGpioLogLevel level)
{
	wiringPiSetLoggingLevel(level);
}

#pragma endregion