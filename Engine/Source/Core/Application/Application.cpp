#include "Application.h"

namespace Vanguard
{
	ApplicationArguments applicationArguments = ApplicationArguments(0,nullptr);
	DynamicArray<INativeEventHandler*> nativeEventHandlers;
	DynamicArray<WindowHandle> nativeWindows;

	void Application::SetApplicationArguments(int aArgC, char** aArgV)
	{
		applicationArguments = ApplicationArguments(aArgC, aArgV);
	}

	ApplicationArguments Application::GetApplicationArguments()
	{
		return applicationArguments;
	}

	void Application::RegisterNativeEventHandler(INativeEventHandler* aHandler)
	{
		nativeEventHandlers.PushBack(aHandler);
	}

	void Application::UnregisterNativeEventHandler(INativeEventHandler* aHandler)
	{
		nativeEventHandlers.Remove(aHandler);
	}

	void Application::RegisterNativeWindow(WindowHandle aWindowHandle)
	{
		nativeWindows.PushBack(aWindowHandle);
	}

	void Application::UnregisterNativeWindow(WindowHandle aHandler)
	{
		nativeWindows.Remove(aHandler);
	}
}

// Platform specific implementations.
#if VANGUARD_WINDOWS
#include "WindowsApplication.cpp.h"
#elif VANGUARD_LINUX
#include "LinuxApplication.cpp.h"
#elif VANGUARD_OSX
#include "OSXApplication.cpp.h"
#else
#error "No application implementation for platform!"
#endif