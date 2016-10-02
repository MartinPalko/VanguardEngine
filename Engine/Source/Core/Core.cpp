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

		if (!worlds.Count())
		{
			LOG_ERROR("No worlds have been registered, shutting down", "Core");
			ShutDown();
		}

		// Main engine loop
		while (state == CoreState::Running)
		{
			ProcessEvents(true);
			jobManager->ServiceMainThreadJobs();

			// Tick worlds
			for (World* world : worlds)
			{
				Timespan currentTime(Timespan::GetElapsedSystemTime());
				Timespan deltaTime;
				if (world->lastTickStartTime.InSeconds() != 0.0)
				{
					deltaTime = currentTime - world->lastTickStartTime;
				}
				else
				{
					// First tick
					deltaTime = world->minimumTickDelta;
					world->lastTickStartTime = currentTime - deltaTime;
				}

				if (deltaTime >= world->minimumTickDelta)
				{
					// Create a new frame for the world.
					Frame* frame = new Frame(world->nextFrameNumber, Math::Min(deltaTime, world->maximumTickDelta), world);
					world->lastTickStartTime = currentTime;
					world->nextFrameNumber++;

					if (profiler->profileNextFrame)
					{
						profiler->profileNextFrame = false;
						profiler->BeginFrameProfile();
					}

					// Update the world.
					frame->AddJob(world->MakeTickJob(frame));
					frame->Start();

					// Wait for the frame to finish.
					while (!frame->Finished())
					{
						// Service main thread jobs dispatched by the frame.
						jobManager->ServiceMainThreadJobs();
						// Help out the worker threads while we wait.
						jobManager->HelpWithJob();
					}

					// Dispatch any events posted while processing the frame.
					ProcessEvents(false);

					if (profiler->IsProfilingFrame())
						profiler->EndFrameProfile(Directories::GetLogDirectory().GetRelative("ProfilerResults.json"));

					delete frame;
				}
			}

			if (worlds.Count())
			{
				// Estimate how long we can sleep,until the next frame needs to be done.
				Timespan nextFrameTime = worlds[0]->GetNextDesiredTickTime();
				for (int i = 1; i < worlds.Count(); i++)
				{
					nextFrameTime = Math::Min(nextFrameTime, worlds[i]->GetNextDesiredTickTime());
				}

				const Timespan currentTime = Timespan::GetElapsedSystemTime();
				if (currentTime < nextFrameTime)
				{
					auto sleepTime = (nextFrameTime - currentTime).InMilliseconds();
					// And sleep
					std::this_thread::sleep_for(std::chrono::microseconds((currentTime - nextFrameTime).InMicroseconds()));
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

	void Core::ShutDown()
	{
		if (state < CoreState::Running)
			throw Exception("Core has not yet been running, shut down is invalid.");

		if (state >= CoreState::ShutDown)
			throw Exception("Core has already shut down, calling shut down again is invalid.");

		if (state == CoreState::StartingShutdown)
		{
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

			LOG_MESSAGE("Core shut down successfully", "Core");
			state = CoreState::ShutDown;

			// Want to flush the log as late as possible to make sure all entries get written to disk.
			LOG_MESSAGE("Flushing log and joining IO Thread", "Core");
			Log::Flush();
			AsyncIO::JoinIOThread();			
			instance = nullptr;
		}
		else if (state == CoreState::Running || state == CoreState::PendingShutdown)
		{
			state = CoreState::PendingShutdown;
		}
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
				listener->CoreEvent(aEvent);
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
		worlds.PushBack(aWorld);
	}

	void Core::DestroyWorld(World* aWorld)
	{
		ASSERT_MAIN_THREAD;
		if (worlds.Contains(aWorld))
		{
			worlds.Remove(aWorld);
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

	JobManager* Core::GetJobManager()
	{
		return jobManager;
	}

	Profiler* Core::GetProfiler()
	{
		return profiler;
	}
}
