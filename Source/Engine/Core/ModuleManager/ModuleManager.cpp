#include "ModuleManager.h"
#include "Foundation.h"

namespace Vanguard
{
	ModuleManager::ModuleManager()
	{
	}


	ModuleManager::~ModuleManager()
	{
	}

	void ModuleManager::UpdateModuleList()
	{
		String pluginWildcard = "*" + String(Platform::DynamicLibExtension());

		List<FilePath> dynamicLibsInModuleDirectory = FileSystem::Find(FileSystem::GetEngineModuleDirectory(), pluginWildcard, false, true, false, true);

		for (uint32 i = 0; i < dynamicLibsInModuleDirectory.Size(); i++)
		{
			std::cout << dynamicLibsInModuleDirectory[i] << "\n";
		}		
	}
}
