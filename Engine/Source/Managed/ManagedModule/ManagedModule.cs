using System;
using Vanguard.ManagedCore.Modules;

namespace Vanguard.ManagedModule
{
    public class ManagedModule : IModule
    {
        public void Test()
        {
            Console.WriteLine("Static Call");
        }

        public void LoadModule()
        {
            Console.WriteLine("Loaded Managed Module: " + GetModuleName());
        }

        public void UnloadModule()
        {
            Console.WriteLine("Unloaded Managed Module: " + GetModuleName());
        }

        public String GetModuleName()
        {
            return "ManagedModule";
        }

        public String GetModuleType()
        {
            return "Managed";
        }
    }
}
