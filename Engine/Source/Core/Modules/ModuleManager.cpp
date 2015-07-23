#include "ModuleManager.h"
#include "Foundation.h"
#include "ModuleInfo.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

#include "Directories.h"

namespace Vanguard
{
	ModuleManager::ModuleManager(ManagedAssembly* aManagedCoreAssembly)
	{
		managedModuleManager = new ManagedObject("Vanguard", "ModuleManager", aManagedCoreAssembly);

		UpdateModuleList();
	}

	ModuleManager::~ModuleManager()
	{
		UnloadAllModules();

		if (managedModuleManager != nullptr)
			delete managedModuleManager;
	}

	typedef IModule * (*MODULE_INST_FUNCTION)();

	void ModuleManager::UpdateModuleList()
	{
		String pluginWildcard = "*" + String(Platform::DynamicLibExtension());

		DynamicArray<FilePath> dynamicLibsInModuleDirectory = FileSystem::Find(Directories::GetEngineModuleDirectory(), pluginWildcard, false, true, false, true);

		moduleInfos.clear();

		for (uint32 i = 0; i < dynamicLibsInModuleDirectory.Count(); i++)
		{
			ModuleInfo* moduleLib = ModuleInfo::LoadModuleAtPath(dynamicLibsInModuleDirectory[i]);

			if (moduleLib != nullptr)
			{
				moduleInfos[moduleLib->moduleName] = moduleLib;
				DEBUG_LOG("Found native module " + moduleLib->moduleName + " of type " + moduleLib->moduleType);
			}
		}
		managedModuleManager->CallMethod("UpdateModuleList");
	}

	void ModuleManager::LoadModule(const String& aModuleName)
	{
		if (moduleInfos.count(aModuleName) != 0)
			moduleInfos[aModuleName]->LoadModule();
		else if (managedModuleManager != nullptr)
			managedModuleManager->CallMethod("LoadModule", aModuleName);
	}

	void ModuleManager::UnloadModule(const String& aModuleName)
	{
		if (moduleInfos.count(aModuleName) != 0)
			moduleInfos[aModuleName]->UnloadModule();
		else if (managedModuleManager != nullptr)
			managedModuleManager->CallMethod("UnloadModule", aModuleName);
	}

	void ModuleManager::UnloadAllModules()
	{
		for (auto& item : moduleInfos)
		{
			item.second->UnloadModule();
		}
		managedModuleManager->CallMethod("UnloadAllModules");
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
