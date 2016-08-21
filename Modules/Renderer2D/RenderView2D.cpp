#include "VanguardSDL.h"
#include "SDL_syswm.h"
#include "RenderView2D.h"

namespace Vanguard
{
	RenderView2D::RenderView2D(WindowHandle aWindowHandle, Camera* aCamera)
		: RenderView(aWindowHandle, aCamera)
		, clearColor(0x33, 0x33, 0x33, 0x00)
	{
		sdlWindow = SDL_CreateWindowFrom(aWindowHandle);

		if (!sdlWindow)
		{
			Log::Exception("Couldn't Create SDL Window!", "Renderer2d");
		}

		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!sdlRenderer)
		{
			Log::Exception("Couldn't create SDL renderer!", "Renderer2d");
		}
	}

	RenderView2D::~RenderView2D()
	{
		SDL_DestroyRenderer(sdlRenderer);
		SDL_DestroyWindow(sdlWindow);
	}
}