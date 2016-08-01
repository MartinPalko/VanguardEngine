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
			}
		}
	}

	ModuleManager::eModuleLoadResult ModuleManager::LoadModule(const String& aModuleName)
	{
		return LoadModule(aModuleName, true);
	}

	ModuleManager::eModuleLoadResult ModuleManager::LoadModule(const String& aModuleName, bool aExplicit)
	{
		if (!moduleInfos.count(aModuleName))
		{
			return eModuleLoadResult::NotFound;
		}

		ModuleInfo* moduleInfo = moduleInfos.at(aModuleName);

		moduleInfo->loadedExplicitly = moduleInfo->loadedExplicitly || aExplicit;

		if (moduleInfo->GetIsLoaded())
		{			
			return eModuleLoadResult::AlreadyLoaded;
		}

		// Load dependencies.
		for (int i = 0; i < moduleInfo->dependencies.Count(); i++)
		{
			if (!moduleInfos.count(moduleInfo->dependencies[i]))
			{
				// Currently, regular dynamic and static libraries are listed as dependencies alongside modules,
				// so there's no need for alarm if a dependency is not registered as a module.
				continue;
			}			
			ModuleInfo* dependentModule = moduleInfos.at(moduleInfo->dependencies[i]);

			const eModuleLoadResult dependentLoadResult = LoadModule(dependentModule->moduleName, false);
			if (dependentLoadResult != eModuleLoadResult::Error)
			{
				dependentModule->inUseBy.PushBack(aModuleName);
			}
			else
			{
				// Loading a dependent module failed.
				UnloadModule(aModuleName);
				return eModuleLoadResult::Error;
			}
		}

		Log::Message("Loading module: " + aModuleName, "Modules");
		return moduleInfo->LoadModule() ? eModuleLoadResult::Success : eModuleLoadResult::Error;
	}

	void ModuleManager::UnloadModule(const String& aModuleName)
	{
		if (!moduleInfos.count(aModuleName))
		{
			return;
		}

		ModuleInfo* moduleInfo = moduleInfos.at(aModuleName);

		moduleInfo->UnloadModule();
		moduleInfo->loadedExplicitly = false;

		// Unload dependencies.
		for (std::pair<String, ModuleInfo*> dependentModuleInfo : moduleInfos)
		{
			ModuleInfo* dependentModule = dependentModuleInfo.second;
			if (dependentModule->inUseBy.Contains(moduleInfo->moduleName))
			{
				dependentModule->inUseBy.Remove(moduleInfo->moduleName);

				if (dependentModule->inUseBy.Count() <= 0 && !dependentModule->loadedExplicitly)
				{
					UnloadModule(dependentModule->moduleName);
				}
			}
		}
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
