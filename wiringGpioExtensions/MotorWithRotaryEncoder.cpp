#include <iostream>
#include <iomanip>
#include <math.h>
#include <unistd.h>

#include "wiringGpioExtensions.h"
#include "MotorWithRotaryEncoder.h"
#include "RotaryEncoder.h"



// https://gist.github.com/thirdwing/79a4b3949a30c69b6403

using namespace std;

int count = 0;

MotorWithRotaryEncoder::MotorWithRotaryEncoder(const int bridgeIn1, const int bridgeIn2, const int bridgePwm, const int bridgePwmRange, RotaryEncoder* encoder)
{
	TurnToEnabled = false;
	HoldAtEnabled = false;
	
	BridgePinA = bridgeIn1;
	BridgePinB = bridgeIn2;
	BridgePinPwm = bridgePwm;
	BridgePwmRange = bridgePwmRange;
	SetUsefulPowerRange(0.0, 1.0);
	
	PinMode(bridgePwmRange, PWM_OUTPUT);
	PinMode(bridgeIn1, OUTPUT);
	PinMode(bridgeIn2, OUTPUT);
	SetRunForward();
	SetPwmPower(0.0);
	
	Encoder = encoder;
	Encoder->RegisterUpdatedClassFnCallback([this](int i) {return this->EncoderUpdated(i);});
	
	LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "MotorWithRotaryEncoder", "Setup Motor %d. Pin 1 %d. Pin 2 %d.", BridgePinPwm, BridgePinA, BridgePinB);
}


void MotorWithRotaryEncoder::SetUsefulPowerRange(double minPower, double maxPower)
{
	PowerMinUsefulPwm = minPower * BridgePwmRange;
	PowerMaxUsefulPwm = maxPower * BridgePwmRange;
	
	LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "SetUsefulPowerRange", "Motor %d. Set useful power range %.3lf %.3lf.", BridgePinPwm, minPower, maxPower);
}


void MotorWithRotaryEncoder::ResetCount(int setCount)
{ 
	if (Encoder != NULL) 
	{
		Encoder->ResetCounter(setCount);
		LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "ResetCount", "Motor %d. Reset count to %d", BridgePinPwm, setCount);
	}
	else
	{
		LogFormatted(LogLevelError, "MotorWithRotaryEncoder", "ResetCount", "Error Motor %d: Encoder is null.", BridgePinPwm);
	}
}

	
int MotorWithRotaryEncoder::GetCount()
{
	if (Encoder != NULL) 
	{
		return Encoder->GetCount();
	}
	else
	{
		LogFormatted(LogLevelError, "MotorWithRotaryEncoder", "GetCount", "Error Motor %d: Encoder is null.", BridgePinPwm);
	}
	
	return 0;
}


int MotorWithRotaryEncoder::GetTick()
{
	if (Encoder != NULL) 
	{
		return Encoder->GetTickCount();
	}
	else
	{
		LogFormatted(LogLevelError, "MotorWithRotaryEncoder", "GetTick", "Error Motor %d: Encoder is null.", BridgePinPwm);
	}
	
	return 0;
}
	

double MotorWithRotaryEncoder::GetCircle()
{
	if (Encoder != NULL) 
	{
		return Encoder->GetCircle();
	}
	else
	{
		LogFormatted(LogLevelError, "MotorWithRotaryEncoder", "GetCircle", "Error Motor %d: Encoder is null.", BridgePinPwm);
	}
	return 0;
}


double MotorWithRotaryEncoder::GetRpm()
{
	if (Encoder != NULL) 
	{
		return Encoder->GetRpm();
	}
	else
	{
		LogFormatted(LogLevelError, "MotorWithRotaryEncoder", "GetRpm", "Error Motor %d: Encoder is null.", BridgePinPwm);
	}
	return 0.0;
}
	

double MotorWithRotaryEncoder::GetTickFrequency()
{
	if (Encoder != NULL) 
	{
		return Encoder->GetTickFrequency();
	}
	else
	{
		LogFormatted(LogLevelError, "MotorWithRotaryEncoder", "GetTickFrequency", "Error Motor %d: Encoder is null.", BridgePinPwm);
	}
	return 0.0;
}


double MotorWithRotaryEncoder::GetCountFrequency()
{
	if (Encoder != NULL) 
	{
		return Encoder->GetCountFrequency();
	}
	else
	{
		LogFormatted(LogLevelError, "MotorWithRotaryEncoder", "GetCountFrequency", "Error Motor %d: Encoder is null.", BridgePinPwm);
	}
	return 0.0;
}


void MotorWithRotaryEncoder::SetRunForward()
{	
	DigitalWrite(BridgePinA, 1);
	DigitalWrite(BridgePinB, 0);
	
	LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "SetRunForward", "Motor %d. Pin %d on. Pin %d off. ", BridgePinPwm, BridgePinA, BridgePinB);
}



void MotorWithRotaryEncoder::SetRunBackward()
{	
	DigitalWrite(BridgePinA, 0);
	DigitalWrite(BridgePinB, 1);
	
	LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "SetRunBackward", "Motor %d. Pin %d off. Pin %d on. ", BridgePinPwm, BridgePinA, BridgePinB);
}



void MotorWithRotaryEncoder::BrakeMotor(double power = 1.0)
{
	DigitalWrite(BridgePinA, 1);
	DigitalWrite(BridgePinB, 1);
	LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "BrakeMotor", "Motor %d. Pin %d on. Pin %d on. ", BridgePinPwm, BridgePinA, BridgePinB);
	SetPwmPower(1.0);
}


void MotorWithRotaryEncoder::SetPwmPower(double power)
{
	if (fabs(power) < 0.01)
	{
		CurrentPower = 0.0;
		PwmWrite(BridgePinPwm, 0);
		LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "SetPwmPower", "Motor %d. PWM off. ", BridgePinPwm);
	}
	else
	{
		CurrentPower = fabs(power);
		int pwm = PowerMinUsefulPwm + (PowerMaxUsefulPwm - PowerMinUsefulPwm)*fabs(power);
		PwmWrite(BridgePinPwm, pwm);
		LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "SetPwmPower", "Motor %d. Set PWM %d. ", BridgePinPwm, pwm);
	}
}


void MotorWithRotaryEncoder::Run(double power)
{
	TurnToEnabled = false;
	HoldAtEnabled = false;
	
	if (power > 0.0)
	{	
		SetRunForward();
	}
	else
	{
		SetRunBackward();
	}
	
	SetPwmPower(power);
}
	
void MotorWithRotaryEncoder::Stop()
{
	HoldAtEnabled = false;
	TurnToEnabled = false;
	SetRunForward();
	SetPwmPower(0);
}
	
void MotorWithRotaryEncoder::TurnBy(double rotations, double power)
{	
	HoldAtEnabled = false;
	HoldAtReached = false;
	TurnToEnabled = true;
	TurnToStartTick = Encoder->GetTickCount();
	TurnToTargetTick = TurnToStartTick + (int)(rotations *  Encoder->GetTicksPerRevolution());

	if (abs(TurnToStartTick - TurnToTargetTick) <= 3)
		return;
	
	LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "TurnBy", "Motor %d. Turn by %d to %d. ", BridgePinPwm, (TurnToTargetTick - TurnToStartTick), TurnToTargetTick);
	
	SetPwmPower(0.0);
	if (rotations > 0.0)
		SetRunForward();
	else
		SetRunBackward();
	
	SetPwmPower(power);
}



void MotorWithRotaryEncoder::TurnTo(double circle, double power)
{	
	SetPower = power;
	HoldAtEnabled = true;
	HoldAtReached = false;
	TurnToEnabled = true;
	TurnToStartTick = Encoder->GetTickCount();
	TurnToTargetTick = (int)(circle *  Encoder->GetTicksPerRevolution());
		
	if (abs(TurnToStartTick - TurnToTargetTick) <= 3)
		return;
	
	LogFormatted(LogLevelInfo, "MotorWithRotaryEncoder", "TurnTo", "Motor %d. Turn %d to setpoint %d. ", BridgePinPwm, (TurnToTargetTick - TurnToStartTick), TurnToTargetTick);

	SetPwmPower(0.0);
	if ((TurnToTargetTick - TurnToStartTick) > 0)
		SetRunForward();
	else
		SetRunBackward();
	
	SetPwmPower(power);
}





void MotorWithRotaryEncoder::TurnAndHold(double circle, double power)
{
	SetPwmPower(0.0);
	
	//	HoldAtEnabled = false;
	//	TurnToEnabled = true;
	//	TurnToStartTick = Encoder->GetTickCount();
	//	
	//	TurnToTargetTick = TurnToStartTick + (int)(rotations *  Encoder->GetTicksPerRevolution());
	//	
	//	if (rotations > 0.0)
	//		SetRunForward();
	//	else
	//		SetRunBackward();
	//	
	//	SetPwmPower(power);
}



//  Encoder Updated Callback
//  will get notified every time the associated encoder has update
void MotorWithRotaryEncoder::EncoderUpdated(int i)
{	
	LogFormatted(LogLevelTrace, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Tick %d. ", BridgePinPwm, i);

	if (TurnToEnabled)
	{	
		bool runningForwards = (TurnToTargetTick - TurnToStartTick) > 0;
		
		if (TurnToTargetTick == i)
		{
			LogFormatted(LogLevelDebug, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Turn to tick triggered %d. ", BridgePinPwm, TurnToTargetTick);

			if (HoldAtEnabled)
			{
				HoldAtReached = true;
				BrakeMotor(1.0);
			}
			else
			{
				SetPwmPower(0.0);
			}
		}
		else if (runningForwards)
		{
			if ((TurnToTargetTick - i)  == -4) 
			{
				LogFormatted(LogLevelDebug, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Turn to tick went past %d going forwards to setpoint %d.", BridgePinPwm, i - TurnToTargetTick, TurnToTargetTick);

				SetRunBackward();
				SetPwmPower(0.1); 
			}
//			else if ((TurnToTargetTick - i) == 10)
//			{
//				Log("MotorWithRotaryEncoder", "EncoderUpdated", format("Motor %d. Turn to target tick %d triggered brake at %d.", BridgePinPwm, TurnToTargetTick, TurnToTargetTick - i), LogLevelTrace);
//
//				BrakeMotor(1.0);
//			}
			else if (((TurnToTargetTick - i) == 25) || ((TurnToTargetTick - i) == 15) || ((TurnToTargetTick - i) == 10))
			{
				if (fabs(CurrentPower) > 0.02)
				{
					LogFormatted(LogLevelDebug, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Turn to tick slow down %d triggered at %d. Set Power %d", BridgePinPwm, abs(TurnToTargetTick - i), i, (int)(CurrentPower * 100 / 2));
					SetPwmPower(CurrentPower / 2.0);
				}
			}
		}
		else 
		{
			if ((TurnToTargetTick - i) == 4)
			{
				LogFormatted(LogLevelDebug, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Turn to tick went past %d going backwards to setpoint %d.", BridgePinPwm, i - TurnToTargetTick, TurnToTargetTick);
				SetRunForward();
				SetPwmPower(0.1);
			}
//			else if ((TurnToTargetTick - i) == -10)
//			{
//				Log("MotorWithRotaryEncoder", "EncoderUpdated", format("Motor %d. Turn to target tick %d triggered brake at %d.", BridgePinPwm, TurnToTargetTick, TurnToTargetTick - i), LogLevelTrace);
//				BrakeMotor(1.0);
//			}
			else if (((TurnToTargetTick - i) == -20) || ((TurnToTargetTick - i) == -15) || ((TurnToTargetTick - i) == -10))
			{
				if (fabs(CurrentPower) > 0.02)
				{
					LogFormatted(LogLevelDebug, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Turn to tick slow down %d triggered at %d. Set Power %d", BridgePinPwm, abs(TurnToTargetTick - i), i, (int)(CurrentPower * 100 / 2)); 
					SetPwmPower(CurrentPower / 2.0);
				}
			}
		}
		
		
		if (HoldAtReached)
		{
			if (abs(i - TurnToTargetTick) > 6)
			{
				TurnToStartTick = i;
				if (i > TurnToTargetTick)
				{
					LogFormatted(LogLevelDebug, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Hold At Forward - reverse motor from %d to %d", BridgePinPwm, i, TurnToTargetTick); 
					SetPwmPower(.1);	//  TODO - motor min useful power ?
					SetRunBackward();
				}
				else
				{
					LogFormatted(LogLevelDebug, "MotorWithRotaryEncoder", "EncoderUpdated", "Motor %d. Hold At Behind - forward motor from %d to %d", BridgePinPwm, i, TurnToTargetTick); 
					SetPwmPower(.1); 
					SetRunForward();
				}
			}
		}
	}
}
	