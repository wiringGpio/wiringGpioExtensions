#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "WiringPiExtensionApi.h"


#define BASE 100
#define SPI_CHAN 0

int main(int argc, char *argv[])
{
	int i;

	
	int rc = WiringPiSetup();
	rc = WiringPiSPISetup(SPI_CHAN, 500000);
	
	rc = Mcp3008Setup(BASE, SPI_CHAN);
	
	// Loop indefinitely, waiting for 100ms between each set of data
	while(1)
	{
		for (i = 0; i < 8; i++)printf("Channel %d: value=%4d\n", i, AnalogRead(BASE + i));
		printf("\n");
		usleep(100000);	
	}
}
