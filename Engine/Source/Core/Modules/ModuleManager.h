#pragma once

#include "Foundation.h"
#include "Core_Common.h"
#include "IModule.h"

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
		void LoadModule(const String& aModuleName);
		void UnloadModule(const String& aModuleName);
		void UnloadAllModules();

		DynamicArray<IModule*> GetLoadedModules();

	private:
		// List of all recognized modules.
		std::map<String, ModuleInfo*> moduleInfos;
	};
}
	