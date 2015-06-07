#pragma once

#include "Foundation.h"
#include "Core.h"
#include "IModule.h"

namespace Vanguard
{
	struct ModuleLib
	{
		friend class ModuleManager;

	public:
		~ModuleLib();

	private:
		ModuleLib(const FilePath& aLibPath, const String& aName, const String& aType);
		String moduleName;
		String moduleType;
		FilePath filePath;
		juce::DynamicLibrary* dynamicLibReference;
		IModule* moduleInstance;

	protected:
		// Tries to load a module library at a given path, returns nullptr if it can't be loaded.
		static ModuleLib* LoadModuleAtPath(FilePath aModulePath);
	};
}
