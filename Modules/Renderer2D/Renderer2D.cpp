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

	class RenderJob : public FrameJob
	{
	public:
		DynamicArray<JobRenderView> renderViews;
		DynamicArray<RenderItem> renderItems;

		RenderJob(const String& aName, Frame* aFrame) : FrameJob(aName, aFrame)
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

				SDL_SetRenderDrawColor(view.renderer, SPLIT_COLOR(view.clearColor));
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

					SDL_Rect screenRect;				
					screenRect.w = finalSize.x;
					screenRect.h = finalSize.y;
					screenRect.x = screenspace.x - screenRect.w / 2;
					screenRect.y = screenspace.y - screenRect.h / 2;

					SDL_SetRenderDrawColor(view.renderer, SPLIT_COLOR(renderItem.color));

					switch (renderItem.type)
					{
					case ERenderItemType::rectangle:
						SDL_RenderFillRect(view.renderer, &screenRect);
					default:
						break;
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

		DynamicArray<VanguardObject*> sprites = aFrame->world->GetInstances(Type::GetType<SpriteComponent>());
		renderJob->renderItems.Reserve(sprites.Count());
		for (int i = 0; i < sprites.Count(); i++)
		{
			SpriteComponent* sprite = (SpriteComponent*)sprites[i];
			if (sprite->GetEntity()->Enabled())
			{
				RenderItem item = {
					ERenderItemType::rectangle,
					sprite->GetColor(),
					sprite->GetDimensions(),
					sprite->GetEntity()->GetComponent<Transform>()->position
				};
				renderJob->renderItems.PushBack(item);
			}
		}

		// Add job to frame & return
		aFrame->AddJob(renderJob);
	}

	
}