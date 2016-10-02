#pragma once
#include "Core_Common.h"
#include "RenderView.h"
#include "Jobs/Job.h"
#include "Application/Application.h"


namespace Vanguard
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {}
		virtual String RendererName() = 0;
		virtual RenderView* CreateRenderView(Camera* aCamera) { return CreateRenderView(aCamera, Application::GetWindowCreationParams()); };
		virtual RenderView* CreateRenderView(Camera* aCamera, WindowCreationParameters aWindowParameters) = 0;
		virtual void StartRenderJob(Frame* aFrame) = 0;
		virtual void Present(Frame* aFrame) = 0;
	};
}