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
	BooleanConfigVar Core::clearTempDirectoryOnShutdown = BooleanConfigVar("Core", "Core", "ClearTempDirectoryOnShutdown", true);

	Core* Core::instance = nullptr;

	Core* Core::GetInstance() { return instance; }

	void Core::Initialize(int aArgC, char** aArgV)
	{
		state = CoreState::Initializing;
		instance = this;

		Application::applicationArguments = ApplicationArguments(aArgC, aArgV);

		// Load config as early as possible, otherwise some classes might be stuck reading their default values!
		ConfigTable::LoadConfigFromDisk();

		Log::Initialize();

		JobManager::CreateThreads();

		managedCore = new ManagedAssembly("ManagedCore");
		moduleManager = new ModuleManager(managedCore);

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
			ShutDown();
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

			Log::Message("Shutting down Core", "Core");

			delete moduleManager;
			delete managedCore;

			if (clearTempDirectoryOnShutdown)
				FileSystem::ClearEngineTempDirectory();

			// Want to flush the log as late as possible to make sure all entries get written to disk.
			Log::Flush();

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
			delete aWorld;
			worlds.Remove(aWorld);
		}
	}
}
