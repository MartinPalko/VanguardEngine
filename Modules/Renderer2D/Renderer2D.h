#include "Renderer2D_Common.h"
#include "Interfaces/IModule.h"
#include "Renderer/IRenderer.h"

#include "SpriteComponent.h"
#include "RenderView2D.h"

namespace Vanguard
{		
	enum class ERenderItemType
	{
		rectangle,
	};

	struct RenderItem
	{
		ERenderItemType type;
		Color color;
		Vector2 dimensions;
		Vector3 position;
	};

	struct JobRenderView
	{
		Matrix4x4 projectionMatrix;
		Matrix4x4 worldToCamera;
		SDL_Window* window;
		SDL_Renderer* renderer;
		Color clearColor;
	};

	class Renderer2D : public IModule, public IRenderer
	{
		DynamicArray<RenderView2D*> renderViews;

		// IModule Implementation
		virtual void LoadModule() override;
		virtual void UnloadModule() override;

		// IRenderer Implementation
		virtual String RendererName() override;
		virtual RenderView* CreateRenderView(Camera* aCamera, WindowCreationParameters aWindowParameters) override;
		virtual Job* StartRenderJob(Frame* aFrame) override;
	};
}