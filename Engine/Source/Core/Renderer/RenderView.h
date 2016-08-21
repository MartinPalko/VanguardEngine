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
		int resolutionX;
		int resolutionY;

		RenderView(WindowHandle aWindowHandle, Camera* aViewCamera)
		{
			windowHandle = aWindowHandle;
			viewCamera = aViewCamera;
		}
		virtual ~RenderView() {}

		virtual WindowHandle GetWindowHandle() { return windowHandle; }
	};
}