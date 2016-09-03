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

	RenderView* Renderer2D::CreateRenderView(Camera* aCamera, WindowCreationParameters aWindowParameters)
	{
		RenderView2D* newView = new RenderView2D(aCamera, aWindowParameters);
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
			const float aspectRatio = (float)screenX / (float)screenY;
			Vector2 screenSize(screenX, screenY);
			Vector2 aspectAdjustment = aspectRatio > 1 ? Vector2(1 / aspectRatio, 1) : Vector2(1, aspectRatio);

			SDL_SetRenderDrawColor(view->sdlRenderer, SPLIT_COLOR(view->clearColor));
			SDL_RenderClear(view->sdlRenderer);

			DynamicArray<SpriteComponent*> sprites = world->GetInstances<SpriteComponent>();
			for (size_t i = 0; i < sprites.Count(); i++)
			{
				SpriteComponent* sprite = sprites[i];

				if (!sprite->GetEntity()->Enabled())
					continue;

				Transform* spriteTransform = sprite->GetEntity()->GetComponent<Transform>();

				Vector2 spriteDimensions = sprite->GetDimensions();

				// World to camera
				const Matrix4x4 worldToCamera = Matrix4x4::CreateTranslation(camera->GetTransform()->position);
				Vector3 cameraSpace = worldToCamera.TransformPoint(spriteTransform->position);
				float scaleRelativeCamera = 1.0f / camera->GetFov();

				// Projection
				const Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();
				Vector3 clipSpace = projectionMatrix.TransformPoint(cameraSpace);

				// Correct for aspect ratio
				clipSpace *= Vector3(aspectAdjustment.x, aspectAdjustment.y, 1);

				// NDC space
				Vector3 ndcSpace = (clipSpace + 1.0f) / 2.0f;
				ndcSpace.y = 1 - ndcSpace.y;

				// Screenspace
				Vector3 screenspace = ndcSpace * Vector3(screenX, screenY, 1);
				Vector2 finalSize = spriteDimensions * scaleRelativeCamera * screenSize * aspectAdjustment;

				SDL_Rect spriteRect;				
				spriteRect.w = finalSize.x;
				spriteRect.h = finalSize.y;
				spriteRect.x = screenspace.x - spriteRect.w / 2;
				spriteRect.y = screenspace.y - spriteRect.h / 2;

				SDL_SetRenderDrawColor(view->sdlRenderer, SPLIT_COLOR(sprite->GetColor()));

				SDL_RenderFillRect(view->sdlRenderer, &spriteRect);
				//SDL_RenderDrawRect(view->sdlRenderer, &spriteRect);
			}
			SDL_RenderPresent(view->sdlRenderer);
		}
	}
}