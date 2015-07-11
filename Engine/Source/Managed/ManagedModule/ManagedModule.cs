using System;

namespace Vanguard
{
    public class ManagedModule : IModule
    {
        public void Test()
        {
           //Log.Message("Static Calll", "Modules");
        }

        public void LoadModule()
        {
            Log.Message("Loaded Managed Module: " + GetModuleName(), "Modules");
        }

        public void UnloadModule()
        {
            Log.Message("Unloaded Managed Module: " + GetModuleName(), "Modules");
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
