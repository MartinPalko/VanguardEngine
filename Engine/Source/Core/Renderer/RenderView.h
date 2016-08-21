#pragma once
#include "Foundation.h"

namespace Vanguard
{
	class Camera;

	class RenderView
	{
	private:
		WindowHandle windowHandle;

	public:
		Camera* viewCamera;

		RenderView(WindowHandle aWindowHandle, Camera* aViewCamera)
		{
			windowHandle = aWindowHandle;
			viewCamera = aViewCamera;
		}
		virtual ~RenderView() {}

		WindowHandle GetWindowHandle() { return windowHandle; }
	};
}