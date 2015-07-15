#include "Core.h"
#include <ostream>
#include <thread>

#include "Mono/ManagedClass.h"
#include "Modules/ModuleManager.h"
#include "Modules/IModule.h"
#include "NativeReflection/NativeReflection.h"


namespace Vanguard
{
	BooleanConfigVar Core::clearTempDirectoryOnShutdown = BooleanConfigVar("Core", "Core", "ClearTempDirectoryOnShutdown", true);

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

		Log::Initialize();

		JobManager::CreateThreads();

		managedCore = new ManagedAssembly("ManagedCore");

		Log::AddInternalCalls();// This can't be done in Log::Initialize() because the managed core isn't created at that point in time.

		moduleManager = new ModuleManager(managedCore);

		// Load modules required by project.
		DynamicArray<String> requiredModules = loadedProject->GetRequiredModules();
		for (size_t i = 0; i < requiredModules.Count(); i++)
		{
			moduleManager->LoadModule(requiredModules[i]);
		}
		// Load the project's modules.
		moduleManager->LoadModule(loadedProject->GetName() + "_Native");
		moduleManager->LoadModule(loadedProject->GetName() + "_Managed");

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
			// At the moment, shut down right away since there's no proper way to exit the application.
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
				FileSystem::Delete(Directories::GetProjectTempDirectory());

			// Want to flush the log as late as possible to make sure all entries get written to disk.
			Log::Flush();

			delete loadedProject;

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
