#pragma once

#include "Config.h"
#include "Foundation.h"
#include "Core_Common.h"
#include "EntityComponentSystem.h"
#include "NativeReflection.h"
#include "Application.h"
#include "Log.h"
#include "Project.h"
#include "Directories.h"

#include "JobManager.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	enum class CoreState : uint8
	{
		NotInitialized = 0,
		Initializing,
		Initialized,
		Running,
		PendingShutdown,
		StartingShutdown,
		ShuttingDown,
		ShutDown
	};

	class ModuleManager;
	class ManagedAssembly;

	class CORE_API Core
	{
	private:
		static BooleanConfigVar clearTempDirectoryOnShutdown;

		static Core* instance;

		class Project* loadedProject;
		class ModuleManager* moduleManager;
		class ManagedAssembly* managedCore;

		DynamicArray<World*> worlds = DynamicArray<World*>();

		CoreState state = CoreState::NotInitialized;

	public:
		static Core* GetInstance();

		void Initialize(int aArgC, char** aArgV, String aProjectName = "");
		void Run();
		void ShutDown();

		inline CoreState GetState(){ return state; }

		void LoadModule(const String& aModuleName);

		class Project* GetLoadedProject(){ return loadedProject; }

		World* CreateWorld(const String& aWorldName);
		World* GetWorld(const String& aWorldName);
		void DestroyWorld(World* aWorld);
	};


}