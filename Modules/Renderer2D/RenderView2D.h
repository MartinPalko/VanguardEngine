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

		RenderView2D(Camera* aCamera, WindowCreationParameters aWindowParameters);
		~RenderView2D();

		virtual NativeWindow GetWindowHandle() override;
		virtual void Resize(int aWidth, int aHeight) override;
		virtual void GetSize(int& OutWidth, int& OutHeight) override;
	};
}