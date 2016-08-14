#include "Renderer2D_Common.h"
#include "Modules/IModule.h"
#include "Renderer/IRenderer.h"

#include "SpriteComponent.h"
#include "RenderView2D.h"

namespace Vanguard
{
	class Renderer2D : public IModule, public IRenderer
	{
		DynamicArray<RenderView2D*> renderViews;

		// IModule Implementation
		virtual void LoadModule() override;
		virtual void UnloadModule() override;

		// IRenderer Implementation
		virtual String RendererName() override;
		virtual RenderView* CreateRenderView(Camera* aCamera, int aResolutionX, int aResolutionY) override;
		virtual void RenderViews() override;		
	};
}