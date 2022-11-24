//  Wiring GPIO Extensions Library
//  Functions to wrap up wiringPi / wiringJet GPIO libraries into a single interface,
//    and to provide extended features for using GPIO in some common applications, such as stepper motors, motors with rotary encoders, seven segment displays, ...
//  
//
//  By http://LittleBytesOfPi.com 
//  


#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
	
//  Defines
//	These defines map out the equivelent values in wiringPi / wiringJet
//
#pragma region Defines
	
//  Pin Modes
#define	PINMODE_INPUT				(0)
#define	PINMODE_OUTPUT				(1)
#define	PINMODE_PWM_OUTPUT			(2)
#define	PINMODE_GPIO_CLOCK			(3)
#define	PINMODE_SOFT_PWM_OUTPUT		(4)
#define	PINMODE_SOFT_TONE_OUTPUT	(5)
#define	PINMODE_PWM_TONE_OUTPUT		(6)
	
//  Pin Values
#define	PINVALUE_LOW	(0)
#define	PINVALUE_HIGH	(1)
	
//  Pull Up / Down resistor values
#define	PULLUPDN_OFF	(0)
#define	PULLUPDN_DOWN	(1)
#define	PULLUPDN_UP		(2)

//  Interrupt triggers
//
#ifdef JETSON
//  wiringJet definition follows GPIO defines
#define	INTERRUPT_EDGE_SETUP		(0)
#define	INTERRUPT_EDGE_FALLING		(2)
#define	INTERRUPT_EDGE_RISING		(1)
#define	INTERRUPT_EDGE_BOTH			(3)
#else
//  wiringPi definition swaps falling and rising
#define	INTERRUPT_EDGE_SETUP		(0)
#define	INTERRUPT_EDGE_FALLING		(1)
#define	INTERRUPT_EDGE_RISING		(2)
#define	INTERRUPT_EDGE_BOTH			(3)
#endif

#pragma endregion	//Defines
	
	
//  Setup
//
#pragma region Setup
	
	//  Setup, callthe setup function for your pin numbering scheme
	//
	extern int WiringGpioSetup();
	
	extern int WiringGpioSetupGpio();

	extern int WiringGpioSetupSys();

	extern int WiringGpioSetupPhys();
	
	//  Tear Down
	//  stops extension objects, waiting for all threads to stop
	//  stops any softPwm threads still running
	extern void WiringGpioTerminate();
	
	//  Extension Node Management
	extern int GetPinBaseForNode(int pin);
	
#pragma endregion
	
	
//  Pin Control
//
#pragma region GPIO
	
	//  Node management
	extern int WiringGpioGetPinBaseForNode(int pin);
	
	extern int WiringGpioGetFileDescriptorForNode(int pin);
	
	
	//  Basic GPIO
	extern void PinMode(int pin, int mode);

	extern void PinModeAlt(int pin, int mode);

	extern void DigitalWrite(int pin, int value);

	extern void DigitalWriteByte(int value);

	extern int  DigitalRead(int pin);
	
	extern void AnalogWrite(int pin, int value);
	
	extern int  AnalogRead(int pin);

	extern void PullUpDnControl(int pin, int pud);

	//  PWM
	extern void PwmSetMode(int mode);
	
	extern void PwmWrite(int pin, int value);	

	extern int PwmGetRange(int pin);

	
	//  Software PWM
	extern int  SoftPwmCreate(int pin, int value, int range);

	extern void SoftPwmWrite(int pin, int value);

	extern void SoftPwmStop(int pin);
	
#pragma endregion
	
	

//  Interrupts
//
#pragma region Interrupts
	
	extern int WiringGpioISR(int pin, int mode, void(*function)());
	
#pragma endregion
	
	

	
//  SPI
//
#pragma region SPI
	
	extern int WiringGpioSPISetup(int channel, int speed);

	extern int WiringGpioSPIDataRW(int channel, unsigned char* data, int len); 
	
#pragma endregion
	
	
//  I2C
//
#pragma region I2C
	
	extern int WiringGpioI2CSetup(int bus, int devId);

	extern int WiringGpioI2CRead(int fd);

	extern int WiringGpioI2CWrite(int fd, int data);

	extern int WiringGpioI2CWriteReg8(int fd, int reg, int data);

	extern int WiringGpioI2CWriteReg16(int fd, int reg, int data);

	extern int WiringGpioI2CReadReg8(int fd, int reg);

	extern int WiringGpioI2CReadReg16(int fd, int reg);
	
#pragma endregion
	
	
//  mcp23***.h
//
#pragma region MCP23xxx
	
	extern int Mcp23017Setup(int bus, int pinBase, int address);

	extern int Mcp23008Setup(int bus, int pinBase, int address);

#pragma endregion
	
	
//  mcp3004.h
//
#pragma region MCP300x
	
	extern int Mcp3004Setup(int pinBase, int spiChannel);
	
	extern int Mcp3008Setup(int pinBase, int spiChannel);

#pragma endregion

	
//  PCA9865 Driver
//
#pragma region PCS9685
	
	extern int Pca9685Setup(int bus, int pinBase, int i2cAddress, float freq);

	extern int Pca9685GetFrequencyForPin(int pin);

	extern int Pca9685GetFileDescriptorForPcaPin(int pin);

	//  Internal PCA control functions
	extern void Pca9685PWMFreq(int fd, float freq);
	extern void Pca9685PWMReset(int fd);
	extern void Pca9685PWMWrite(int fd, int pin, int on, int off);
	extern void Pca9685PWMRead(int fd, int pin, int *on, int *off);
	extern void Pca9685FullOn(int fd, int pin, int tf);
	extern void Pca9685FullOff(int fd, int pin, int tf);
	
#pragma endregion


//  Software Stepper Motor Driver
//
#pragma region StepperMotor
	
	//  Create a software stepper motor controller
	//  input:  XML Element describing the stepper sequence
	//  input:  XML element describing the stepper pins
	//  output: handle to the stepper controller created, or -1 for error
	extern int StepperCreateFromXml(const char* sequenceElement, const char* pinsElement);

	//  Shut down a stepper motor driver, and delete its objects
	//  input:  index number
	extern void StepperRemove(int index);

	//  Set the delay between sequence items
	//  input:	index number
	//  input:	delay, in milliseconds, fractional milliseconds are supported
	extern void StepperSetDelay(int index, float delay);

	//  Step
	//  do the numberSteps of steps, each step runs the complete sequence
	//  input:  index number
	//  input:  the number of steps to do, positive or negative for direction
	extern void StepperStep(int index, int numberSteps);

	//  Spin continuously until stop is received
	//  input:	index number
	//	input:	direction, positive or negative 1
	extern void StepperSpin(int index, int direction);

	//  Set the 'speed' of the stepper in percent
	//  where 0.00 is spinning at max delay in the positive direction
	//  and 1.00 is spinning at min delay in the +ve direction, -1.00 is spinning at min delay in -ve direction
	//  input:  index number
	//  input:	speed in unit 1 percentage, positive or negative for direction
	//  note:	you can have <MinDelay> and <MaxDelay> elements in your pinsElement to define the range for the percent
	//			defaults are 1.5 milliseconds and 10 milliseconds
	extern void StepperSetSpeed(int index, float percent);

	//  Set the sequence interval of the stepper
	//  enter 1 to step through all items in the sequence
	//  enter 2 to perform every other items in the sequence
	extern void StepperSetSequenceInterval(int index, int interval);

	//  Stop
	//  request stop of running steps 
	//  input:  index number
	extern void StepperStop(int index);

	//  Get the step counter position
	//  input:  index number
	extern int StepperGetTachoCount(int index);

	//  Reset the step counter
	extern void StepperResetTachoCount(int index);
	
	//  Shut down and clean up all resources for stepper motors
	extern void ShutDownStepperMotors();

#pragma endregion


//  Seven Segment Delay
//
#pragma region SevenSegDisplay
	
	//  Create a seven segment display from XML text
	//  input: XML element describing display pins
	//  output: index of the seven segment display created, or -1 for error
	extern int SevenSegDisplayCreateFromXml(const char* xml);

	extern int SevenSegDisplayCreate(int segPins[8], int numDigits, int digitPins[]);

	//  Stop and remove driver thread for display by index
	//  input:  index number
	extern void SevenSegDisplayRemove(int index);

	//  Turn off the display
	//  input:  index number
	extern void SevenSegDisplayOff(int index);

	//  Set the delay between sequence items
	//  input:	index number
	//  input:	delay, in milliseconds, fractional milliseconds are supported
	extern void SevenSegDisplaySetDelay(int index, int delay); 

	//  Set the display of the seven segment display
	//  input:  index number
	//  input:  characters to display (empty space for no character in that position)
	extern void SevenSegDisplaySet(int index, const char* display);
	
	//  Shut down and clean up all resources in use for seven segment displays
	extern void ShutDownSevenSegDisplays();

#pragma endregion
	

//  Rotary Encoder
//
#pragma region RotaryEncoder
	
	typedef void(*EncoderUpdatedCallback)(int);
	
	//  Create a rotary encoder object
	//  input:  pin numbers for signal pins (pinIndex is optional, set to -1 if not used)
	//  input:  counts per revolution -  The number of bar and window pairs or counts per revolution (CPR) of the codewheel.
	//  input:  function pointer to callback if you want to receive message when tick changes
	//  output: index number of the created rotary encoder, or -1 for error
	extern int RotaryEncoderCreate(int pinA, int pinB, int pinIndex, int countsPerRevolution, EncoderUpdatedCallback callback);

	//  Remove the rotary encoder object
	//  Note: Rotary Encoder object uses wiringPiISR (interrupt).  There is no way to unregister an interrupt.
	extern void RotaryEncoderRemove(int index);

	//  Get rotary encoder raw value
	//  input:  index of the encoder
	extern int RotaryEncoderGetCount(int index);

	//  Get rotary encoder revolutions per minute
	extern double RotaryEncoderGetRpm(int index);

	//  Get rotary encoder revolutions per minute
	extern double RotaryEncoderGetFrequency(int index);

	// Reset the rotary encoder count
	//  input:  index of the encoder
	extern void RotaryEncoderResetCount(int index, int setCount);
	
	//  Shut down and clean up all resources used for rotary encoders
	extern void ShutDownEncoders();

#pragma endregion

	
//  Motor with Rotary Encoder
//
#pragma region MotorWithRotaryEncoder
	
	//  Create a motor with rotary encoder
	//  input:  pin numbers for HBridge control, bridgeIn1 is pin when high motor is running forwards
	//  input:  pin numbers for rotary encoder, encoderPinA is pin that is activated first when motor is running forwards
	//  input:  countsPerRevolution is how many signals the encoder will receive for one full rotation of the output axis (the number of 'windows' on an optical rotary encoder for example)
	//  input:  function pointer to callback if you want to receive message when tick changes
	//  output: index number of the motor with rotary encoder created, or -1 for error
	extern int MotorWithRotaryEncoderCreate(int bridgeIn1, int bridgeIn2, int bridgePwm, int encoderA, int encoderB, int encoderIndex, int countsPerRevolution, EncoderUpdatedCallback callback);

	//  Set the min / max useful power of this motor
	//  TODO
	extern void MotorWithRotaryEncoderSetUsefulPowerRange(int index, double minPower, double maxPower);
	
	//  Remove the motor with rotary encoder
	//  Note: Rotary Encoder object uses wiringPiISR (interrupt).  There is no way to unregister an interrupt.
	extern void MotorWithRotaryEncoderRemove(int index);

	//  Reset the count of the rotary encoder
	//  input: index of the motor with rotary encoder
	//  input: count to set (default is 0)
	extern void MotorWithRotaryEncoderResetCount(int index, int setCount);
	
	//  Get the raw count of the encoder
	//  input: index of the motor with rotary encoder
	//  output: count on the encoder
	extern int MotorWithRotaryEncoderGetCount(int index);
	
	//  Get the raw tick of the encoder
	//  input: index of the motor with rotary encoder
	//  output: tick on the encoder
	extern int MotorWithRotaryEncoderGetTick(int index);
	
	//  Get the encoder circle position
	//  input:  index of the motor with rotary encoder
	//  output: encoder position as normalized circle ( +1.0 = one full rotation forward)
	extern double MotorWithRotaryEncoderGetCircle(int index);
	
	//  Get rotary encoder revolutions per minute
	extern double  MotorWithRotaryEncoderGetRpm(int index);

	//  Get rotary encoder tick frequency
	extern double  MotorWithRotaryEncoderGetTickFrequency(int index);
	
	//  Get rotary encoder frequency
	extern double  MotorWithRotaryEncoderGetFrequency(int index);

	// Run the motor
	// input: index of the motor with rotary encoder
	// input: power level normalized from 0.0 to 1.0
	extern void MotorWithRotaryEncoderRun(int index, double power);
	
	// Stop the motor and release the brake
	// input: index of the motor with rotary encoder
	extern void MotorWithRotaryEncoderStop(int index);

	// Brake the motor
	// will 'short' the motor by applying +ve voltage to both H Bridge pins
	// input: index of the motor with rotary encoder
	// input: power level to apply to H Bridge PWM pin for braking 0.0 to 1.0
	extern void MotorWithRotaryEncoderBrake(int index, double power);

	//  Turn the motor by a specified number of rotations
	// input: index of the motor with rotary encoder
	// input: number of rotations (1.0 = one turn forwards, -1.0 = one turn backwards)
	// input: power level to apply to the motor 0 to 1.0
	extern void MotorWithRotaryEncoderTurnBy(int index, double rotations, double power);

	// Turn the motor to a circle reading and hold the position
	// will turn the motor forwards or backwards to maintain the position
	// input: index of the motor
	// input: circle value on the encoder to hold
	// input: power for the turn to initial motion
	extern void MotorWithRotaryEncoderHoldAt(int index, double circle, double power);
	
	//  shut down and clean up all resources used for motors with rotary encoders
	extern void ShutDownMotorsWithRotaryEncoder();
	
#pragma endregion
	
	
	
//  Logging
//
#pragma region Logging

#include <wiringGpioLogging.h>

	// Logging Callback
	extern wiringGpioLoggingCallback LogFunction;

	//  Log Level
	extern wiringGpioLogLevel LoggingLevel;

	extern void WiringGpioSetLoggingCallback(wiringGpioLoggingCallback);
	extern void WiringGpioSetLoggingLevel(wiringGpioLogLevel level);

	//  Log functions
	void Log(wiringGpioLogLevel level, const char* sender, const char* function, const char* data);
	void LogFormatted(wiringGpioLogLevel level, const char* sender, const char* function, const char* format, ...);

#pragma endregion	// Logging
	
#ifdef __cplusplus
}
#endif
