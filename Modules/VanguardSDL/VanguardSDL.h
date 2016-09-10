#pragma once
#include "VanguardSDL_Common.h"
#include "Interfaces/IModule.h"
#include "Application/Application.h"

namespace Vanguard
{
	class VANGUARDSDL_API VanguardSDL : public IModule , public INativeEventProcessor
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

		// Implement IModule
		virtual void LoadModule();
		virtual void UnloadModule();

		// Implement INativeEventProcessor
		virtual bool GetNextEvent(NativeEvent& aOutNextEvent) override;
	};
}