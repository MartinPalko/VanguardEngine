#pragma once
#include "Foundation.h"
#include "Application/Application.h"

namespace Vanguard
{
	class Camera;

	class RenderView
	{
	private:
		WindowHandle windowHandle;

	public:
		Camera* viewCamera;
		Color clearColor;

		RenderView(Camera* aViewCamera)
			: clearColor(0, 0, 0)
		{
			viewCamera = aViewCamera;
		}
		virtual ~RenderView() {}

		WindowHandle GetWindowHandle() { return windowHandle; }
	};
}