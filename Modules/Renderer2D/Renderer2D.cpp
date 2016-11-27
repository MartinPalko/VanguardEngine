#include "Renderer2D.h"

#include "VanguardSDL.h"
#include "RenderView2D.h"
#include "WorldObjects/Transform.h"
#include "SDLImageResource.h"
#include "ResourceManager.h"
#include "RenderableComponent2D.h"

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
		Alignment alignment;
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
		VanguardSDL::RegisterModuleUse(VanguardSDL::TTFFont);
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

		VanguardSDL::UnregisterModuleUse(VanguardSDL::TTFFont);
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

		RenderJob(const String& aName, Frame* aFrame, const JobRenderView& aView) : FrameJob(aName, aFrame, true)
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

			// Sort by Z-value
			renderItems.Sort([](const RenderItem& a, const RenderItem& b)->bool{return a.position.z < b.position.z;});

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
				Vector2 screenspace = Vector2(screenX * ndcSpace.x, screenY * ndcSpace.y);
				Vector2 finalSize = renderItem.dimensions * scaleRelativeCamera * screenSize * aspectAdjustment;

				SDL_Rect screenspaceRect;
				screenspaceRect.w = finalSize.x;
				screenspaceRect.h = finalSize.y;

				SDL_Point rotationPivot;

				switch (renderItem.alignment.GetVertical())
				{
				case eVerticalAlignment::Top:
					screenspaceRect.y = screenspace.y - screenspaceRect.h;
					rotationPivot.y = screenspaceRect.h;
					break;
				case eVerticalAlignment::Bottom:
					screenspaceRect.y = screenspace.y;
					rotationPivot.y = 0;
					break;
				case eVerticalAlignment::Center:
				default:
					screenspaceRect.y = screenspace.y - screenspaceRect.h / 2;
					rotationPivot.y = screenspaceRect.h / 2;
					break;
				}

				switch (renderItem.alignment.GetHorizontal())
				{
				case eHorizontalAlignment::Left:
					screenspaceRect.x = screenspace.x - screenspaceRect.w;
					rotationPivot.x = screenspaceRect.h;
					break;
				case eHorizontalAlignment::Right:
					screenspaceRect.x = screenspace.x;
					rotationPivot.x = 0;
					break;
				case eHorizontalAlignment::Center:
				default:
					screenspaceRect.x = screenspace.x - screenspaceRect.w / 2;
					rotationPivot.x = screenspaceRect.w / 2;
					break;
				}

				double angle = (renderItem.rotation * 180) / Math::Pi;

				if (renderItem.texture)
				{
					const static SDL_Point zeroPoint = { 0, 0 };

					SDL_SetTextureBlendMode(renderItem.texture, renderItem.blendMode);
					SDL_SetTextureColorMod(renderItem.texture, SPLIT_COLOR_RGB(renderItem.color));
					SDL_SetTextureAlphaMod(renderItem.texture, renderItem.color.a);
					SDL_RenderCopyEx(renderView.renderer, renderItem.texture, NULL, &screenspaceRect, angle, &rotationPivot, renderItem.flip);
				}
				else
				{
					SDL_SetRenderDrawColor(renderView.renderer, SPLIT_COLOR_RGBA(renderItem.color));
					SDL_RenderFillRect(renderView.renderer, &screenspaceRect);
				}
			}
		}
	};

	void Renderer2D::StartRenderJob(Frame* aFrame)
	{
		// Compose render job data
		for (RenderView2D* worldView : renderViews)
		{
			if (worldView->viewCamera->GetWorld() != aFrame->GetWorld())
				continue;

			const Matrix4x4 projectionMatrix = worldView->viewCamera->GetProjectionMatrix();
			const Matrix4x4 worldToCamera = Matrix4x4::CreateTranslation(worldView->viewCamera->GetTransform()->GetPosition()).GetInverse();

			RenderJob* renderJob = new RenderJob("Render Job", aFrame,
			{
				projectionMatrix,
				worldToCamera,
				worldView->sdlWindow,
				worldView->sdlRenderer,
				worldView->clearColor
			});

			DynamicArray<WorldObject*> renderables = aFrame->GetWorld()->GetInstances(Type::GetType<RenderableComponent2D>(), true);
			renderJob->renderItems.Reserve(renderables.Count());
			for (int s = 0; s < renderables.Count(); s++)
			{
				RenderableComponent2D* renderable = static_cast<RenderableComponent2D*>(renderables[s]);
				if (renderable->GetEntity()->Enabled())
				{

					RenderItem item = {
						renderable->GetColor(),
						renderable->GetDimensions(),
						renderable->GetEntity()->GetComponent<Transform>()->GetPosition(),
						renderable->GetEntity()->GetComponent<Transform>()->GetRotation2D(),
						(SDL_RendererFlip)renderable->GetFlipped(),
						renderable->GetSDLTexture(worldView->sdlRenderer),
						(SDL_BlendMode)renderable->GetBlendMode(),
						renderable->GetAlignment()
					};
					renderJob->renderItems.PushBack(item);
				}
			}

			// Add job to frame & return
			aFrame->AddJob(renderJob);
		}
	}

	void Renderer2D::Present(Frame* aFrame)
	{
		for (RenderView2D* worldView : renderViews)
		{
			if (worldView->viewCamera->GetWorld() == aFrame->GetWorld())
			{
				SDL_RenderPresent(worldView->sdlRenderer);
			}
		}		
	}
}