#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringGpioExtensions.h>

#include "../Logging.h"

#define BASE 100
#define SPI_CHAN 0

int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	int rc = WiringGpioSetup();
	rc = WiringGpioSPISetup(SPI_CHAN, 500000);
	
	rc = Mcp3008Setup(BASE, SPI_CHAN);
	
	// Loop indefinitely, waiting for 100ms between each set of data
	while(1)
	{
		for (int i = 0; i < 8; i++)printf("Channel %d: value=%4d\n", i, AnalogRead(BASE + i));
		printf("\n");
		usleep(100000);	
	}
}
