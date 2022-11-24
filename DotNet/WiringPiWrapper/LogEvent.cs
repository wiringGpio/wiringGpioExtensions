using System;

using static System.Runtime.InteropServices.Marshal;



namespace WiringPiWrapper
{
    public class LogEvent
    {
        public LogEvent(WiringPiExtensionLogEvent wpeEvent)
        {
			LogTime = DateTimeOffset.FromUnixTimeMilliseconds (wpeEvent.LogUnixTimeMilliseconds);
            ObjectName = PtrToStringAnsi(wpeEvent.ObjectNamePtr);
            FunctionName = PtrToStringAnsi(wpeEvent.FunctionNamePtr);
            Log = PtrToStringAnsi(wpeEvent.LogPtr);
            Level = wpeEvent.Level;
        }

        public DateTimeOffset LogTime { get; protected set; }
        public string ObjectName { get; protected set; }
        public string FunctionName { get; protected set; }
        public string Log { get; protected set; }
        public Logging.LogLevel Level;

        
    }
}
