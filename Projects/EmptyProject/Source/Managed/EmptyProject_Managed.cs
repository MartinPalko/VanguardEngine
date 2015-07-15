﻿using System;

namespace Vanguard
{
    public class ManagedModule : IModule
    {
        public void LoadModule()
        {
            Console.WriteLine("Loaded EmptyProject Module: " + GetModuleName());
        }

        public void UnloadModule()
        {
            Console.WriteLine("Unloaded EmptyProject Module: " + GetModuleName());
        }

        public String GetModuleName()
        {
            return "EmptyProject";
        }

        public String GetModuleType()
        {
            return "Project";
        }
    }
}