#pragma once

class RotaryEncoder;


class MotorWithRotaryEncoder
{
public:
	MotorWithRotaryEncoder();
	
	MotorWithRotaryEncoder(const int bridgeIn1, const int bridgeIn2, const int bridgePwm, const int pwmRange, RotaryEncoder* encoder);
	
	void SetUsefulPowerRange(double minPower, double maxPower);
	
	void ResetCount(int setCount = 0);
	
	int GetCount();
	
	int GetTick();
	
	double GetCircle();

	double GetRpm();
	
	double GetTickFrequency();

	double GetCountFrequency();
	
	
	void Run(double power);
	
	void Stop();
	
	void BrakeMotor(double power);
	
	void TurnBy(double rotations, double power);
	
	void TurnTo(double rotations, double power);
	
	void TurnAndHold(double circle, double power);
	
	
protected:
	
	int BridgePinA;
	int BridgePinB;
	int BridgePinPwm;
	int BridgePwmRange;
	
	int PowerMinUsefulPwm;
	int PowerMaxUsefulPwm;
	
	double CurrentPower;
	double SetPower;
	
	bool TurnToEnabled;
	int TurnToTargetTick;
	int TurnToStartTick;
	
	bool HoldAtEnabled;
	bool HoldAtReached;
	
	void SetRunForward();
	void SetRunBackward();
	void SetPwmPower(double power);
	
	
	RotaryEncoder* Encoder;
	void EncoderUpdated(int count);
	
	
	
};
