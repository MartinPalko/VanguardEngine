#pragma once
#include "Renderer2D_Common.h"
#include "Interfaces/IModule.h"
#include "Renderer/IRenderer.h"

#include "SpriteRenderer.h"
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
		virtual RenderView* CreateRenderView(Camera* aCamera, WindowCreationParameters aWindowParameters) override;
		virtual void StartRenderJob(Frame* aFrame) override;
	};
}