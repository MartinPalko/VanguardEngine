#include "Renderer2D.h"

#include "VanguardSDL.h"
#include "RenderView2D.h"
#include "WorldObjects/Transform.h"
#include "SDLImageResource.h"

#define SPLIT_COLOR_RGBA(VanguardColor) VanguardColor.r, VanguardColor.g, VanguardColor.b, VanguardColor.a
#define SPLIT_COLOR_RGB(VanguardColor) VanguardColor.r, VanguardColor.g, VanguardColor.b

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(Renderer2D);

	Int32ConfigVar Renderer2D::defaultTextureFiltering = Int32ConfigVar("Renderer2D", "", "DefaultTextureFiltering", 1);

	struct RenderItem
	{
		Color color;
		Vector2 dimensions;
		Vector3 position;
		double rotation;
		SDL_RendererFlip flip;
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

		switch (defaultTextureFiltering)
		{
		case 0:
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
			break;
		case 1:
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
			break;
		case 2:
		default:
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
			break;
		}
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
		JobRenderView renderView;
		DynamicArray<RenderItem> renderItems;

		RenderJob(const String& aName, Frame* aFrame, JobRenderView aView) : FrameJob(aName, aFrame, true)
			, renderView(aView)
			, renderItems()
		{}

		void DoJob() override
		{
			const World* world = frame->GetWorld();

			int screenX;
			int screenY;
			SDL_GetWindowSize(renderView.window, &screenX, &screenY);
			const float aspectRatio = (float)screenX / (float)screenY;
			Vector2 screenSize(screenX, screenY);
			Vector2 aspectAdjustment = aspectRatio > 1 ? Vector2(1 / aspectRatio, 1) : Vector2(1, aspectRatio);

			SDL_SetRenderDrawColor(renderView.renderer, SPLIT_COLOR_RGBA(renderView.clearColor));
			SDL_RenderClear(renderView.renderer);

			for (size_t i = 0; i < renderItems.Count(); i++)
			{
				const RenderItem& renderItem = renderItems[i];

				// World to camera
				Vector3 cameraSpace = renderView.worldToCamera.TransformPoint(renderItem.position);

				// Projection
				Vector3 clipSpace = renderView.projectionMatrix.TransformPoint(cameraSpace);
				Vector3 scaleReference = renderView.projectionMatrix.TransformPoint(cameraSpace + Vector3(0,1,0));
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
					const static SDL_Point zeroPoint = { 0, 0 };

					SDL_SetTextureBlendMode(renderItem.texture, renderItem.blendMode);
					SDL_SetTextureColorMod(renderItem.texture, SPLIT_COLOR_RGB(renderItem.color));
					SDL_SetTextureAlphaMod(renderItem.texture, renderItem.color.a);
					SDL_RenderCopyEx(renderView.renderer, renderItem.texture, NULL, &screenspaceRect, renderItem.rotation, &zeroPoint, renderItem.flip);
				}
				else
				{
					SDL_SetRenderDrawColor(renderView.renderer, SPLIT_COLOR_RGBA(renderItem.color));
					SDL_RenderFillRect(renderView.renderer, &screenspaceRect);
				}

			}
			SDL_RenderPresent(renderView.renderer);
		}
	};

	void Renderer2D::StartRenderJob(Frame* aFrame)
	{
		// Compose render job data
		for (int i = 0; i < renderViews.Count(); i++)
		{
			RenderView2D* worldView = renderViews[i];

			const Matrix4x4 projectionMatrix = worldView->viewCamera->GetProjectionMatrix();
			const Matrix4x4 worldToCamera = Matrix4x4::CreateTranslation(worldView->viewCamera->GetTransform()->position).GetInverse();

			RenderJob* renderJob = new RenderJob("Render Job", aFrame,
			{
				projectionMatrix,
				worldToCamera,
				worldView->sdlWindow,
				worldView->sdlRenderer,
				worldView->clearColor
			});

			DynamicArray<WorldObject*> sprites = aFrame->GetWorld()->GetInstances(Type::GetType<SpriteRenderer>(), true);
			renderJob->renderItems.Reserve(sprites.Count());
			for (int s = 0; s < sprites.Count(); s++)
			{
				SpriteRenderer* sprite = (SpriteRenderer*)sprites[s];
				if (sprite->GetEntity()->Enabled())
				{
					SDL_Texture* sdlTexture = nullptr;
					SDLImageResource* imageResource = sprite->GetImage();
					if (imageResource)
						sdlTexture = imageResource->getSDLTexture(worldView->sdlRenderer);

					RenderItem item = {
						sprite->GetColor(),
						sprite->GetDimensions(),
						sprite->GetEntity()->GetComponent<Transform>()->position,
						0, // TODO: Support rotation from Transform
						(SDL_RendererFlip)sprite->GetFlipped(),
						sdlTexture,
						(SDL_BlendMode)sprite->GetBlendMode()
					};
					renderJob->renderItems.PushBack(item);
				}
			}

			// Add job to frame & return
			aFrame->AddJob(renderJob);
		}
	}

	
}