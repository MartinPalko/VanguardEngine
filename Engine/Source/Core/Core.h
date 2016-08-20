#pragma once

#include "Core_Common.h"

#include "Config/Config.h"
#include "World.h"
#include "Type.h"
#include "Application/Application.h"
#include "AsyncIO.h"
#include "Log.h"
#include "Project.h"
#include "Directories.h"

#include "Jobs/JobManager.h"
#include "Jobs/Job.h"
#include "Jobs/Frame.h"

#include "Renderer/Camera.h"
#include "Renderer/IRenderer.h"
#include "Renderer/RenderView.h"

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
		static BooleanConfigVar showConsoleOnStart;

		static Core* instance;

		JobManager* jobManager;

		class Project* loadedProject;
		class ModuleManager* moduleManager;
		DynamicArray<World*> worlds;
		class DynamicArray<IRenderer*> renderers;
		IRenderer* primaryRenderer;

		CoreState state = CoreState::NotInitialized;

	public:
		Core();

		static Core* GetInstance();
		JobManager* GetJobManager();

		void Initialize(int aArgC, char** aArgV, String aProjectName = "");
		void Run();
		void ShutDown();

		inline CoreState GetState(){ return state; }

		void LoadModule(const String& aModuleName);

		class Project* GetLoadedProject(){ return loadedProject; }

		World* CreateWorld(const String& aWorldName);
		World* GetWorld(const String& aWorldName);
		void DestroyWorld(World* aWorld);

		void RegisterRenderer(IRenderer* aRenderer);
		void UnregisterRenderer(IRenderer* aRenderer);
		IRenderer* GetPrimaryRenderer();
	};


}