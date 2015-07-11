using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Vanguard
{
    public static class Log
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Message(String aMessage, String aChannel = "");

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Warning(String aMessage, String aChannel = "");

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Error(String aMessage, String aChannel = "");

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Exception(String aMessage, String aChannel = "");
    }
}
