#pragma once
#include "Renderer/RenderView.h"
#include "Renderer/Camera.h"
#include "VanguardSDL.h"

namespace Vanguard
{
	class RenderView2D : public RenderView
	{
	public:
		SDL_Window* sdlWindow;
		SDL_Renderer* sdlRenderer;
		Color clearColor;
		WindowHandle windowHandle;

		RenderView2D(Camera* aViewCamera, int aResolutionX, int aResolutionY);
		~RenderView2D();

		virtual WindowHandle GetWindowHandle() override;
	};
}