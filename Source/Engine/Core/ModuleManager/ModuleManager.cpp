#include "ModuleManager.h"
#include "Foundation.h"
#include "ModuleLib.h"
#include "juce_core.h"

namespace Vanguard
{
	ModuleManager::ModuleManager()
	{
		UpdateModuleList();
	}


	ModuleManager::~ModuleManager()
	{
		// Todo: unload all modules
	}

	typedef IModule * (*MODULE_INST_FUNCTION)();

	void ModuleManager::UpdateModuleList()
	{
		String pluginWildcard = "*" + String(Platform::DynamicLibExtension());

		List<FilePath> dynamicLibsInModuleDirectory = FileSystem::Find(FileSystem::GetEngineModuleDirectory(), pluginWildcard, false, true, false, true);

		moduleLibs.Clear();

		for (uint32 i = 0; i < dynamicLibsInModuleDirectory.Size(); i++)
		{
			ModuleLib* moduleLib = ModuleLib::LoadModuleAtPath(dynamicLibsInModuleDirectory[i]);

			if (moduleLib != nullptr)
			{
				moduleLibs.Add(moduleLib->moduleName,moduleLib);
				//std::cout << "Found module " << moduleLib->moduleName << " of type " << moduleLib->moduleType << "\n";
			}
		}
	}
}
