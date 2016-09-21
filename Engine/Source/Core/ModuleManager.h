#pragma once
#include "Foundation.h"
#include "Core_Common.h"
#include "Interfaces/IModule.h"
#include "Log.h"

namespace Vanguard
{
	class IModule;

	// Information struct about a module.
	// All modules located in the modules directory have a ModuleInfo created for them, whether they are loaded or not.
	struct ModuleInfo
	{
		friend class ModuleManager;

	public:
		~ModuleInfo();
		bool GetIsLoaded() { return moduleInstance != nullptr; }
		bool LoadModule();
		void UnloadModule();
		// Pointer to the instance of this module. nullptr if not loaded.
		IModule* GetInstance() { return moduleInstance; }

	private:
		ModuleInfo(const FilePath& aLibPath, const String& aName, DynamicArray<String> aDependencies);
		String moduleName;
		DynamicArray<String> dependencies;
		FilePath filePath;
		DynamicLibrary* dynamicLibReference;
		IModule* moduleInstance;

		// If not loaded explicitly, module has been loaded as a result of another module's dependencies,
		// and will automatically be unloaded when it's no longer in use by another module.
		bool loadedExplicitly;

		// List of other loaded modules currently using this module.
		// This module cannot be unloaded until it's no longer in use by another module.
		DynamicArray<String> inUseBy;

	protected:
		// Tries to load a module library at a given path, returns nullptr if it can't be loaded.
		static ModuleInfo* LoadModuleAtPath(FilePath aModulePath);
	};

	class CORE_API ModuleManager
	{
	public:
		enum class eModuleLoadResult : uint8
		{
			Error = 0,
			NotFound,
			Success,
			AlreadyLoaded
		};

	private:
		// List of all recognized modules.
		std::map<String, ModuleInfo*> moduleInfos;

	public:
		ModuleManager();
		~ModuleManager();

		// Create or update the module list with all modules found in the bin folder.
		void UpdateModuleList();
		void ClearModuleList();
		eModuleLoadResult LoadModule(const String& aModuleName);
		void UnloadModule(const String& aModuleName);
		void UnloadAllModules();

		IModule* GetLoadedModule(const String& aModuleName);
		DynamicArray<IModule*> GetLoadedModules();
		

	private:
		eModuleLoadResult LoadModule(const String& aModuleName, bool aExplicit);
	};
}
