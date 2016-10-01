#include "Application.h"
#include "Core.h"

namespace Vanguard
{
	BooleanConfigVar Application::fullscreen = BooleanConfigVar("Core", "Application", "Fullscreen", false);
	Int32ConfigVar Application::resolutionX = Int32ConfigVar("Core", "Application", "ResolutionX", 0);
	Int32ConfigVar Application::resolutionY = Int32ConfigVar("Core", "Application", "ResolutionY", 0);
	BooleanConfigVar Application::resizable = BooleanConfigVar("Core", "Application", "Resizable", false);

	ApplicationArguments applicationArguments = ApplicationArguments(0,nullptr);
	DynamicArray<INativeEventProcessor*> nativeEventProcessors;
	DynamicArray<INativeEventHandler*> nativeEventHandlers;
	DynamicArray<NativeWindow> nativeWindows;

	void Application::SetApplicationArguments(int aArgC, char** aArgV)
	{
		applicationArguments = ApplicationArguments(aArgC, aArgV);
	}

	void Application::ProcessNativeEvents()
	{
		for (size_t i = 0; i < nativeEventProcessors.Count(); i++)
		{
			NativeEvent nativeEvent;
			while (nativeEventProcessors[i]->GetNextEvent(nativeEvent))
			{
				DispatchNativeEvent(nativeEvent);
			}
		}
	}

	void Application::DispatchNativeEvent(NativeEvent nativeEvent)
	{
		for (size_t h = 0; h < nativeEventHandlers.Count(); h++)
		{
			nativeEventHandlers[h]->HandleNativeEvent(nativeEvent);
		}
	}

	ApplicationArguments Application::GetApplicationArguments()
	{
		return applicationArguments;
	}

	void Application::RegisterNativeEventProcessor(INativeEventProcessor* aProcessor)
	{
		nativeEventProcessors.PushBack(aProcessor);
	}

	void Application::UnregisterNativeEventProcessor(INativeEventProcessor* aProcessor)
	{
		nativeEventProcessors.Remove(aProcessor);
	}

	void Application::RegisterNativeEventHandler(INativeEventHandler* aHandler)
	{
		nativeEventHandlers.PushBack(aHandler);
	}

	void Application::UnregisterNativeEventHandler(INativeEventHandler* aHandler)
	{
		nativeEventHandlers.Remove(aHandler);
	}

	WindowCreationParameters Application::GetWindowCreationParams()
	{
		WindowCreationParameters params;
		params.fullscreen = fullscreen;
		params.title = Core::GetInstance()->GetLoadedProject()->GetFriendlyName();
		if (resolutionX)
			params.sizeX = resolutionX;
		if (resolutionY)
			params.sizeY = resolutionY;
		params.resizable = resizable;
		return params;
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