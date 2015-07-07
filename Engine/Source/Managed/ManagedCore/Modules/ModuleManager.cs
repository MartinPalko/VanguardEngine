using System;
using System.Collections.Generic;
using System.Reflection;
using System.IO;

namespace Vanguard.ManagedCore.Modules
{
    class ModuleManager
    {
        private Dictionary<String,ModuleInfo> moduleInfos = new Dictionary<string,ModuleInfo>();

        public void UpdateModuleList()
        {
            moduleInfos.Clear();
            String moduleDirectory = Directory.GetParent(Assembly.GetExecutingAssembly().Location).ToString();

            string[] dlls = Directory.GetFiles(moduleDirectory, "*.dll");

            foreach(String dll in dlls)
            {
                AssemblyName assemblyName;
                try
                {
                    // Try to load assembly, if it throws an exception, then it's not a managed assembly and we can move on.
                    assemblyName = AssemblyName.GetAssemblyName(dll);                  
                }
                catch { continue; }
                
                // Load the assembly if it's not already loaded.
                Assembly assembly = Assembly.Load(assemblyName);
                // Search assembly for implementations of IModule
                Type[] types = assembly.GetTypes();
                foreach (Type type in types)
                {
                    if (!type.IsInterface && !type.IsAbstract)
                    {
                        if (typeof(IModule).IsAssignableFrom(type))
                        {
                            ModuleInfo moduleInfo = new ModuleInfo(type);
                            if (!moduleInfos.ContainsKey(moduleInfo.ModuleName))
                                moduleInfos[moduleInfo.ModuleName] = moduleInfo;
                            else throw new Exception("Duplicate module name " + moduleInfo.ModuleName + " already in use.");
                        }
                    }
                }                 
            }
        }

        public void LoadModule(String aModuleName)
        {
            if (moduleInfos.ContainsKey(aModuleName))
                moduleInfos[aModuleName].Load();
        }

        public void UnloadModule(String aModuleName)
        {
            if (moduleInfos.ContainsKey(aModuleName))
                moduleInfos[aModuleName].Unload();
        }

        public void UnloadAllModules()
        {
            foreach (KeyValuePair<String, ModuleInfo> entry in moduleInfos)
            {
                entry.Value.Unload();
            }
        }
    }
}
