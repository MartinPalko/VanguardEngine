#pragma once
#include "Foundation.h"
#include "Application/Application.h"

namespace Vanguard
{
	class Camera;

	class RenderView
	{
	public:
		Camera* viewCamera;
		Color clearColor;

		RenderView(Camera* aViewCamera)
			: viewCamera(aViewCamera)
			, clearColor(0, 0, 0)			
		{
		}

		virtual ~RenderView() {}

		virtual NativeWindow GetWindowHandle() = 0;
	};
}