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

		// Register with vanguard application so it will handle processing events for this window.
		#if defined(VANGUARD_WINDOWS) && defined(SDL_VIDEO_DRIVER_WINDOWS)
		Application::RegisterNativeWindow(NativeWindow{ windowInfo.info.win.window });
		#elif defined(VANGUARD_LINUX) && defined(SDL_VIDEO_DRIVER_X11)
		Application::RegisterNativeWindow(NativeWindow{ (void*)windowInfo.info.x11.window, (void*)windowInfo.info.x11.display });
		#else
		#error Unknown SDL video driver
		#endif

		if (!sdlWindow)
		{
			Log::Exception("Failed to create SDL window!", "Renderer2d");
		}

		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
		
		if (!sdlRenderer)
		{
			Log::Exception("Failed to create SDL renderer!", "Renderer2d");
		}
	}

	RenderView2D::~RenderView2D()
	{
		SDL_DestroyRenderer(sdlRenderer);
		SDL_DestroyWindow(sdlWindow);
	}
}