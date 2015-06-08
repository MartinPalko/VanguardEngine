#include "Core.h"
#include <ostream>

#include "ModuleManager.h"
#include "Mono.h"

namespace Vanguard
{
	void Core::Initialize()
	{
		ConfigTable::LoadConfigFromDisk();

		moduleManager = new ModuleManager();
		monoInstance = new Mono();

		std::cout << "Initialized Core" << "\n" << "\n";
	}

	void Core::Run()
	{
		std::cout << "Ran Core" << "\n";
	}

	void Core::ShutDown()
	{
		delete moduleManager;

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