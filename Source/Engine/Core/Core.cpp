#include "Core.h"
#include <ostream>

#include "ManagedClass.h"
#include "ModuleManager.h"
#include "ManagedAssembly.h"

namespace Vanguard
{
	void Core::Initialize()
	{
		ConfigTable::LoadConfigFromDisk();
		ConfigTable::SaveConfigToDisk(); // Save right away, to generate defaults if they don't exist. TODO: More elequently

		managedCore = new ManagedAssembly("ManagedCore");
		moduleManager = new ModuleManager(managedCore);

		moduleManager->LoadModule("PhysX");
		moduleManager->LoadModule("ManagedModule");

		std::cout << "Initialized Core" << "\n\n";
	}

	void Core::Run()
	{
		std::cout << "Ran Core" << "\n\n";
	}

	void Core::ShutDown()
	{
		delete moduleManager;
		delete managedCore;

		std::cout << "Shut Down Core" << "\n\n";

		ConfigTable::SaveConfigToDisk();
	}

	String Core::ReturningFoundationType()
	{
		return "ReturnValue";
	}
}
