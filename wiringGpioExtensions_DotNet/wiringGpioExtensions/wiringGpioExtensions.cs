

using System;
using System.Runtime.InteropServices;


namespace wiringGpioExtensions
{
    public static class Constants
    {
        public static double RPiPwmClockSpeed = 19.2e6;
        public static int RPiPwmFrequency = 50;
    }

    public enum PinMode
    {
        Input = 0,
        Output = 1,
        PWMOutput = 2,
        GPIOClock = 3,
        SoftPwmOutput,
        SoftToneOutput,
        PwmToneOutput,
    }

    public enum PinValue
    {
        Low = 0,
        High = 1
    }

    public enum PullUpDownValue
    {
        Off = 0,
        Down = 1,
        Up = 2
    }

    public enum InterruptLevels
    {
        INT_EDGE_SETUP = 0,
        INT_EDGE_FALLING = 1,
        INT_EDGE_RISING = 2,
        INT_EDGE_BOTH = 3
    }

    public enum Mcp23Type
    {
        Mcp23017 = 0,
        Mcp23008 = 1,
    }


    public struct WiringPiExtensionLogEvent
    {
        public long LogUnixTimeMilliseconds;
        public IntPtr ObjectNamePtr;
        public IntPtr FunctionNamePtr;
        public IntPtr LogPtr;
        public Logging.LogLevel Level;
    }


    /// <summary>
    /// Logging
    /// </summary>
    /// 
    public delegate void LoggingCallback(WiringPiExtensionLogEvent e);
    //
    public class Logging
    {
        public enum LogLevel
        {
            All = 0,
            Verbose,
            Trace,
            Debug,
            Info,
            Warn,
            Error,
            Latal,
            User,
            Off,
        }

        [DllImport("libWiringPiExtension.so", EntryPoint = "SetLoggingCallback")]
        public static extern void SetLoggingCallback(LoggingCallback fn);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SetLoggingLevel")]
        public static extern  void SetLoggingLevel(LogLevel level);

    }

    /// <summary>
    /// Initialization
    /// Raw functions can be used to initialise Gordon's library if you only want to use raw functions from wiringPi
    /// If you are using the extension, you should call SetupWiringPiExtension
    /// </summary>
    public class Setup
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiSetup")]
        public static extern int WiringPiSetup();

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiSetupGpio")]
        public static extern int WiringPiSetupGpio();

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiSetupSys")]
        public static extern int WiringPiSetupSys();

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiSetupPhys")]
        public static extern int WiringPiSetupPhys();

        [DllImport("libWiringPiExtension.so", EntryPoint = "SetupWiringPiExtension")]
        public static extern int SetupWiringPiExtension();

        [DllImport("libWiringPiExtension.so", EntryPoint = "ShutDownWiringPiExtension")]
        public static extern int ShutDownWiringPiExtension();
    }


    /// <summary>
    /// GPIO pin functions
    /// </summary>
    public class GPIO
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "PinMode")]           
        public static extern void PinMode(int pin, PinMode mode);

        [DllImport("libWiringPiExtension.so", EntryPoint = "PinModeAlt")]
        public static extern void PinModeAlt(int pin, PinMode mode);

        [DllImport("libWiringPiExtension.so", EntryPoint = "DigitalWrite")]      
        public static extern void DigitalWrite(int pin, PinValue value);

        [DllImport("libWiringPiExtension.so", EntryPoint = "DigitalWriteByte")]      
        public static extern void DigitalWriteByte(int value);

        [DllImport("libWiringPiExtension.so", EntryPoint = "DigitalRead")]
        public static extern int DigitalRead(int pin);

        [DllImport("libWiringPiExtension.so", EntryPoint = "AnalogRead")]           
        public static extern int AnalogRead(int pin);

        [DllImport("libWiringPiExtension.so", EntryPoint = "AnalogWrite")]
        public static extern int AnalogWrite(int pin, int value);
        
        [DllImport("libWiringPiExtension.so", EntryPoint = "PullUpDnControl")]         
        public static extern void PullUpDnControl(int pin, PullUpDownValue pud);

        //This pwm mode cannot be used when using GpioSys mode!!
        [DllImport("libWiringPiExtension.so", EntryPoint = "PwmWrite")]              
        public static extern void PwmWrite(int pin, int value);

        [DllImport("libWiringPiExtension.so", EntryPoint = "PwmSetMode")]             
        public static extern void PwmSetMode(PinMode mode);

        [DllImport("libWiringPiExtension.so", EntryPoint = "PwmSetRange")]             
        public static extern void PwmSetRange(uint range);

        [DllImport("libWiringPiExtension.so", EntryPoint = "PwmSetClock")]             
        public static extern void PwmSetClock(int divisor);

        [DllImport("libWiringPiExtension.so", EntryPoint = "GpioClockSet")]              
        public static extern void GpioClockSet(int pin, int freq);

        //  Software PWM
        //
        [DllImport("libWiringPiExtension.so", EntryPoint = "SoftPwmCreate")]
        public static extern int SoftPwmCreate(int pin, int value, int range);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SoftPwmWrite")]
        public static extern void SoftPwmWrite(int pin, int value);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SoftPwmStop")]
        public static extern void SoftPwmStop(int pin);
    }

    
    

    /// <summary>
    /// Provides use of the Timing functions such as delays
    /// </summary>
    public class Timing
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "Millis")]
        public static extern uint Millis();

        [DllImport("libWiringPiExtension.so", EntryPoint = "Micros")]
        public static extern uint Micros();

        [DllImport("libWiringPiExtension.so", EntryPoint = "Delay")]
        public static extern void Delay(uint howLong);

        [DllImport("libWiringPiExtension.so", EntryPoint = "DelayMicroseconds")]
        public static extern void DelayMicroseconds(uint howLong);
    }


    /// <summary>
    /// Provides access to the Thread priority and interrupts for IO
    /// </summary>
    //
    public delegate void ISRCallback();
    //
    public class Interrupts
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "PiHiPri")]
        public static extern int PiHiPri(int priority);

        [DllImport("libWiringPiExtension.so", EntryPoint = "WaitForInterrupt")]
        public static extern int WaitForInterrupt(int pin, int timeout);
        
        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiISR")]
        public static extern int WiringPiISR(int pin, int mode, ISRCallback method);

    }

    public class PiBoard
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "PiBoardRev")]
        public static extern int PiBoardRev();

        [DllImport("libWiringPiExtension.so", EntryPoint = "WpiPinToGpio")]
        public static extern int WpiPinToGpio(int wPiPin);

        [DllImport("libWiringPiExtension.so", EntryPoint = "PhysPinToGpio")]
        public static extern int PhysPinToGpio(int physPin);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SetPadDrive")]
        public static extern void SetPadDrive(int group, int value);
    }

    /// <summary>
    /// Provides SPI port functionality
    /// </summary>
    public class SPI
    {
        /// <summary>
        /// Configures the SPI channel specified on the Raspberry Pi
        /// </summary>
        /// <param name="channel">Selects either Channel 0 or 1 for use</param>
        /// <param name="speed">Selects speed, 500,000 to 32,000,000</param>
        /// <returns>-1 for an error, or the linux file descriptor the channel uses</returns>
        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiSPISetup")]
        public static extern int WiringPiSPISetup(int channel, int speed);

        /// <summary>
        /// Read and Write data over the SPI bus, don't forget to configure it first
        /// </summary>
        /// <param name="channel">Selects Channel 0 or Channel 1 for this operation</param>
        /// <param name="data">signed byte array pointer which holds the data to send and will then hold the received data</param>
        /// <param name="len">How many bytes to write and read</param>
        /// <returns>-1 for an error, or the linux file descriptor the channel uses</returns>
        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiSPIDataRW")]
        public static unsafe extern int WiringPiSPIDataRW(int channel, byte* data, int len);  //char is a signed byte
    }

    /// <summary>
    /// Provides access to the I2C port
    /// </summary>
    public class I2C
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiI2CSetup")]
        public static extern int WiringPiI2CSetup(int devId);

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiI2CRead")]
        public static extern int WiringPiI2CRead(int fd);

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiI2CWrite")]
        public static extern int WiringPiI2CWrite(int fd, int data);

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiI2CWriteReg8")]
        public static extern int WiringPiI2CWriteReg8(int fd, int reg, int data);

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiI2CWriteReg16")]
        public static extern int WiringPiI2CWriteReg16(int fd, int reg, int data);

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiI2CReadReg8")]
        public static extern int WiringPiI2CReadReg8(int fd, int reg);

        [DllImport("libWiringPiExtension.so", EntryPoint = "WiringPiI2CReadReg16")]
        public static extern int WiringPiI2CReadReg16(int fd, int reg);
    }



    /// <summary>
    /// MCP 23008 and 23017 Pin Expander Chips
    /// </summary>
    public class MCP230
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "Mcp23017Setup")]
        public static extern int Mcp23017Setup(int pinBase, int address);

        [DllImport("libWiringPiExtension.so", EntryPoint = "Mcp23008Setup")]
        public static extern int Mcp23008Setup(int pinBase, int address);
    }


    /// <summary>
    /// MCP 3004 and 3008 Analog to Digital Converter Chip
    /// </summary>
    public class MCP300
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "Mcp3004Setup")]
        public static extern int Mcp3004Setup(int pinBase, int spiChannel);

        [DllImport("libWiringPiExtension.so", EntryPoint = "Mcp3008Setup")]
        public static extern int Mcp3008Setup(int pinBase, int spiChannel);
    }


    /// <summary>
    /// PCA Chips
    /// </summary>
    public class PCS9685
    {

        [DllImport("libWiringPiExtension.so", EntryPoint = "Pca9685Setup")]
        public static extern int Setup(int pinBase, int i2cAddress, float freq);

        [DllImport("libWiringPiExtension.so", EntryPoint = "Pca9685PWMReset")]
        public static extern int PWMReset(int fd);

        [DllImport("libWiringPiExtension.so", EntryPoint = "Pca9685GetFrequencyForPin")]
        public static extern int GetFrequencyForPin(int pin);

    }


    /// <summary>
    /// Software stepper motor
    /// </summary>
    public class StepperMotor
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperCreateFromXml")]
        public static extern int CreateFromXml(string sequenceElement, string pinsElement);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperRemove")]
        public static extern int Remove(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperSetDelay")]
        public static extern void SetDelay(int index, float delay);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperStep")]
        public static extern void Step(int index, int numberSteps);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperSpin")]
        public static extern void Spin(int index, int direction);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperSetSpeed")]
        public static extern void SetSpeed(int index, float percent);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperStop")]
        public static extern void Stop(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperGetTachoCount")]
        public static extern int GetTachoCount(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "StepperResetTachoCount")]
        public static extern void ResetTachoCount(int index);
    }


    /// <summary>
    /// Seven Segment Display
    /// </summary>
    public class SevenSegDisplay
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "SevenSegDisplayCreateFromXml")]
        public static extern int CreateFromXml(string xml);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SevenSegDisplayCreate")]
        public static extern int Create(int[] segPins, int numDigits, int[] digitPins);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SevenSegDisplayRemove")]
        public static extern void Remove(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SevenSegDisplayOff")]
        public static extern void Off(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SevenSegDisplaySetDelay")]
        public static extern void SetDelay(int index, int delay);

        [DllImport("libWiringPiExtension.so", EntryPoint = "SevenSegDisplaySet")]
        public static extern void Set(int index, string display);
    }


    /// <summary>
    /// Rotary Encoder
    /// </summary>
    //
    public delegate void EncoderUpdatedCallback(int count);
    //
    public class RotaryEncoder
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "RotaryEncoderCreate")]
        public static extern int Create(int pinA, int pinB, int pinIndex, int countsPerRevolution, EncoderUpdatedCallback callback);

        [DllImport("libWiringPiExtension.so", EntryPoint = "RotaryEncoderRemove")]
        public static extern void Remove(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "RotaryEncoderGetCount")]
        public static extern int GetCount(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "RotaryEncoderGetRpm")]
        public static extern double GetRpm(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "RotaryEncoderGetFrequency")]
        public static extern double GetFrequency(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "RotaryEncoderResetCount")]
        public static extern void ResetCount(int index, int setCount);
    }


    /// <summary>
    /// Motor with Rotary Encoder
    /// </summary>
    public class MotorWithRotaryEncoder
    {
        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderCreate")]
        public static extern int Create(int bridgeIn1, int bridgeIn2, int bridgePwm, int encoderA, int encoderB, int encoderIndex, int countsPerRevolution, EncoderUpdatedCallback callback);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderRemove")]
        public static extern void Remove(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderResetCount")]
        public static extern void ResetCount(int index, int setCount);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderGetCount")]
        public static extern int GetCount(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderGetCircle")]
        public static extern double GetCircle(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderGetRpm")]
        public static extern double GetRpm(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderGetTickFrequency")]
        public static extern double GetTickFrequency(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderGetFrequency")]
        public static extern double GetFrequency(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderRun")]
        public static extern void Run(int index, double power);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderStop")]
        public static extern void Stop(int index);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderBrake")]
        public static extern void Brake(int index, double power);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderTurnBy")]
        public static extern void TurnBy(int index, double rotations, double power);

        [DllImport("libWiringPiExtension.so", EntryPoint = "MotorWithRotaryEncoderHoldAt")]
        public static extern void HoldAt(int index, double circle, double power);
    }


}