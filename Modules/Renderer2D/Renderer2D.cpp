#include "Renderer2D.h"

#include "VanguardSDL.h"
#include "RenderView2D.h"
#include "Entity/Transform.h"
#include "SDLImageResource.h"

#define SPLIT_COLOR_RGBA(VanguardColor) VanguardColor.r, VanguardColor.g, VanguardColor.b, VanguardColor.a
#define SPLIT_COLOR_RGB(VanguardColor) VanguardColor.r, VanguardColor.g, VanguardColor.b

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(Renderer2D)

	struct RenderItem
	{
		Color color;
		Vector2 dimensions;
		Vector3 position;
		SDL_Texture* texture;
		SDL_BlendMode blendMode;
	};

	struct JobRenderView
	{
		Matrix4x4 projectionMatrix;
		Matrix4x4 worldToCamera;
		SDL_Window* window;
		SDL_Renderer* renderer;
		Color clearColor;
	};

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

	class RenderJob : public FrameJob
	{
	public:
		DynamicArray<JobRenderView> renderViews;
		DynamicArray<RenderItem> renderItems;

		RenderJob(const String& aName, Frame* aFrame) : FrameJob(aName, aFrame, true)
		, renderViews()
		, renderItems()
		{}

		void DoJob() override
		{
			for (int i = 0; i < renderViews.Count(); i++)
			{
				const JobRenderView& view = renderViews[i];

				const World* world = frame->world;

				int screenX;
				int screenY;
				SDL_GetWindowSize(view.window, &screenX, &screenY);
				const float aspectRatio = (float)screenX / (float)screenY;
				Vector2 screenSize(screenX, screenY);
				Vector2 aspectAdjustment = aspectRatio > 1 ? Vector2(1 / aspectRatio, 1) : Vector2(1, aspectRatio);

				SDL_SetRenderDrawColor(view.renderer, SPLIT_COLOR_RGBA(view.clearColor));
				SDL_RenderClear(view.renderer);

				for (size_t i = 0; i < renderItems.Count(); i++)
				{
					const RenderItem& renderItem = renderItems[i];

					// World to camera
					Vector3 cameraSpace = view.worldToCamera.TransformPoint(renderItem.position);

					// Projection
					Vector3 clipSpace = view.projectionMatrix.TransformPoint(cameraSpace);
					Vector3 scaleReference = view.projectionMatrix.TransformPoint(cameraSpace + Vector3(0,1,0));
					float scaleRelativeCamera = (clipSpace - scaleReference).Length() / 2;

					// Correct for aspect ratio
					clipSpace *= Vector3(aspectAdjustment.x, aspectAdjustment.y, 1);

					// NDC space
					Vector3 ndcSpace = (clipSpace + 1.0f) / 2.0f;
					ndcSpace.y = 1 - ndcSpace.y;

					// Screenspace
					Vector3 screenspace = ndcSpace * Vector3(screenX, screenY, 1);
					Vector2 finalSize = renderItem.dimensions * scaleRelativeCamera * screenSize * aspectAdjustment;

					SDL_Rect screenspaceRect;				
					screenspaceRect.w = finalSize.x;
					screenspaceRect.h = finalSize.y;
					screenspaceRect.x = screenspace.x - screenspaceRect.w / 2;
					screenspaceRect.y = screenspace.y - screenspaceRect.h / 2;

					if (renderItem.texture)
					{
						SDL_SetTextureColorMod(renderItem.texture, SPLIT_COLOR_RGB(renderItem.color));
						SDL_RenderCopy(view.renderer, renderItem.texture, NULL, &screenspaceRect);
					}
					else
					{
						SDL_SetRenderDrawColor(view.renderer, SPLIT_COLOR_RGBA(renderItem.color));
						SDL_RenderFillRect(view.renderer, &screenspaceRect);
					}

				}
				SDL_RenderPresent(view.renderer);
			}
		}
	};

	void Renderer2D::StartRenderJob(Frame* aFrame)
	{
		RenderJob* renderJob = new RenderJob("Render Job", aFrame);

		// Compose render job data
		renderJob->renderViews.Reserve(renderViews.Count());
		for (int i = 0; i < renderViews.Count(); i++)
		{
			RenderView2D* worldView = renderViews[i];

			const Matrix4x4 projectionMatrix = worldView->viewCamera->GetProjectionMatrix();
			const Matrix4x4 worldToCamera = Matrix4x4::CreateTranslation(worldView->viewCamera->GetTransform()->position);

			JobRenderView jobView = {
				projectionMatrix,
				worldToCamera,
				worldView->sdlWindow,
				worldView->sdlRenderer,
				worldView->clearColor
			};

			renderJob->renderViews.PushBack(jobView);
		}

		DynamicArray<VanguardObject*> sprites = aFrame->world->GetInstances(Type::GetType<SpriteRenderer>(), true);
		renderJob->renderItems.Reserve(sprites.Count());
		for (int i = 0; i < sprites.Count(); i++)
		{
			SpriteRenderer* sprite = (SpriteRenderer*)sprites[i];
			if (sprite->GetEntity()->Enabled())
			{
				SDL_Texture* sdlTexture = nullptr;

				const SDLImageResource* imageResource = sprite->GetImage();
				if (imageResource && imageResource->sdlSurface)
				{
					sdlTexture = SDL_CreateTextureFromSurface(renderViews[0]->sdlRenderer, imageResource->sdlSurface);
				}

				RenderItem item = {
					sprite->GetColor(),
					sprite->GetDimensions(),
					sprite->GetEntity()->GetComponent<Transform>()->position,
					sdlTexture,
					SDL_BLENDMODE_NONE
				};
				renderJob->renderItems.PushBack(item);
			}
		}

		// Add job to frame & return
		aFrame->AddJob(renderJob);
	}

	
}