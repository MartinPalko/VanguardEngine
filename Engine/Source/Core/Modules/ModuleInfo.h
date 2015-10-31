#pragma once

#include "Foundation.h"
#include "Core.h"
#include "IModule.h"

namespace Vanguard
{
	struct ModuleInfo
	{
		friend class ModuleManager;

	public:
		~ModuleInfo();
		bool GetIsLoaded(){ return moduleInstance != nullptr; }
		void LoadModule();
		void UnloadModule();

	private:
		ModuleInfo::ModuleInfo(const FilePath& aLibPath, const String& aName, const String& aDependencies);
		String moduleName;
		String dependencies;
		FilePath filePath;
		DynamicLibrary* dynamicLibReference;
		IModule* moduleInstance;

	protected:
		// Tries to load a module library at a given path, returns nullptr if it can't be loaded.
		static ModuleInfo* LoadModuleAtPath(FilePath aModulePath);
	};
}
