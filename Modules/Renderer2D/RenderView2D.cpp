#include "VanguardSDL.h"
#include "SDL_syswm.h"
#include "RenderView2D.h"

#ifdef VANGUARD_LINUX
// // Hack from: http://forums.libsdl.org/viewtopic.php?p=44634&sid=ac3883de654fcade89ffea73492c2a05
struct SDL_Window 
{
	const void *magic; 
	Uint32 id; 
	char *title; 
	SDL_Surface *icon; 
	int x, y; 
	int w, h; 
	int min_w, min_h; 
	int max_w, max_h; 
	Uint32 flags; 
}; 
typedef struct SDL_Window SDL_Window; 
#endif

namespace Vanguard
{
	RenderView2D::RenderView2D(Camera* aCamera, WindowCreationParameters aWindowParameters)
		: RenderView(aCamera)
		, sdlWindow(nullptr)
		, sdlRenderer(nullptr)
	{
		int windowFlags = SDL_WINDOW_OPENGL;

		if (aWindowParameters.fullscreen)
			windowFlags |= SDL_WINDOW_FULLSCREEN;

		if (aWindowParameters.resizable)
			windowFlags |= SDL_WINDOW_RESIZABLE;

		sdlWindow = SDL_CreateWindowFrom(Application::CreateNativeWindow(aWindowParameters));

		if (!sdlWindow)
		{
			Log::Exception("Failed to create SDL window!", "Renderer2d");
		}

#ifdef VANGUARD_LINUX
		sdlWindow->flags |= SDL_WINDOW_OPENGL; 
		SDL_GL_LoadLibrary(NULL); 
#endif

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