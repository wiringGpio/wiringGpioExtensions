#include <map>

#include "wiringGpioExtensions.h"
#include "StepDriver.h"


using namespace std;

//  Map of stepper motor index to steper motor object
map<int, StepDriver*> StepperMotorMap;


StepDriver* FindStepperMotor(int index)
{
	//  Find the step driver
	map<int, StepDriver*>::iterator it = StepperMotorMap.find(index);
	if (it != StepperMotorMap.end())
	{
		return it->second;
	}
	return NULL;
}

//  Software Stepper Motor Driver
//
#pragma region StepperMotor

int StepperCreateFromXml(const char* sequenceElement, const char* pinsElement)
{
	//  get the next available index number
	int index = 0;
	while (true)
	{
		map<int, StepDriver*>::iterator it = StepperMotorMap.find(index);
		if (it == StepperMotorMap.end())
		{
			break;
		}
		index++;
	}
	
	//  Create a stepper driver
	StepDriver* newDriver = new StepDriver();
	
	//  Step driver load the xml definition
	if(!newDriver->LoadFromXml(sequenceElement, pinsElement))
	{
		delete newDriver;
		return -1;
	}
	
	//  add this new driver to the map
	StepperMotorMap[index] = newDriver;
	
	//  start the thread
	newDriver->Start();
	
	//  return index of new driver
	return index;
	
}


void StepperRemove(int index)
{
	StepDriver* findDriver = FindStepperMotor(index);
	if (findDriver != NULL)
	{
		findDriver->Cancel();
		StepperMotorMap.erase(index);
		delete findDriver;
	}
}
	

void StepperSetDelay(int index, float delay)
{
	StepDriver* findDriver = FindStepperMotor(index);
	if (findDriver != NULL)
		findDriver->SetDelay(delay);
}
	

void StepperStep(int index, int numberSteps)
{
	StepDriver* findDriver = FindStepperMotor(index);
	if (findDriver != NULL)
		findDriver->AddStepCommand(numberSteps);
}
	

void StepperSpin(int index, int direction)
{
	StepDriver* findDriver = FindStepperMotor(index);
	if (findDriver != NULL)
		findDriver->AddSpinCommand(direction);
}


void StepperSetSpeed(int index, float percent)
{
	StepDriver* findDriver = FindStepperMotor(index);
	if (findDriver != NULL)
		findDriver->SetSpeed(percent);
}


void StepperSetSequenceInterval(int index, int interval)
{
	StepDriver* findDriver = FindStepperMotor(index);
	if (findDriver != NULL)
		findDriver->SetSequenceInterval(interval);
}


void StepperStop(int index)
{
	StepDriver* findDriver = FindStepperMotor(index);
	if (findDriver != NULL)
		findDriver->Stop();
}
	

int StepperGetTachoCount(int index)
{
	// TODO
	return 0;
}
	

void StepperResetTachoCount(int index)
{
	//  TODO
	
}


void ShutDownStepperMotors()
{
	//  shut down our step driver threads
	map<int, StepDriver*>::iterator itStepDrivers;
	for (itStepDrivers = StepperMotorMap.begin(); itStepDrivers != StepperMotorMap.end(); ++itStepDrivers)
	{
		//  shut down the thread
		itStepDrivers->second->Cancel();
		//  delete the object
		delete itStepDrivers->second;
	}
	//  clean up the map
	StepperMotorMap.clear();
}

#pragma endregion
