#pragma once

namespace Vanguard
{
	class Camera;

	class RenderView
	{
	public:
		Camera* viewCamera;
		int resolutionX;
		int resolutionY;

		RenderView(Camera* aViewCamera, int aResolutionX, int aResolutionY)
		{
			viewCamera = aViewCamera;
			resolutionX = aResolutionX;
			resolutionY = aResolutionY;
		}
	};
}