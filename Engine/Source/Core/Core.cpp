#include "Core.h"
#include <ostream>
#include <thread>

#include "ModuleManager.h"
#include "NativeReflection/NativeReflection.h"


namespace Vanguard
{
	BooleanConfigVar Core::clearTempDirectoryOnShutdown = BooleanConfigVar("Core", "Core", "ClearTempDirectoryOnShutdown", true);
	BooleanConfigVar Core::showConsoleOnStart = BooleanConfigVar("Core", "Core", "ShowConsoleOnStart", false);

	Core* Core::instance = nullptr;

	Core* Core::GetInstance() { return instance; }

	void Core::Initialize(int aArgC, char** aArgV, String aProjectName)
	{
		state = CoreState::Initializing;
		instance = this;

		Application::applicationArguments = ApplicationArguments(aArgC, aArgV);

		loadedProject = new Project(Directories::GetVanguardProjectsDirectory().GetRelative(aProjectName));

		// Load config as early as possible, otherwise some classes might be stuck reading their default values!
		ConfigTable::LoadConfigFromDisk();
		
		// TODO: Fix, currently causes crash.
		//if (showConsoleOnStart)
		//	Vanguard::Application::ShowConsoleWindow();

		Log::Initialize();

		JobManager::CreateThreads();

		moduleManager = new ModuleManager();

		// Load the project's module.
		moduleManager->LoadModule(loadedProject->GetName());

		Log::Message("Initialized Core", "Core");
		state = CoreState::Initialized;
	}

	void Core::Run()
	{
		if (state != CoreState::Initialized)
			throw Exception("Core must be in state \"Initialized\" to run");

		Log::Message("Core Running", "Core");
		state = CoreState::Running;

		// Main engine loop
		while (state == CoreState::Running)
		{
			// Tick worlds
			for (int i = 0; i < worlds.Count(); i++)
			{
				World* world = worlds[i];

				Timespan currentTime = Timespan::GetElapsedSystemTime();
				Timespan deltaTime;
				if (world->lastTickStartTime.InSeconds() != 0.0)
				{
					deltaTime = currentTime - world->lastTickStartTime;
				}
				else
				{
					deltaTime = world->minimumTickDelta;
					world->lastTickStartTime = currentTime - deltaTime;
				}

				double ct = currentTime.InSeconds();
				double dt = deltaTime.InSeconds();

				if (deltaTime >= world->minimumTickDelta)
				{
					// Create a new frame for the world
					Frame* frame = new Frame(world->nextFrameNumber, deltaTime, world);
					world->lastTickStartTime = currentTime;
					world->nextFrameNumber++;

					// Update the world
					frame->AddJob([world, frame]()-> void { world->Tick(frame); });

					JobManager::ProcessFrame(frame);
					delete frame;
				}

				if (world->nextFrameNumber > 15)
				{
					this->ShutDown();
				}
			}
			//std::this_thread::sleep_for(std::chrono::microseconds(1));
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

			delete moduleManager;
			moduleManager = nullptr;

			if (clearTempDirectoryOnShutdown)
				FileSystem::Delete(Directories::GetProjectTempDirectory());

			// Want to flush the log as late as possible to make sure all entries get written to disk.
			Log::Flush();

			delete loadedProject;
			loadedProject = nullptr;

			AsyncIO::JoinIOThread();

			Log::Message("Core shut down successfully", "Core");

			state = CoreState::ShutDown;
		}
		else if (state == CoreState::Running || state == CoreState::PendingShutdown)
		{
			state = CoreState::PendingShutdown;
		}
	}

	void Core::LoadModule(const String& aModuleName)
	{
		moduleManager->LoadModule(aModuleName);
	}

	World* Core::CreateWorld(const String& aWorldName)
	{
		if (GetWorld(aWorldName) == nullptr)
		{			
			World* newWorld = new World(aWorldName);
			worlds.PushBack(newWorld);
			return newWorld;
		}
		else
			throw Exception(String("World with name \"" + aWorldName + "\" already exists").GetCharPointer());
	}

	World* Core::GetWorld(const String& aWorldName)
	{
		for (size_t i = 0; i < worlds.Count(); i++)
		{
			if (worlds[i]->GetWorldName() == aWorldName)
				return worlds[i];
		}
		return nullptr;
	}

	void Core::DestroyWorld(World* aWorld)
	{
		if (worlds.Contains(aWorld))
		{
			worlds.Remove(aWorld);
			delete aWorld;			
		}
	}
}
