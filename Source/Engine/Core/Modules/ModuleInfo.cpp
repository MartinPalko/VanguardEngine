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
		juce::DynamicLibrary* tempLoadedLib = new juce::DynamicLibrary(aModulePath.GetFilename());

		if (tempLoadedLib == nullptr)
		{
			// Could not load dynamic library
			//std::cout << "Could not load lib " << aModulePath.GetFullPathName() << "\n";
			return nullptr;
		}

		MODULE_INST_FUNCTION InstantiationFunction = (MODULE_INST_FUNCTION)tempLoadedLib->getFunction(InstantiateFunctionName);

		if (InstantiationFunction == nullptr)
		{
			// Could not find instantiation function (not a module library)
			//std::cout << "could not locate instantiation function for " << aModulePath.GetFullPathName() << "\n";
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
			dynamicLibReference = new juce::DynamicLibrary(filePath.GetFilename());

			if (dynamicLibReference == nullptr)
			{
				throw std::exception("Could not load module " + filePath.GetFullPathName());
			}

			MODULE_INST_FUNCTION InstantiationFunction = (MODULE_INST_FUNCTION)dynamicLibReference->getFunction(InstantiateFunctionName);

			if (InstantiationFunction == nullptr)
			{
				throw std::exception("Could not locate instantiation function for " + filePath.GetFullPathName());
			}

			moduleInstance = InstantiationFunction();
			moduleInstance->LoadModule();
		}
	}

	void ModuleInfo::UnloadModule()
	{
		if (GetIsLoaded())
		{
			moduleInstance->UnloadModule();
			delete moduleInstance;
			delete dynamicLibReference;
		}
	}
}
