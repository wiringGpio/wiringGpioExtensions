#include <iostream>
#include <unistd.h>
#include <iomanip>

#include <wiringGpioExtensions.h>


using namespace std;

double million = 1000000.0;

void RawControl(int index)
{
	double powerLevel = 0.0;

	bool repeat = true;
	while (repeat)
	{
		// Set terminal to raw mode 
		system("stty raw");

		char key =  ' ';
		key = getchar();
		cout << '\b'; 		//  backspace the cursor
		
		if(key == 'q')
		{
			repeat = false;
		}
		else if(key == 'e')
		{
			powerLevel +=.1;
			if (powerLevel > 1.0)
				powerLevel = 1.0;
		}
		else if(key == 'd')
		{
			powerLevel -=.1;
			if (powerLevel < -1.0)
				powerLevel = -1.0;
			
		}
		else if(key == 'w')
		{
			powerLevel += .01;
			if (powerLevel > 1.0)
				powerLevel = 1.0;
		}
		else if(key == 's')
		{
			powerLevel -= .01;
			if (powerLevel < -1.0)
				powerLevel = -1.0;
			
		}
		else if(key == 'x')
		{
			powerLevel = 0;
		}
		
		// Reset terminal to normal "cooked" mode 
		system("stty cooked");
		
		cout << "Set Motor Power " << powerLevel <<  endl;

		MotorWithRotaryEncoderRun(index, powerLevel);
	}
}

void ShowRpm(int index)
{
	int counter = 0;
	while (counter < 20)
	{
		counter++;
		cout << setprecision(3) <<  "Freq " << RotaryEncoderGetFrequency(index)  <<  " Hz    " << RotaryEncoderGetRpm(index) << " RPM  |  Value " << (int)RotaryEncoderGetCount(index) << endl;
		usleep(250000);
	}
}

int MotorWithRotaryEncoderSampleMain(int argc, char *argv[])
{
	cout << "Motor With Rotary Encoder Sample" << endl; 
	
	int index = MotorWithRotaryEncoderCreate(11, 7, 12, 40, 37, -1, 180, 0x00);
	
	//  test motor at 12V
	MotorWithRotaryEncoderSetUsefulPowerRange(index, .1, 1.0);
	
	MotorWithRotaryEncoderResetCount(index, 0);
	
	//  test motor at 12V
	//MotorWithRotaryEncoderSetUsefulPowerRange(index, .175, 1.0);
	
	double turnPowerLevel = .2;
	bool repeat = true;
	while (repeat)
	{	
		string key;
		getline(cin, key);
		
		if (key == "q")
		{
			MotorWithRotaryEncoderRun(index, 0.0);
			repeat = false;
		}
		else if (key == "w")
		{
			MotorWithRotaryEncoderTurnBy(index, .25, turnPowerLevel);
		}
		else if (key == "s")
		{
			MotorWithRotaryEncoderTurnBy(index, -.25, turnPowerLevel);
		}
		else if (key == "e")
		{
			MotorWithRotaryEncoderHoldAt(index, .25, turnPowerLevel);
		}
		else if (key == "d")
		{
			MotorWithRotaryEncoderHoldAt(index, 0, turnPowerLevel);
		}
		else if (key == "c")
		{
			MotorWithRotaryEncoderHoldAt(index, -.25, turnPowerLevel);
		}
		else if (key == "x")
		{
			MotorWithRotaryEncoderRun(index, 0.0);
		}
		else if (key == "b")
		{
			MotorWithRotaryEncoderBrake(index, 1.0);
		}
		else if (key == "r")
		{
			MotorWithRotaryEncoderResetCount(index, 0);
		}
		else if (key == "v")
		{
			RawControl(index);
		}
		else if (key == "h")
		{
			ShowRpm(index);
		}
		
	}
	
	cout << "Shutting down." << endl;
	WiringGpioTerminate();
	
	
	return 0;
}