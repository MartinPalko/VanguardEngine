#include <ostream>
#include <thread>

#include "Core.h"

#include "ModuleManager.h"
#include "Type.h"

VANGUARD_DECLARE_CORE_IMP(Vanguard::Core)

namespace Vanguard
{
	BooleanConfigVar Core::clearTempDirectoryOnShutdown = BooleanConfigVar("Core", "Core", "ClearTempDirectoryOnShutdown", true);

	Core* Core::instance = nullptr;

	Core::Core()
		: jobManager(nullptr)
		, profiler(nullptr)
		, resourceManager(nullptr)
		, loadedProject(nullptr)
		, moduleManager(nullptr)
		, worlds()
		, renderers()
		, primaryRenderer(nullptr)
		, projectModule(nullptr)
	{
		mainThreadID = Thread::CurrentThreadID();
	}

	Core* Core::GetInstance() { return instance; }

	void Core::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		if (String(aProjectName) == "")
		{
			LOG_EXCEPTION("No project specified!", "Core");
		}

		if (state != CoreState::NotInitialized)
		{
			LOG_EXCEPTION("Core is already initialized!", "Core");
		}

		state = CoreState::Initializing;

		if (instance)
		{
			LOG_EXCEPTION("There can only be one Vanguard Core initialized at a time!", "Core");
		}

		instance = this;

		ASSERT_MAIN_THREAD;

		Application::SetApplicationArguments(aArgC, aArgV);

		AsyncIO::StartIOThread();

		loadedProject = new Project(Directories::GetVanguardProjectsDirectory().GetRelative(aProjectName));

		// Load config as early as possible, otherwise some classes might be stuck reading their default values!
		ConfigTable::LoadConfigFromDisk();

		Log::Initialize();

		jobManager = new JobManager();

		profiler = new Profiler();

		moduleManager = new ModuleManager();

		// Load the project's module.
		ModuleManager::eModuleLoadResult projectLoadResult = moduleManager->LoadModule(loadedProject->GetName());
		if (projectLoadResult == ModuleManager::eModuleLoadResult::Success)
		{
			IModule* module = moduleManager->GetLoadedModule(loadedProject->GetName());
			projectModule = dynamic_cast<ProjectModule*>(module);
			if (!projectModule)
				LOG_ERROR("Project " + loadedProject->GetName() + "'s module is not a Project module", "Core");
		}
		else
			LOG_ERROR("Project " + loadedProject->GetName() + " failed to load!", "Core");

		LOG_MESSAGE("Initialized Core", "Core");
		state = CoreState::Initialized;
	}

	World* Core::CreateProjectWorld()
	{
		World* projectWorld = projectModule->CreateWorld();
		AddWorld(projectWorld);
		return projectWorld;
	}

	void Core::Run()
	{
		ASSERT_MAIN_THREAD;

		if (state != CoreState::Initialized)
		{
			LOG_EXCEPTION("Must initialize before running!", "Core");
		}

		LOG_MESSAGE("Core Running", "Core");
		state = CoreState::Running;

		// Main engine loop
		while (state == CoreState::Running)
		{
			ProcessEvents(true);
			jobManager->ServiceMainThreadJobs();

			Timespan nextServiceDue = Timespan::GetElapsedSystemTime() + Timespan(10.0); // Max time to sleep between checking on subsystems

			// Service Subsystems
			for (ISubsystem* subsystem : subsystems)
			{
				Timespan currentTime = Timespan::GetElapsedSystemTime();
				Timespan aLastServiced = subsystemLastServicedTimes[subsystem];
				Timespan estNextServiceTime;

				if (subsystem->NeedsService(currentTime, aLastServiced, estNextServiceTime))
				{
					subsystem->ServiceSubsystem(currentTime);
					subsystemLastServicedTimes[subsystem] = currentTime;
				}

				nextServiceDue = Math::Min(nextServiceDue, estNextServiceTime);
			}

			if (subsystems.Count())
			{
				const Timespan currentTime = Timespan::GetElapsedSystemTime();
				if (currentTime < nextServiceDue)
				{
					// Sleep until the next subsystem needs service
					std::this_thread::sleep_for(std::chrono::microseconds((currentTime - nextServiceDue).InMicroseconds()));
				}
			}
			else
			{
				std::this_thread::yield();
			}
		}

		if (state != CoreState::PendingShutdown)
			throw Exception("Core state is invalid, only \"PendingShutdown\" is valid after \"Running\".");

		state = CoreState::StartingShutdown;
		ShutDown();

	}

	void Core::Quit()
	{
		if (state == CoreState::Running || state == CoreState::PendingShutdown)
		{
			state = CoreState::PendingShutdown;
		}
		else
		{
			LOG_EXCEPTION("Note in a valid state to have Quit() called", "Core");
		}
	}

	void Core::ShutDown()
	{
		if (state != CoreState::StartingShutdown)
		{
			LOG_EXCEPTION("Note in a valid state to have ShutDown() called", "Core");
		}

		ASSERT_MAIN_THREAD;

		state = CoreState::ShuttingDown;

		LOG_MESSAGE("Shutting down Core", "Core");

		for (int i = 0; i < worlds.Count(); i++)
		{
			delete worlds[i];
		}
		worlds.Clear();

		delete profiler;
		profiler = nullptr;

		jobManager->JoinThreads();
		delete jobManager;
		jobManager = nullptr;

		delete moduleManager;
		moduleManager = nullptr;

		if (clearTempDirectoryOnShutdown)
			FileSystem::Delete(Directories::GetProjectTempDirectory());

		delete loadedProject;
		loadedProject = nullptr;

		Event* e;
		while (pendingEvents.try_dequeue(e))
			delete e;

		LOG_MESSAGE("Core shut down successfully", "Core");
		state = CoreState::ShutDown;

		// Want to flush the log as late as possible to make sure all entries get written to disk.
		LOG_MESSAGE("Flushing log and joining IO Thread", "Core");
		Log::Flush();
		AsyncIO::JoinIOThread();			
		instance = nullptr;
	}

	void Core::LoadModule(const char* aModuleName)
	{
		moduleManager->LoadModule(aModuleName);
	}

	void Core::PostEvent(Event* aEvent)
	{
		pendingEvents.enqueue(aEvent);
	}

	void Core::BroadcastEvent(Event* aEvent)
	{
		ASSERT_MAIN_THREAD;

		if(WorldEvent* worldEvent = Type::SafeCast<WorldEvent>(aEvent))
		{
			worldEvent->GetWorld()->BroadcastEvent(worldEvent);
		}
		else
		{
			for(auto listener : eventListeners)
			{
				listener->OnCoreEvent(aEvent);
			}
		}
	}

	void Core::RegisterEventListener(ICoreEventListener* aListener)
	{
		eventListeners.PushBack(aListener);
	}

	void Core::UnregisterEventListener(ICoreEventListener* aListener)
	{
		eventListeners.Remove(aListener);
	}

	void Core::ProcessEvents(bool aIncludeNativeEvents)
	{
		ASSERT_MAIN_THREAD;

		if (aIncludeNativeEvents)
			Application::ProcessNativeEvents();

		Event* e;
		while (pendingEvents.try_dequeue(e))
		{
			BroadcastEvent(e);
			delete e;
		}
	}

	void Core::AddWorld(World* aWorld)
	{
		ASSERT_MAIN_THREAD;
		if (!worlds.Contains(aWorld))
		{
			worlds.PushBack(aWorld);
			BroadcastEvent(&WorldAddedEvent(aWorld));
		}
	}

	void Core::DestroyWorld(World* aWorld)
	{
		ASSERT_MAIN_THREAD;
		if (worlds.Contains(aWorld))
		{
			worlds.Remove(aWorld);
			BroadcastEvent(&WorldRemovedEvent(aWorld));
			delete aWorld;
		}
	}

	void Core::RegisterResourceManager(ResourceManager* aResourceManager)
	{
		ASSERT_MAIN_THREAD;
		if (!resourceManager)
		{
			resourceManager = aResourceManager;
		}
		else
		{
			LOG_EXCEPTION("Cannot register resource manager, a resource manager is already registered", "Core");
		}
	}

	void Core::UnregisterResourceManager()
	{
		ASSERT_MAIN_THREAD;
		if (resourceManager)
		{
			resourceManager = nullptr;
		}
		else
		{
			LOG_EXCEPTION("Cannot unregister resource manager, no resource manager is registered", "Core");
		}
	}

	ResourceManager* Core::GetResourceManager()
	{
		return resourceManager;
	}

	void Core::RegisterRenderer(IRenderer* aRenderer)
	{
		ASSERT_MAIN_THREAD;
		if (renderers.Contains(aRenderer))
		{
			LOG_WARNING("A renderer already registered!", "Core");
			return;
		}

		renderers.PushBack(aRenderer);

		if (!primaryRenderer)
		{
			primaryRenderer = aRenderer;
		}

		LOG_MESSAGE("Registered renderer: " + aRenderer->RendererName(), "Core");
	}

	void Core::UnregisterRenderer(IRenderer * aRenderer)
	{
		ASSERT_MAIN_THREAD;
		if (aRenderer == primaryRenderer)
		{
			primaryRenderer = nullptr;
		}

		renderers.Remove(aRenderer);

		if (!primaryRenderer && renderers.Count())
		{
			primaryRenderer = renderers[0];
		}
		LOG_MESSAGE("Unregistered renderer: " + aRenderer->RendererName(), "Core");
	}

	IRenderer* Core::GetPrimaryRenderer()
	{
		return primaryRenderer;
	}

	void Core::RegisterSubsystem(ISubsystem* aSubsystem)
	{
		ASSERT_MAIN_THREAD;
		if (subsystems.Contains(aSubsystem))
		{
			LOG_WARNING("Subsystem is already registered!", "Core");
			return;
		}

		subsystems.PushBack(aSubsystem);
		subsystemLastServicedTimes[aSubsystem] = Timespan(MinFloat()); // A long time ago, to guarantee it gets serviced the first time.
	}

	void Core::UnregisterSubsystem(ISubsystem* aSubsystem)
	{
		ASSERT_MAIN_THREAD;
		if (!subsystems.Contains(aSubsystem))
		{
			LOG_WARNING("Cannot unregister subsystem, it is not currently registered!", "Core");
			return;
		}

		subsystems.Remove(aSubsystem);
		subsystemLastServicedTimes.erase(aSubsystem);
	}

	DynamicArray<World*> Core::GetWorlds()
	{
		return worlds;
	}

	JobManager* Core::GetJobManager()
	{
		return jobManager;
	}

	Profiler* Core::GetProfiler()
	{
		return profiler;
	}
}
