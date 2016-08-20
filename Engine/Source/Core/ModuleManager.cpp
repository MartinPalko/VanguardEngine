#include "ModuleManager.h"
#include "Foundation.h"

#include "Directories.h"

namespace Vanguard
{
	ModuleInfo::ModuleInfo(const FilePath& aLibPath, const String& aName, DynamicArray<String> aDependencies)
	{
		moduleName = aName;
		dependencies = aDependencies;
		filePath = aLibPath;

		// Not loaded yet.
		dynamicLibReference = nullptr;
		moduleInstance = nullptr;

		loadedExplicitly = false;
	}

	ModuleInfo::~ModuleInfo()
	{
		delete moduleInstance;
		delete dynamicLibReference;
	}

	ModuleInfo* ModuleInfo::LoadModuleAtPath(FilePath aModulePath)
	{
		DynamicLibrary* tempLoadedLib = new DynamicLibrary();

		if (!tempLoadedLib->Open(aModulePath))
		{
			// Could not load dynamic library
			DEBUG_ERROR("Could not load library " + aModulePath.GetFilename());
			delete tempLoadedLib;
			return nullptr;
		}

		T_VANGUARD_MODULE_INST_FUNCTION InstantiationFunction = (T_VANGUARD_MODULE_INST_FUNCTION)tempLoadedLib->GetFunction(TO_STRING(VANGUARD_MODULE_INST_FUNCTION));
		T_VANGUARD_MODULE_NAME_FUNCTION NameFunction = (T_VANGUARD_MODULE_NAME_FUNCTION)tempLoadedLib->GetFunction(TO_STRING(VANGUARD_MODULE_NAME_FUNCTION));
		T_VANGUARD_MODULE_DEPENDENCY_FUNCTION DependenciesFunction = (T_VANGUARD_MODULE_DEPENDENCY_FUNCTION)tempLoadedLib->GetFunction(TO_STRING(VANGUARD_MODULE_DEPENDENCY_FUNCTION));

		if (InstantiationFunction == nullptr || NameFunction == nullptr || DependenciesFunction == nullptr)
		{
			// Could not find a required function (not a module library)
			delete tempLoadedLib;
			return nullptr;
		}

		const String moduleName(NameFunction());
		const String moduleDependencies(DependenciesFunction());

		// Got name and dependencies, done with the temp stuff, so it can be unloaded.
		delete tempLoadedLib;

		return new ModuleInfo(aModulePath, moduleName, moduleDependencies.Split(';'));
	}

	bool ModuleInfo::LoadModule()
	{
		if (GetIsLoaded())
		{
			return true;
		}

		dynamicLibReference = new DynamicLibrary();

		if (!dynamicLibReference->Open(filePath))
		{
			UnloadModule();
			Log::Error("Could not load module \"" + filePath.GetFullPathName() + "\", library cannot be opened.", "Modules");
			return false;
		}

		T_VANGUARD_MODULE_INST_FUNCTION InstantiationFunction = (T_VANGUARD_MODULE_INST_FUNCTION)dynamicLibReference->GetFunction(TO_STRING(VANGUARD_MODULE_INST_FUNCTION));

		if (InstantiationFunction == nullptr)
		{
			UnloadModule();
			Log::Error("Could not load module \"" + filePath.GetFullPathName() + "\", library does not contain function: " + TO_STRING(VANGUARD_MODULE_INST_FUNCTION), "Modules");
			return false;
		}

		moduleInstance = InstantiationFunction();
		moduleInstance->LoadModule();
		return true;
	}

	void ModuleInfo::UnloadModule()
	{
		if (moduleInstance)
		{
			moduleInstance->UnloadModule();
			delete moduleInstance;
			moduleInstance = nullptr;
		}

		if (dynamicLibReference)
		{
			dynamicLibReference->Close();
			delete dynamicLibReference;
			dynamicLibReference = nullptr;
		}
	}

	ModuleManager::ModuleManager()
	{
		UpdateModuleList();
	}

	ModuleManager::~ModuleManager()
	{
		UnloadAllModules();
		ClearModuleList();
	}

	typedef IModule * (*MODULE_INST_FUNCTION)();

	void ModuleManager::UpdateModuleList()
	{
		String pluginWildcard = "*" + String(Platform::DynamicLibExtension());

		DynamicArray<FilePath> dynamicLibsInModuleDirectory = FileSystem::Find(Directories::GetEngineModuleDirectory(), pluginWildcard, false, true, false, true);

		ClearModuleList();

		for (uint32 i = 0; i < dynamicLibsInModuleDirectory.Count(); i++)
		{
			ModuleInfo* moduleInfo = ModuleInfo::LoadModuleAtPath(dynamicLibsInModuleDirectory[i]);

			if (moduleInfo != nullptr)
			{
				moduleInfos[moduleInfo->moduleName] = moduleInfo;
			}
		}
	}

	void ModuleManager::ClearModuleList()
	{
		for (auto pair : moduleInfos)
		{
			delete(pair.second);
		}
		moduleInfos.clear();
	}

	ModuleManager::eModuleLoadResult ModuleManager::LoadModule(const String& aModuleName)
	{
		return LoadModule(aModuleName, true);
	}

	ModuleManager::eModuleLoadResult ModuleManager::LoadModule(const String& aModuleName, bool aExplicit)
	{
		if (!moduleInfos.count(aModuleName))
		{
			if (aExplicit)
			{
				Log::Error("Loading module " + aModuleName + " failed; could not find module", "Modules");
			}
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
				Log::Error("Loading module dependencies for " + aModuleName + " failed", "Modules");
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
