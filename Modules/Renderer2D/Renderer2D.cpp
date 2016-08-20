#include "Renderer2D.h"

#include "VanguardSDL.h"
#include "RenderView2D.h"

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(Renderer2D)

	void Renderer2D::LoadModule()
	{
		VanguardSDL::RegisterModuleUse(VanguardSDL::Video);

		Core::GetInstance()->RegisterRenderer(this);
	}

	void Renderer2D::UnloadModule()
	{
		for (int i = 0; i < renderViews.Count(); i++)
		{
			delete renderViews[i];
		}
		renderViews.Clear();

		Core::GetInstance()->UnregisterRenderer(this);

		VanguardSDL::UnregisterModuleUse(VanguardSDL::Video);
	}

	String Renderer2D::RendererName()
	{
		return "2D";
	}

	RenderView* Renderer2D::CreateRenderView(Camera* aCamera, int aResolutionX, int aResolutionY)
	{
		RenderView2D* newView = new RenderView2D(aCamera, aResolutionX, aResolutionY);
		renderViews.PushBack(newView);
		return newView;
	}

	void Renderer2D::RenderViews()
	{
		// TEMP: For now, just pump SDL events before rendering views.
		SDL_PumpEvents();

		for (int i = 0; i < renderViews.Count(); i++)
		{
			RenderView2D* view = renderViews[i];

			World* world = view->viewCamera->GetWorld();

			DynamicArray<SpriteComponent*> sprites = world->GetInstances<SpriteComponent>();
			DEBUG_LOG("Would be rendering " + String::FromInt32(sprites.Count()) + " sprites");

			SDL_RenderClear(view->sdlRenderer);
			SDL_RenderPresent(view->sdlRenderer);
		}
	}
}