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

		RenderView(WindowHandle aWindowHandle, Camera* aViewCamera)
			: clearColor(0, 0, 0)
		{
			windowHandle = aWindowHandle;
			viewCamera = aViewCamera;
		}
		virtual ~RenderView() {}

		WindowHandle GetWindowHandle() { return windowHandle; }
	};
}