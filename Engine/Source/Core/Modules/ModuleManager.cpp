#include "ModuleManager.h"
#include "Foundation.h"
#include "ModuleInfo.h"

#include "Directories.h"

namespace Vanguard
{
	ModuleManager::ModuleManager()
	{
		UpdateModuleList();
	}

	ModuleManager::~ModuleManager()
	{
		UnloadAllModules();
	}

	typedef IModule * (*MODULE_INST_FUNCTION)();

	void ModuleManager::UpdateModuleList()
	{
		String pluginWildcard = "*" + String(Platform::DynamicLibExtension());

		DynamicArray<FilePath> dynamicLibsInModuleDirectory = FileSystem::Find(Directories::GetEngineModuleDirectory(), pluginWildcard, false, true, false, true);

		moduleInfos.clear();

		for (uint32 i = 0; i < dynamicLibsInModuleDirectory.Count(); i++)
		{
			ModuleInfo* moduleInfo = ModuleInfo::LoadModuleAtPath(dynamicLibsInModuleDirectory[i]);

			if (moduleInfo != nullptr)
			{
				moduleInfos[moduleInfo->moduleName] = moduleInfo;
				DEBUG_LOG("Found native module " + moduleInfo->moduleName);
				DEBUG_LOG("Dependencies: " + moduleInfo->dependencies);
			}
		}
	}

	void ModuleManager::LoadModule(const String& aModuleName)
	{
		if (moduleInfos.count(aModuleName) != 0)
			moduleInfos[aModuleName]->LoadModule();
	}

	void ModuleManager::UnloadModule(const String& aModuleName)
	{
		if (moduleInfos.count(aModuleName) != 0)
			moduleInfos[aModuleName]->UnloadModule();
	}

	void ModuleManager::UnloadAllModules()
	{
		for (auto& item : moduleInfos)
			item.second->UnloadModule();
	}

	DynamicArray<IModule*> ModuleManager::GetLoadedModules()
	{
		DynamicArray<IModule*> returnList = DynamicArray<IModule*>();

		for (auto& item : moduleInfos)
		{
			if (item.second->GetIsLoaded())
				returnList.PushBack(item.second->moduleInstance);
		}

		return returnList;
	}
}
