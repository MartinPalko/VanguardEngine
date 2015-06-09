#include "Core.h"
#include <ostream>

#include "ModuleManager.h"
#include "ManagedAssembly.h"

namespace Vanguard
{
	void Core::Initialize()
	{
		ConfigTable::LoadConfigFromDisk();
		ConfigTable::SaveConfigToDisk(); // Save right away, to generate defaults if they don't exist. TODO: More elequently

		moduleManager = new ModuleManager();
		managedCore = new ManagedAssembly("ManagedCore");

		std::cout << "Initialized Core" << "\n" << "\n";
	}

	void Core::Run()
	{
		std::cout << "Ran Core" << "\n";
	}

	void Core::ShutDown()
	{
		delete moduleManager;
		delete managedCore;

		std::cout << "Shut Down Core" << "\n";

		ConfigTable::SaveConfigToDisk();
	}

	String Core::ReturningFoundationType()
	{
		return "ReturnValue";
	}
	void Core::TakingFoundationType(String aArgument)
	{
		std::cout << aArgument << "\n";
	}
}
