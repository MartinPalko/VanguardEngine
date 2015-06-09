using System;
using System.Collections.Generic;
using System.Reflection;

namespace Vanguard.ManagedCore.Modules
{
    class ModuleInfo
    {
        private String moduleName;
        public String ModuleName { get { return moduleName; } }

        private String moduleType;
        public String MduleType { get { return moduleType; } }

        private Type moduleInstanceType;
        public Type ModuleInstanceType { get { return moduleInstanceType; } }

        private IModule moduleInstance = null;
        public IModule ModuleInstance { get { return moduleInstance; } }

        public bool IsLoaded { get { return moduleInstance != null; } }

        public ModuleInfo(Type aInstanceType)
        {
            IModule tempInstance = (IModule)Activator.CreateInstance(aInstanceType);
            moduleName = tempInstance.GetModuleName();
            moduleType = tempInstance.GetModuleType();
            moduleInstanceType = aInstanceType;
            Console.WriteLine("Found managed module " + moduleName + " of type " + moduleType);
        }

        public void Load()
        {
            moduleInstance = (IModule)Activator.CreateInstance(moduleInstanceType);
            moduleInstance.LoadModule();
        }

        public void Unload()
        {
            if (moduleInstance != null)
                moduleInstance.UnloadModule();
            moduleInstance = null;
        }
    }
}
