#include "Core.h"
#include <ostream>
#include <thread>

#include "ManagedClass.h"
#include "ModuleManager.h"
#include "IModule.h"
#include "ManagedAssembly.h"
#include "NativeReflection.h"
#include "JobManager.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	Core* Core::instance = nullptr;

	Core* Core::GetInstance() { return instance; }

	void Core::Initialize()
	{
		state = CoreState::Initializing;
		instance = this;

		ConfigTable::LoadConfigFromDisk();
		ConfigTable::SaveConfigToDisk(); // Save right away, to generate defaults if they don't exist. TODO: More elequently

		JobManager::CreateThreads();

		managedCore = new ManagedAssembly("ManagedCore");
		moduleManager = new ModuleManager(managedCore);

		Log::Write("Initialized Core");
		state = CoreState::Initialized;
	}

	void Core::Run()
	{
		if (state != CoreState::Initialized)
			throw Exception("Core must be in state \"Initialized\" to run");

		Log::Write("Core Running");
		state = CoreState::Running;

		// Main engine loop
		while (state == CoreState::Running)
		{
			// TODO: Tick worlds

			//Frame* frame = new Frame(0, 0.03f, gameWorld);

			//List<IModule*> loadedModules = moduleManager->GetLoadedModules();

			//for (int i = 0; i < loadedModules.Size(); i++)
			//{
			//	frame->AddJob([=]()-> void { loadedModules[i]->OnFrame(frame); });
			//}

			//JobManager::ProcessFrame(frame);
			//delete frame;

			std::this_thread::sleep_for(std::chrono::microseconds(10));
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

			Log::Write("Shutting down Core");

			delete moduleManager;
			delete managedCore;
			ConfigTable::SaveConfigToDisk();

			state == CoreState::ShutDown;
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
			World* newWorld = new World(aWorldName);
		else
			throw Exception(String("World with name \"" + aWorldName + "\" already exists").GetCharPointer());
	}

	World* Core::GetWorld(const String& aWorldName)
	{
		for (int i = 0; i < worlds.Size(); i++)
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
			delete aWorld;
			worlds.Remove(aWorld);
		}
	}
}
