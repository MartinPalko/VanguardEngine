#pragma once

#include "Core_Common.h"

#include "Config/Config.h"
#include "EntityComponent/EntityComponentSystem.h"
#include "NativeReflection/NativeReflection.h"
#include "Application/Application.h"
#include "Log.h"
#include "Project.h"
#include "Directories.h"

#include "Jobs/JobManager.h"
#include "Jobs/Job.h"
#include "Jobs/Frame.h"

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

	class CORE_API Core
	{
	private:
		static BooleanConfigVar clearTempDirectoryOnShutdown;

		static Core* instance;

		class Project* loadedProject;
		class ModuleManager* moduleManager;

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