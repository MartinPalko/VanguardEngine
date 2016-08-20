#include "Interfaces/IModule.h"
#include "VanguardSDL.h"

#include "Foundation.h"

#include <SDL.h>

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
			Log::Message("SDL initialized " + moduleNames[module] + " subsystem", "SDL");
		}

		moduleUsers[SDLModule::Timer]++;
	}

	void VanguardSDL::UnregisterModuleUse(SDLModule module)
	{
		moduleUsers[module]--;

		if (moduleUsers[module] == 0)
		{
			SDL_QuitSubSystem(module);
			Log::Message("SDL quit " + moduleNames[module] + " subsystem", "SDL");
		}
	}

	void VanguardSDL::LoadModule()
	{
		SDL_Init(0);
		Log::Message("SDL initialized", "SDL");

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
		Log::Message("SDL quit", "SDL");
	}
}