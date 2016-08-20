#include "VanguardSDL.h"
#include "SDL_syswm.h"
#include "RenderView2D.h"

namespace Vanguard
{
	RenderView2D::RenderView2D(Camera* aViewCamera, int aResolutionX, int aResolutionY)
		: RenderView(aViewCamera, aResolutionX, aResolutionY)
		, clearColor(0x33, 0x33, 0x33, 0x00)
		, windowHandle(nullptr)
	{
		sdlWindow = SDL_CreateWindow("Vanguard Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aResolutionX, aResolutionY, SDL_WINDOW_SHOWN);
		if (!sdlWindow)
		{
			Log::Exception("Couldn't Create SDL Window!", "Renderer2d");
		}

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		if (SDL_GetWindowWMInfo(sdlWindow, &wmInfo))
		{
#if VANGUARD_WINDOWS
			windowHandle = wmInfo.info.win.window;
#else
			// TODO:
#endif
		}

		if (windowHandle)
		{
			Application::RegisterNativeWindow(windowHandle);
		}

		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!sdlRenderer)
		{
			Log::Exception("Couldn't create SDL renderer!", "Renderer2d");
		}
	}

	RenderView2D::~RenderView2D()
	{
		Application::UnregisterNativeWindow(GetWindowHandle());

		SDL_DestroyRenderer(sdlRenderer);
		SDL_DestroyWindow(sdlWindow);
	}

	WindowHandle RenderView2D::GetWindowHandle()
	{
		return windowHandle;
	}
}