#include "ModuleInfo.h"

namespace Vanguard
{
	typedef IModule * (*MODULE_INST_FUNCTION)();
	static const String InstantiateFunctionName = "InstantiateVanguardModule";

	ModuleInfo::ModuleInfo(const FilePath& aLibPath, const String& aName, const String& aType)
	{
		moduleName = aName;
		moduleType = aType;

		filePath = aLibPath;

		// Not loaded yet.
		dynamicLibReference = nullptr;
		moduleInstance = nullptr;
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

		MODULE_INST_FUNCTION InstantiationFunction = (MODULE_INST_FUNCTION)tempLoadedLib->GetFunction(InstantiateFunctionName.GetCharPointer());

		if (InstantiationFunction == nullptr)
		{
			// Could not find instantiation function (not a module library)
			delete tempLoadedLib;
			return nullptr;
		}

		IModule* tempModuleInstance = InstantiationFunction();

		String moduleName = tempModuleInstance->GetModuleName();
		String moduleType = tempModuleInstance->GetModuleType();

		// Got name and type, done with the temp stuff, so it can be unloaded.
		delete tempModuleInstance;
		delete tempLoadedLib;

		return new ModuleInfo(aModulePath, moduleName, moduleType);
	}

	void ModuleInfo::LoadModule()
	{
		if (!GetIsLoaded())
		{
			dynamicLibReference = new DynamicLibrary();

			if (!dynamicLibReference->Open(filePath))
			{
				// Could not load dynamic library
				UnloadModule();
				throw Exception(String("Could not load module " + filePath.GetFullPathName()).GetCharPointer());
			}

			MODULE_INST_FUNCTION InstantiationFunction = (MODULE_INST_FUNCTION)dynamicLibReference->GetFunction(InstantiateFunctionName.GetCharPointer());

			if (InstantiationFunction == nullptr)
			{
				throw Exception(String("Could not load module " + filePath.GetFullPathName()).GetCharPointer());
			}

			Log::Message("Loading native module " + moduleName, "Modules");

			moduleInstance = InstantiationFunction();
			moduleInstance->LoadModule();
		}
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
