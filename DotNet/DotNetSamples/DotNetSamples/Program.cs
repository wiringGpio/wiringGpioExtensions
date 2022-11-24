using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WiringPiWrapper;

namespace DotNetSamples
{
    class Program
    {
		static void LoggingCallbackFunction(WiringPiExtensionLogEvent e)
		{
            var logEvent = new LogEvent(e);

			var date = new DateTimeOffset(e.LogUnixTimeMilliseconds,TimeSpan.FromHours(-5));

			Console.WriteLine($"{logEvent.LogTime}   {logEvent.ObjectName} {logEvent.FunctionName} {logEvent.Log}.");
		}

        static void EncoderCallbackFunction(int count)
		{
			Console.WriteLine($"Encoder callback {count}");
		}


        static async Task Main(string[] args)
        {
            
            WiringPiWrapper.Logging.SetLoggingCallback(LoggingCallbackFunction);
			Logging.SetLoggingLevel(Logging.LogLevel.Trace);
            WiringPiWrapper.Setup.SetupWiringPiExtension();

			var index = MotorWithRotaryEncoder.Create(11, 7, 12, 40, 37, -1, 180, EncoderCallbackFunction);

			MotorWithRotaryEncoder.ResetCount(index, 0);

			MotorWithRotaryEncoder.TurnBy(index, .25, .3);

			await Task.Delay(5000);

			MotorWithRotaryEncoder.Stop(index);
			MotorWithRotaryEncoder.Remove(index);

			Setup.ShutDownWiringPiExtension();

			Console.WriteLine("Test Complete");
			Console.ReadLine ();

			Console.WriteLine("Test should exit now ...");

			return; //  TODO - why does this not return after reading line ?

        }

        
    }
}
