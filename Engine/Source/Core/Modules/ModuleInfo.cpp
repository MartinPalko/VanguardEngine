#include "ModuleInfo.h"

#include "Log.h"
#include "ModuleManager.h"

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

		String moduleName = NameFunction();
		String moduleDependencies = DependenciesFunction();

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
			Log::Error("Could not load module " + filePath.GetFullPathName() + "\", library cannot be opened.", "Modules");
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
}
