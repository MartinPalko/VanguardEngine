#pragma once

#include "Foundation.h"
#include "Core_Common.h"
#include "Modules/IModule.h"
namespace Vanguard
{
	class IModule;

	struct ModuleInfo
	{
		friend class ModuleManager;

	public:
		~ModuleInfo();
		bool GetIsLoaded() { return moduleInstance != nullptr; }
		bool LoadModule();
		void UnloadModule();

	private:
		ModuleInfo(const FilePath& aLibPath, const String& aName, DynamicArray<String> aDependencies);
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
		eModuleLoadResult LoadModule(const String& aModuleName);
		void UnloadModule(const String& aModuleName);
		void UnloadAllModules();

		DynamicArray<IModule*> GetLoadedModules();

	private:
		eModuleLoadResult LoadModule(const String& aModuleName, bool aExplicit);
	};
}
