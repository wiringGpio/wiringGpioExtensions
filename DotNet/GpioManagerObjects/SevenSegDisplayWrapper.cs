using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FakeWiringPi
{
    public class SevenSegDisplayWrapper
    {
        public SevenSegDisplayWrapper(int displayIndex, List<GpioPinWrapper> pins)
        {
            Pins = pins;
            DisplayIndex = displayIndex;
        }

        public void Set(string display)
        {   
            WiringPiWrapper.SevenSegDisplay.Set(DisplayIndex, display);
        }

        public void Off()
        {
            WiringPiWrapper.SevenSegDisplay.Off(DisplayIndex);
        }

        public int DisplayIndex { get; protected set; }
        public List<GpioPinWrapper> Pins { get; protected set; }
    }
}
