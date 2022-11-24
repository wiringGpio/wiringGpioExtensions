#include <map>

#include "wiringGpioExtensions.h"
#include <pca9685.h>


using namespace std;


int Pca9685GetFrequencyForPin(int pin)
{
	//  TODO
	return -1;
}




//  PCA9865 Driver
//
#pragma region PCS9685


int Pca9685Setup(int bus, int pinBase, int i2cAddress, float freq)
{
	return pca9685Setup(bus, pinBase, i2cAddress, freq);
}


void Pca9685PWMFreq(int fd, float freq)
{
	pca9685PWMFreq(fd, freq);
}


void Pca9685PWMReset(int fd)
{
	pca9685PWMReset(fd);
}


void Pca9685PWMWrite(int fd, int pin, int on, int off)
{
	pca9685PWMWrite(fd, pin, on, off);
}


void Pca9685PWMRead(int fd, int pin, int *on, int *off)
{
	pca9685PWMRead(fd, pin, on, off);
}


void Pca9685FullOn(int fd, int pin, int tf)
{
	pca9685FullOn(fd, pin, tf);
}


void Pca9685FullOff(int fd, int pin, int tf)
{
	pca9685FullOff(fd, pin, tf);
}


#pragma endregion