#include "Core.h"
#include <ostream>

#include "ModuleManager.h"

namespace Vanguard
{
	void Core::Initialize()
	{
		moduleManager = new ModuleManager();

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