#include "Renderer2D.h"

#include "VanguardSDL.h"
#include "RenderView2D.h"
#include "Entity/Transform.h"

#define SPLIT_COLOR(VanguardColor) VanguardColor.r, VanguardColor.g, VanguardColor.b, VanguardColor.a

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

	RenderView* Renderer2D::CreateRenderView(WindowHandle aWindowHandle, Camera* aCamera)
	{
		RenderView2D* newView = new RenderView2D(aWindowHandle, aCamera);
		renderViews.PushBack(newView);
		return newView;
	}

	void Renderer2D::RenderViews()
	{
		for (int i = 0; i < renderViews.Count(); i++)
		{
			RenderView2D* view = renderViews[i];

			Camera* camera = view->viewCamera;
			World* world = camera->GetWorld();

			int screenX;
			int screenY;
			SDL_GetWindowSize(view->sdlWindow, &screenX, &screenY);
			Vector2 screenSize(screenX, screenY);

			SDL_SetRenderDrawColor(view->sdlRenderer, SPLIT_COLOR(view->clearColor));
			SDL_RenderClear(view->sdlRenderer);

			DynamicArray<SpriteComponent*> sprites = world->GetInstances<SpriteComponent>();
			for (size_t i = 0; i < sprites.Count(); i++)
			{
				SpriteComponent* sprite = sprites[i];
				Vector2 spriteDimensions = sprite->GetDimensions();

				Transform* transform = sprite->GetEntity()->GetComponent<Transform>();
				Vector2 spritePosition = Vector2(transform->position.x, transform->position.y);

				// Scale to screen space
				spriteDimensions *= screenSize;
				spritePosition *= screenSize;

				SDL_Rect spriteRect;				
				spriteRect.w = spriteDimensions.x;
				spriteRect.h = spriteDimensions.y;
				spriteRect.x = spritePosition.x;
				spriteRect.y = spritePosition.y;

				SDL_SetRenderDrawColor(view->sdlRenderer, SPLIT_COLOR(sprite->GetColor()));

				SDL_RenderFillRect(view->sdlRenderer, &spriteRect);
				//SDL_RenderDrawRect(view->sdlRenderer, &spriteRect);
			}
			SDL_RenderPresent(view->sdlRenderer);
		}
	}
}