#include "Application.h"

namespace Vanguard
{
	ApplicationArguments Application::applicationArguments(0,nullptr);

	ApplicationArguments Application::GetApplicationArguments()
	{
		return applicationArguments;
	}
}

#if VANGUARD_WINDOWS
#include "WindowsApplication.cpp.h"
#elif VANGUARD_LINUX
#include "LinuxApplication.cpp.h"
#elif VANGUARD_OSX
#include "OSXApplication.cpp.h"
#else
#error "No application implementation for platform!"
#endif