using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FakeWiringPi
{
    /// <summary>
    /// Stepper Motor Wrapper
    /// </summary>
    public class StepperWrapper
    {
        public StepperWrapper(int stepperIndex, List<GpioPinWrapper> pins, string name)
        {
            Name = name;
            Pins = pins;
            StepperIndex = stepperIndex;
        }

        public void SetDelay(float delay)
        {
            WiringPiWrapper.StepperMotor.SetDelay(StepperIndex, delay);
        }

        public void Step(int steps)
        {
            WiringPiWrapper.StepperMotor.Step(StepperIndex, steps);
        }

        public void Spin(int direction)
        {
            WiringPiWrapper.StepperMotor.Spin(StepperIndex, direction);
        }

        public void SetSpeed(float value)
        {
            WiringPiWrapper.StepperMotor.SetSpeed(StepperIndex, value);
        }

        public void Stop()
        {
            WiringPiWrapper.StepperMotor.Stop(StepperIndex);
        }

        public int TachoCount
        {
            get
            {
                return WiringPiWrapper.StepperMotor.GetTachoCount(StepperIndex);
            }
        }

        public void ResetTachoCount()
        {
            WiringPiWrapper.StepperMotor.ResetTachoCount(StepperIndex);
        }

       

        public int StepperIndex { get; protected set; }
        public List<GpioPinWrapper> Pins { get; protected set; }
        public string Name { get; protected set; }
    }
}
