  
# wiringGpioExtensions

A library to provide a single interface for GPIO control code that can be built for the Raspberry Pi or the NVIDIA Jetson simply by changing the linker settings.

The wiringGpioExtensions library interface mimics the wiringPi library. Most function names are the same as wiringPi except with a capital first letter. Function names starting with `wiringPi*` are  renamed to `wiringGpio*`. For example:

    main()
    {
	    WiringGpioSetupPhys();     // call instead of wiringPiSetupPhys()
	    PinMode(15, OUTPUT);       // call instead of pinMode()
	    DigitalWrite(15,1);        // call instead of digitalWrite()
	    Mcp23017Setup(1,100,0x20); // call instead of mcp23017Setup()
	    DigitalWrite(101,1);       // you get the idea ...    
    }

  
The wiringGpioExtensions library also provides a number of additional driver methods, such as:

-   Unipolar and bipolar stepper motor
-   Rotary encoder
-   Motor with rotary encoder
-   Seven segment display
-   ...
  

To use the library with Raspberry Pi, build with `make CONFIG=Release`, and install with `sudo make CONFIG=Release install`.  In your program, `#include <wiringGpioExtensions.h>` and add `-lwiringGpioExtensions -lwiringPi -lpthread` to your linker settings.
  
To use with NVIDIA Jetson, build with `make CONFIG=ReleaseJet`, and install with `sudo make CONFIG=ReleaseJet install`.  In your program, `#include <wiringGpioExtensions.h>` and add `-lwiringGpioExtensions -lwiringJet -lpthread` to your linker settings.

