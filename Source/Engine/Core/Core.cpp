#include "Core.h"
#include <ostream>

namespace Vanguard
{
	void Core::Initialize()
	{
		std::cout << "Initialized Core" << "\n" << "\n";
	}

	void Core::Run()
	{
		std::cout << "Ran Core" << "\n";
	}

	void Core::ShutDown()
	{
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