#include <unistd.h>
#include <iostream>
#include <iomanip>
#include "WiringPiExtensionApi.h"

using namespace std;



//  Rotary Encoder test function
//
int RotaryEncoderSampleMain(int argc, char *argv[])
{
	int encoderIndex = RotaryEncoderCreate(18, 16, -1, 360, 0x00);
	
	cout << "Rotary Encoder Test -- " << endl;
	while (true)
	{
		cout << setprecision(3) <<  "Freq " << RotaryEncoderGetFrequency(encoderIndex) / 1000.0 <<  " KHz    " << RotaryEncoderGetRpm(encoderIndex) << " RPM  |  Value " << (int)RotaryEncoderGetCount(encoderIndex) << endl;
		usleep(250000);
	}
	
	
	return 0;
}