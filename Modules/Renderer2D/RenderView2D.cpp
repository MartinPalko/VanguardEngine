#include "RenderView2D.h"

#include "VanguardSDL.h"

namespace Vanguard
{
	RenderView2D::RenderView2D(Camera* aViewCamera, int aResolutionX, int aResolutionY) 
		: RenderView(aViewCamera, aResolutionX, aResolutionY)
	{
		sdlWindow = SDL_CreateWindow("Vanguard Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, aResolutionX, aResolutionY, SDL_WINDOW_SHOWN);
		if (!sdlWindow)
		{
			Log::Exception("Couldn't Create SDL Window!", "Renderer2d");
		}

		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!sdlWindow)
		{
			Log::Exception("Couldn't create SDL renderer!", "Renderer2d");
		}

		SDL_SetRenderDrawColor(sdlRenderer, 0x33, 0x33, 0x33, 0x00);
	}

	RenderView2D::~RenderView2D()
	{
		SDL_DestroyRenderer(sdlRenderer);
		SDL_DestroyWindow(sdlWindow);
	}
}