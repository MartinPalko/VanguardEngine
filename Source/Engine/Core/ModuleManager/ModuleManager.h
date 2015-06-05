#pragma once

#include "Foundation.h"
#include "CoreMacros.h"

namespace Vanguard
{
	struct ModuleInfo;
	class IModule;

	class CORE_API ModuleManager
	{
	public:
		ModuleManager();
		~ModuleManager();

		// Create or update the module list with all modules found in the bin folder.
		void UpdateModuleList();

	private:
		//List<IModule*> LoadedModules;
		List<ModuleInfo*> moduleList;

		//void RefreshModuleInfoes();


		//bool LoadModule(const string aModuleName);
		//bool UnloadModule(const string aModuleName);

		//// Gets a loaded module by name. Returns null if the module isn't loaded.
		//IModule* GetModule(const string aModuleName);

		//ModuleInfo GetModuleInfo(const string aModuleName);
	};
}

