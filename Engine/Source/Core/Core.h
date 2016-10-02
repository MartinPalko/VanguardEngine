#pragma once

#include "Interfaces/ICore.h"
#include "Core_Common.h"

#include "Config/Config.h"
#include "World.h"
#include "Type.h"
#include "Application/Application.h"
#include "AsyncIO.h"
#include "Log.h"
#include "Profiler.h"
#include "Project.h"
#include "Directories.h"
#include "Events.h"

#include "Entity/Actor.h"
#include "Entity/Component.h"
#include "Entity/Entity.h"
#include "Entity/Transform.h"

#include "Jobs/JobManager.h"
#include "Jobs/Job.h"
#include "Jobs/Frame.h"

#include "Renderer/Camera.h"
#include "Renderer/IRenderer.h"
#include "Renderer/RenderView.h"

#include "concurrentqueue.h"

namespace Vanguard
{
	class ResourceManager;

	// Small helper class that uses RAII behaviour for logging
	// TODO: This should be moved out of Core.h
	class QuickProfiler
	{
		Timespan startTime;
		String message;
		bool ended;

	public:
		QuickProfiler(String aMessage)
		{
			startTime = Timespan::GetElapsedSystemTime();
			message = aMessage;
			ended = false;
		}

		void End()
		{
			Timespan endTime = Timespan::GetElapsedSystemTime();
			Timespan profileTime = endTime - startTime;
			LOG_MESSAGE(message + String::FromFloat(profileTime.InSeconds() * 1000.0) + "ms", "Quick Profiler");
			ended = true;
		}

		~QuickProfiler()
		{
			if (!ended)
				End();
		}
	};

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

	struct ICoreEventListener
	{
		virtual void CoreEvent(Event* aEvent) = 0;
	};

	class ModuleManager;

	// Core is the main class of the Engine. Only one instance of core can be initialized and running at a time.
	// It is responsible for allocating and initializing all other systems, as well as shutting down and deallocating them.
	// Core also provides a singleton for access. The singleton is set on initialization, and cleared on shutdown.
	class CORE_API Core : public ICore
	{
	private:
		static BooleanConfigVar clearTempDirectoryOnShutdown;
		static BooleanConfigVar showConsoleOnStart;

		static Core* instance;
		size_t mainThreadID;

		JobManager* jobManager;
		Profiler* profiler;
		ResourceManager* resourceManager;
		Project* loadedProject;
		ModuleManager* moduleManager;
		DynamicArray<World*> worlds;
		class DynamicArray<IRenderer*> renderers;
		IRenderer* primaryRenderer;
		ProjectModule* projectModule;

		CoreState state = CoreState::NotInitialized;

		moodycamel::ConcurrentQueue<Event*> pendingEvents;
		DynamicArray<ICoreEventListener*> eventListeners;

	public:
		Core();
		virtual ~Core() {};

		static Core* GetInstance();
		size_t GetMainThreadID() { return mainThreadID; }
		JobManager* GetJobManager();
		Profiler* GetProfiler();

		// Implementation of ICore
		// Allocate and initialize all Core systems.
		virtual void Initialize(int aArgC, char** aArgV, const char* aProjectName = "") override;
		// Create a new instance of the project's world.
		virtual World* CreateProjectWorld() override;
		// Enter the main application loop. This function will not return until Core has shut down.
		virtual void Run() override;
		// Shuts down and exits Core. Calling this while running will exit the main application loop.
		virtual void ShutDown() override;

		virtual void LoadModule(const char* aModuleName) override;

		void PostEvent(Event* aEvent);
		void BroadcastEvent(Event* aEvent);
		void RegisterEventListener(ICoreEventListener* aListener);
		void UnregisterEventListener(ICoreEventListener* aListener);
		virtual void ProcessEvents(bool aIncludeNativeEvents);

		inline CoreState GetState(){ return state; }

		class Project* GetLoadedProject(){ return loadedProject; }

		void RegisterResourceManager(ResourceManager* aResourceManager);
		void UnregisterResourceManager();
		ResourceManager* GetResourceManager();

		void RegisterRenderer(IRenderer* aRenderer);
		void UnregisterRenderer(IRenderer* aRenderer);
		IRenderer* GetPrimaryRenderer();

	protected:
		void AddWorld(World* aWorld);
		void DestroyWorld(World* aWorld);
	};
}

#if VANGUARD_DEBUG
#define ASSERT_MAIN_THREAD \
if (Vanguard::Core::GetInstance()->GetMainThreadID() != Thread::CurrentThreadID()) LOG_EXCEPTION("This code must execute on the main thread!", "Core");
#else
#define ASSERT_MAIN_THREAD
#endif
