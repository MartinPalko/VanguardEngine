#include "Interfaces/IModule.h"
#include "VanguardSDL.h"

#include "Foundation.h"

#ifdef VANGUARD_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <SDL_syswm.h>

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(VanguardSDL)

	Hashtable<int32> moduleUsers;
	Hashtable<String> moduleNames;
	

	void VanguardSDL::RegisterModuleUse(SDLModule module)
	{
		if (moduleUsers[module] == 0)
		{
			SDL_InitSubSystem(module);
			LOG_MESSAGE("SDL initialized " + moduleNames[module] + " subsystem", "SDL");
		}

		// Ensure native window manager events are enabled.
		SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

		moduleUsers[SDLModule::Timer]++;
	}

	void VanguardSDL::UnregisterModuleUse(SDLModule module)
	{
		moduleUsers[module]--;

		if (moduleUsers[module] == 0)
		{
			SDL_QuitSubSystem(module);
			LOG_MESSAGE("SDL quit " + moduleNames[module] + " subsystem", "SDL");
		}
	}

	void VanguardSDL::LoadModule()
	{
		SDL_Init(0);

		Application::RegisterNativeEventProcessor(this);

		LOG_MESSAGE("SDL initialized", "SDL");

		// Initialize all module users to 0.
		moduleUsers[SDLModule::Timer] = 0;
		moduleUsers[SDLModule::Audio] = 0;
		moduleUsers[SDLModule::Video] = 0;
		moduleUsers[SDLModule::Joystick] = 0;
		moduleUsers[SDLModule::Haptic] = 0;
		moduleUsers[SDLModule::GameController] = 0;
		moduleUsers[SDLModule::Events] = 0;

		// Keep hashtable of module names for debugging purposes.
		moduleNames[SDLModule::Timer] = "Timer";
		moduleNames[SDLModule::Audio] = "Audio";
		moduleNames[SDLModule::Video] = "Video";
		moduleNames[SDLModule::Joystick] = "Joystick";
		moduleNames[SDLModule::Haptic] = "Haptic";
		moduleNames[SDLModule::GameController] = "GameController";
		moduleNames[SDLModule::Events] = "Events";
	}

	void VanguardSDL::UnloadModule()
	{
		SDL_Quit();
		Application::UnregisterNativeEventProcessor(this);
		LOG_MESSAGE("SDL quit", "SDL");
	}

	bool VanguardSDL::GetNextEvent(NativeEvent& aOutNextEvent)
	{
		int enabled = SDL_EventState(SDL_SYSWMEVENT, SDL_QUERY);

		SDL_Event sdlEvent;
		if (SDL_PollEvent(&sdlEvent))
		{
			if (sdlEvent.type == SDL_QUIT)
			{
				Core::GetInstance()->ShutDown();
			}

			if (sdlEvent.type == SDL_SYSWMEVENT)
			{
				SDL_SysWMmsg* sdlSysMsg = sdlEvent.syswm.msg;

#if defined(VANGUARD_WINDOWS)
				static MSG lastWinMsg;
				// Reconstruct a windows message to send back.
				lastWinMsg.hwnd = sdlSysMsg->msg.win.hwnd;
				lastWinMsg.message = sdlSysMsg->msg.win.msg;
				lastWinMsg.wParam = sdlSysMsg->msg.win.wParam;
				lastWinMsg.lParam = sdlSysMsg->msg.win.lParam;
				lastWinMsg.time = sdlEvent.syswm.timestamp;
				GetCursorPos(&lastWinMsg.pt);
				aOutNextEvent = &lastWinMsg;
				return true;
#elif defined(VANGUARD_LINUX)
				aOutNextEvent = (NativeEvent)(&sdlSysMsg->msg.x11.event);
				return true;
#endif
			}
		}

		return false;
	}
}