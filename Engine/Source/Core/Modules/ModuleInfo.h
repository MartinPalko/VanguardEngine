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
		bool LoadModule();
		void UnloadModule();

	private:
		ModuleInfo::ModuleInfo(const FilePath& aLibPath, const String& aName, DynamicArray<String> aDependencies);
		String moduleName;
		DynamicArray<String> dependencies;
		FilePath filePath;
		DynamicLibrary* dynamicLibReference;
		IModule* moduleInstance;
		bool loadedExplicitly;
		DynamicArray<String> inUseBy;

	protected:
		// Tries to load a module library at a given path, returns nullptr if it can't be loaded.
		static ModuleInfo* LoadModuleAtPath(FilePath aModulePath);
	};
}
