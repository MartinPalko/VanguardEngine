#include "VanguardSDL.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include "RenderView2D.h"

namespace Vanguard
{
	RenderView2D::RenderView2D(Camera* aCamera, WindowCreationParameters aWindowParameters)
		: RenderView(aCamera)
		, sdlWindow(nullptr)
		, sdlRenderer(nullptr)
	{
		int windowFlags = 0;

		if (aWindowParameters.fullscreen)
			windowFlags |= SDL_WINDOW_FULLSCREEN;

		if (aWindowParameters.resizable)
			windowFlags |= SDL_WINDOW_RESIZABLE;

		sdlWindow = SDL_CreateWindow(aWindowParameters.title.GetCharPointer(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			aWindowParameters.sizeX,
			aWindowParameters.sizeY,
			windowFlags);

		SDL_SysWMinfo windowInfo;
		SDL_VERSION(&windowInfo.version);
		SDL_GetWindowWMInfo(sdlWindow, &windowInfo);

		if (!sdlWindow)
		{
			LOG_EXCEPTION("Failed to create SDL window!", "Renderer2d");
		}

		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
		
		if (!sdlRenderer)
		{
			LOG_EXCEPTION("Failed to create SDL renderer!", "Renderer2d");
		}
	}

	RenderView2D::~RenderView2D()
	{
		SDL_DestroyRenderer(sdlRenderer);
		SDL_DestroyWindow(sdlWindow);
	}

	NativeWindow RenderView2D::GetWindowHandle()
	{
		SDL_SysWMinfo windowInfo;
		SDL_VERSION(&windowInfo.version);
		SDL_GetWindowWMInfo(sdlWindow, &windowInfo);

		NativeWindow nativeWindow;

#if VANGUARD_WINDOWS
		nativeWindow.handle = windowInfo.info.win.window;
#endif

		return nativeWindow;
	}
}