#pragma once
#include "RenderView.h"

namespace Vanguard
{
	class IRenderer
	{
	public:
		virtual String RendererName() = 0;
		virtual RenderView* CreateRenderView(Camera* aCamera, int aResolutionX, int aResolutionY) = 0;
		virtual void RenderViews() = 0;

	};
}