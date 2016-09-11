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
			: windowHandle(nullptr)
			, viewCamera(aViewCamera)
			, clearColor(0, 0, 0)			
		{
		}

		virtual ~RenderView() {}

		WindowHandle GetWindowHandle() { return windowHandle; }
	};
}