//  Rotary Encoder Sample Project
//

#include "main.h"
#include "WiringPiExtensionApi.h"
#include "EncoderSamples.h"
#include "RotaryEncoderSample.h"


using namespace std;

//  Map of MCP chips, pin base to number of pins
map<int, int> McpMap;

// Map of Pca chips, pin base to file descriptor
map<int, int> PcaMap;


//  main
//
int main(int argc, char *argv[])
{
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = SetupWiringPiExtensionGpio();
	
	// - setup any expansion chips here
//	// mcp chip
//	Mcp23017Setup(100, 0x20);
//	McpMap[100] = 16;
//	//  pca chip
//	int fd = Pca9685Setup(200, 0x40, 50.0);
//	if (fd != 0)
//	{
//		PcaMap[500] = fd;
//	}
	
	//EncoderSampleMain(argc, argv);        
	
	RotaryEncoderSampleMain(argc, argv);
	
	ShutDownWiringPiExtension();

}