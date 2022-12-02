#include <map>
#include <string.h>
#include "wiringGpioExtensions.h"
#include "MotorWithRotaryEncoder.h"


#include "StringExtensionMethods.h"


using namespace std;

RotaryEncoder* RotaryEncoderFind(int index);


//  Map of motor with encoder index to motor with encoder object
map<int, MotorWithRotaryEncoder*> MotorWithRotaryEncoderMap;


MotorWithRotaryEncoder* MotorWithRotaryEncoderFind(int index)
{
	map<int, MotorWithRotaryEncoder*>::iterator it = MotorWithRotaryEncoderMap.find(index);
	if (it != MotorWithRotaryEncoderMap.end())
	{
		return it->second;
	}
	return NULL;
}


void MotorWithRotaryEncoderShutDown()
{
	//  delete all motors with rotary encoder
	map<int, MotorWithRotaryEncoder*>::iterator itMotorsWithEncoders;
	for (itMotorsWithEncoders = MotorWithRotaryEncoderMap.begin(); itMotorsWithEncoders != MotorWithRotaryEncoderMap.end(); ++itMotorsWithEncoders)
	{
		delete itMotorsWithEncoders->second;
	}
	MotorWithRotaryEncoderMap.clear();
}


//  Motor with Rotary Encoder
//
#pragma region MotorWithRotaryEncoder

int MotorWithRotaryEncoderCreate(const int bridgeIn1, const int bridgeIn2, const int bridgePwm, const int encoderA, const int encoderB, const int encoderIndex, const int countsPerRevolution, EncoderUpdatedCallback callback)
{
	int newEncoder = RotaryEncoderCreate(encoderA, encoderB, encoderIndex, countsPerRevolution,callback);
	
	if (newEncoder < 0)
		return newEncoder;	//  error creating the encoder (too many created already)
	
	//  get the next available index number
	int index = 0;
	while (true)
	{
		map<int, MotorWithRotaryEncoder*>::iterator it = MotorWithRotaryEncoderMap.find(index);
		if (it == MotorWithRotaryEncoderMap.end())
		{
			break;
		}
		index++;
	}
	
	MotorWithRotaryEncoder* newMotorWithEncoder = new MotorWithRotaryEncoder(bridgeIn1, bridgeIn2, bridgePwm, PwmGetRange(bridgePwm), RotaryEncoderFind(newEncoder));
	MotorWithRotaryEncoderMap[index] = newMotorWithEncoder;
	return index;
}


void MotorWithRotaryEncoderRemove(const int index)
{
	MotorWithRotaryEncoder* findMotor = MotorWithRotaryEncoderFind(index);
	if (findMotor != NULL)
	{
		findMotor->Stop();
		MotorWithRotaryEncoderMap.erase(index);
		delete findMotor;
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderRemove", "Error: Unable to find motor %d.", index);
	}
}


void MotorWithRotaryEncoderSetUsefulPowerRange(int index, double minPower, double maxPower)
{
	MotorWithRotaryEncoder* findMotor = MotorWithRotaryEncoderFind(index);
	if (findMotor != NULL)
	{
		findMotor->SetUsefulPowerRange(minPower, maxPower);
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderSetUsefulPowerRange", "Error: Unable to find motor %d.", index);
	}
}


void MotorWithRotaryEncoderResetCount(const int index, int setCount)
{
	MotorWithRotaryEncoder* findMotor = MotorWithRotaryEncoderFind(index);
	if (findMotor != NULL)
	{
		findMotor->ResetCount(setCount);
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderResetCount", "Error: Unable to find motor %d.", index);
	}
}


	
int MotorWithRotaryEncoderGetCount(const int index)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		return motor->GetCount();
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderGetCount", "Error: Unable to find motor %d.", index);
	}
	
	return 0;
}


	
int MotorWithRotaryEncoderGetTick(const int index)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		return motor->GetTick();
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderGetTick", "Error: Unable to find motor %d.", index);
	}
	
	return 0;
}



double MotorWithRotaryEncoderGetCircle(const int index)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		return motor->GetCircle();
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderGetCircle", "Error: Unable to find motor %d.", index);
	}
	
	return 0;
}


double  MotorWithRotaryEncoderGetRpm(int index)
	
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		return motor->GetRpm();
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderGetRpm", "Error: Unable to find motor %d.", index);
	}
	
	return 0;
}


double  MotorWithRotaryEncoderGetTickFrequency(int index)
	
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		return motor->GetTickFrequency();
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderGetTickFrequency", "Error: Unable to find motor %d.", index);
	}
	
	return 0;
}


double  MotorWithRotaryEncoderGetFrequency(int index)
	
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		return motor->GetCountFrequency();
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderGetFrequency", "Error: Unable to find motor %d.", index);
	}
	
	return 0;
}


void MotorWithRotaryEncoderRun(const int index, double power)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		motor->Run(power);
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderRun", "Error: Unable to find motor %d.", index);
	}
}


void MotorWithRotaryEncoderStop(const int index)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		motor->Stop();
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderStop", "Error: Unable to find motor %d.", index);
	}
}


void MotorWithRotaryEncoderBrake(const int index, double power)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		motor->BrakeMotor(power);
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderBrake", "Error: Unable to find motor %d.", index);
	}
}


void MotorWithRotaryEncoderTurnBy(const int index, double rotations, double power)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		motor->TurnBy(rotations, power);
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderTurnBy", "Error: Unable to find motor %d.", index);
	}
}


void MotorWithRotaryEncoderHoldAt(const int index, double circle, double power)
{
	MotorWithRotaryEncoder* motor = MotorWithRotaryEncoderFind(index); 
	if (motor != NULL)
	{
		motor->TurnTo(circle, power);
	}
	else
	{
		LogFormatted(LogLevelError, "wiringGpioExtension_RotaryEncoder.cpp", "MotorWithRotaryEncoderHoldAt", "Error: Unable to find motor %d.", index);
	}
}


#pragma endregion
