#pragma once

#include "Foundation.h"
#include "CoreMacros.h"
#include "IModule.h"

namespace Vanguard
{
	struct ModuleLib;
	class IModule;

	class ModuleManager
	{
	public:
		ModuleManager();
		~ModuleManager();

		// Create or update the module list with all modules found in the bin folder.
		void UpdateModuleList();

	private:
		// List of all recognized modules.
		Hashtable<String,ModuleLib*> moduleLibs;
	};
}
	