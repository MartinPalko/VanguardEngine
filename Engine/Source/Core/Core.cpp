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
		, loadedProject(nullptr)
		, moduleManager(nullptr)
		, worlds()
		, renderers()
		, primaryRenderer(nullptr)
	{
		mainThreadID = Thread::CurrentThreadID();
	}

	Core* Core::GetInstance() { return instance; }

	void Core::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
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
			ProjectModule* projectModule = dynamic_cast<ProjectModule*>(module);
			if (projectModule)
			{
				// Create the world specified by our project.
				World* projectWorld = projectModule->CreateWorld();
				if (projectWorld)
				{
					AddWorld(projectWorld);
				}
			}
			else
				LOG_ERROR("Project " + loadedProject->GetName() + "'s module is not a Project module", "Core");
		}
		else
			LOG_ERROR("Project " + loadedProject->GetName() + " failed to load!", "Core");

		LOG_MESSAGE("Initialized Core", "Core");
		state = CoreState::Initialized;
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
			Application::ProcessNativeEvents();
			jobManager->ServiceMainThreadJobs();

			// Tick worlds
			for (int i = 0; i < worlds.Count(); i++)
			{
				World* world = worlds[i];

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
					// Create a new frame for the world
					Frame* frame = new Frame(world->nextFrameNumber, Math::Min(deltaTime, world->maximumTickDelta), world);
					world->lastTickStartTime = currentTime;
					world->nextFrameNumber++;

					profiler->StartProfiling();

					// Update the world
					frame->AddJob(world->MakeTickJob(frame));
					frame->Start();

					// Wait for the frame to finish
					while (!frame->Finished())
					{
						// Service main thread jobs dispatched by the frame.
						jobManager->ServiceMainThreadJobs();
						// Help out the worker threads while we wait.
						jobManager->HelpWithJob();
					}

					// Temp debugging					
					const float frameTime = (Timespan::GetElapsedSystemTime() - currentTime).InSeconds() * 1000.0;
					LOG_MESSAGE("Frame time : " + String::FromFloat(frameTime), "Core");

					if (profiler->IsProfiling() && world->nextFrameNumber == 10)
						profiler->EndProfiling(Directories::GetVanguardRootDirectory().GetRelative("ProfilerResults.json"));
					else 
						profiler->EndProfiling();

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

	void Core::AddWorld(World* world)
	{
		worlds.PushBack(world);
	}

	void Core::DestroyWorld(World* aWorld)
	{
		if (worlds.Contains(aWorld))
		{
			worlds.Remove(aWorld);
			delete aWorld;			
		}
	}

	void Core::RegisterRenderer(IRenderer * aRenderer)
	{
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
