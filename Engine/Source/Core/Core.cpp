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
		
	}

	Core* Core::GetInstance() { return instance; }

	void Core::Initialize(int aArgC, char** aArgV, const char* aProjectName)
	{
		if (state != CoreState::NotInitialized)
		{
			Log::Exception("Core is already initialized!", "Core");
		}

		state = CoreState::Initializing;

		if (instance)
		{
			Log::Exception("There can only be one Vanguard Core initialized at a time!", "Core");
		}

		instance = this;

		Application::SetApplicationArguments(aArgC, aArgV);

		loadedProject = new Project(Directories::GetVanguardProjectsDirectory().GetRelative(aProjectName));

		// Load config as early as possible, otherwise some classes might be stuck reading their default values!
		ConfigTable::LoadConfigFromDisk();

		Log::Initialize();

		jobManager = new JobManager();

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
				Log::Error("Project " + loadedProject->GetName() + "'s module is not a Project module", "Core");
		}
		else
			Log::Error("Project " + loadedProject->GetName() + " failed to load!", "Core");

		Log::Message("Initialized Core", "Core");
		state = CoreState::Initialized;
	}

	void Core::Run()
	{
		if (state != CoreState::Initialized)
		{
			Log::Exception("Must initialize before running!", "Core");
		}

		Log::Message("Core Running", "Core");
		state = CoreState::Running;

		if (!worlds.Count())
		{
			Log::Error("No worlds have been registered, shutting down", "Core");
			ShutDown();
		}

		// Main engine loop
		while (state == CoreState::Running)
		{
			Application::ProcessNativeEvents();

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

					// Update the world
					frame->AddJob([world, frame]()-> void { world->Tick(frame); });

					jobManager->ProcessFrame(frame);
					delete frame;
				}
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
			state = CoreState::ShuttingDown;

			Log::Message("Shutting down Core", "Core");

			jobManager->JoinThreads();
			delete jobManager;
			jobManager = nullptr;

			delete moduleManager;
			moduleManager = nullptr;

			if (clearTempDirectoryOnShutdown)
				FileSystem::Delete(Directories::GetProjectTempDirectory());

			delete loadedProject;
			loadedProject = nullptr;

			Log::Message("Core shut down successfully", "Core");
			state = CoreState::ShutDown;

			// Want to flush the log as late as possible to make sure all entries get written to disk.
			Log::Message("Flushing log and joining IO Thread", "Core");
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
			Log::Warning("A renderer already registered!", "Core");
			return;
		}

		renderers.PushBack(aRenderer);

		if (!primaryRenderer)
		{
			primaryRenderer = aRenderer;
		}

		Log::Message("Registered renderer: " + aRenderer->RendererName(), "Core");
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
		Log::Message("Unregistered renderer: " + aRenderer->RendererName(), "Core");
	}

	IRenderer * Core::GetPrimaryRenderer()
	{
		return primaryRenderer;
	}
	JobManager * Core::GetJobManager()
	{
		return nullptr;
	}
}
