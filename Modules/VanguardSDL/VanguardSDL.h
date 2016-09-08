#pragma once
#include "VanguardSDL_Common.h"
#include "Interfaces/IModule.h"

namespace Vanguard
{
	class VANGUARDSDL_API VanguardSDL : public IModule
	{
	public:
		enum SDLModule : int32
		{
			Timer = SDL_INIT_TIMER,
			Audio = SDL_INIT_AUDIO,
			Video = SDL_INIT_VIDEO,
			Joystick = SDL_INIT_JOYSTICK,
			Haptic = SDL_INIT_HAPTIC,
			GameController = SDL_INIT_GAMECONTROLLER,
			Events = SDL_INIT_EVENTS
		};

		static void RegisterModuleUse(SDLModule module);
		static void UnregisterModuleUse(SDLModule module);

		virtual void LoadModule();
		virtual void UnloadModule();
	};
}