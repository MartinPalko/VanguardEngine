#include "ModuleLib.h"

namespace Vanguard
{
	typedef IModule * (*MODULE_INST_FUNCTION)();
	static const String InstantiateFunctionName = "InstantiateVanguardModule";

	ModuleLib::ModuleLib(const FilePath& aLibPath, const String& aName, const String& aType)
	{
		moduleName = aName;
		moduleType = aType;
		
		filePath = aLibPath;

		// Not loaded yet.
		dynamicLibReference = nullptr;
		moduleInstance = nullptr;
	}

	ModuleLib::~ModuleLib()
	{
		delete moduleInstance;
		delete dynamicLibReference;
	}

	ModuleLib* ModuleLib::LoadModuleAtPath(FilePath aModulePath)
	{
		juce::DynamicLibrary* tempLoadedLib = new juce::DynamicLibrary(aModulePath.GetFilename());

		if (tempLoadedLib == nullptr)
		{
			// Could not load dynamic library
			std::cout << "Could not load lib " << aModulePath << "\n";
			return nullptr;
		}

		MODULE_INST_FUNCTION InstantiationFunction = (MODULE_INST_FUNCTION)tempLoadedLib->getFunction(InstantiateFunctionName);

		if (InstantiationFunction == nullptr)
		{
			// Could not find instantiation function (not a module library)
			std::cout << "could not locate instantiation function for " << aModulePath << "\n";
			delete tempLoadedLib;
			return nullptr;
		}

		IModule* tempModuleInstance = InstantiationFunction();

		String moduleName = tempModuleInstance->GetPluginName();
		String moduleType = tempModuleInstance->GetPluginType();

		// Got name and type, done with the temp stuff, so it can be unloaded.
		delete tempModuleInstance;
		delete tempLoadedLib;

		return new ModuleLib(aModulePath, moduleName, moduleType);
	}
}