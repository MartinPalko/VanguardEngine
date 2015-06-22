#pragma once

#include "Foundation.h"
#include "Core_Common.h"
#include "IModule.h"

#include "ManagedObject.h"

namespace Vanguard
{
	struct ModuleInfo;
	class IModule;

	class CORE_API ModuleManager
	{
	public:
		ModuleManager(ManagedAssembly* aManagedCoreAssembly);
		~ModuleManager();

		// Create or update the module list with all modules found in the bin folder.
		void UpdateModuleList();
		void LoadModule(const String& aModuleName);
		void UnloadModule(const String& aModuleName);
		void UnloadAllModules();

		List<IModule*> GetLoadedModules();

	private:
		// List of all recognized modules.
		std::map<String, ModuleInfo*> moduleInfos;
		ManagedObject* managedModuleManager;
	};
}
	