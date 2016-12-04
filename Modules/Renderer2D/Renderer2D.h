#pragma once
#include "Renderer2D_Common.h"
#include "Interfaces/IModule.h"
#include "Renderer/IRenderer.h"

#include "RenderView2D.h"

namespace Vanguard
{
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
		static Int32ConfigVar defaultTextureFiltering;

		DynamicArray<RenderView2D*> renderViews;
		DynamicArray<IDebugDraw*> debugDraws;

		// IModule Implementation
		virtual void LoadModule() override;
		virtual void UnloadModule() override;

		// IRenderer Implementation
		virtual String RendererName() override;
		virtual RenderView* CreateRenderView(Camera* aCamera, WindowCreationParameters aWindowParameters) override;
		virtual void RegisterDebugDraw(IDebugDraw* aDebugDraw) override;
		virtual void UnregisterDebugDraw(IDebugDraw* aDebugDraw) override;
		virtual void StartRenderJob(Frame* aFrame) override;
		virtual void Present(Frame* aFrame) override;
	};
}