#pragma once
#include "Core_Common.h"
#include "RenderView.h"
#include "Jobs/Job.h"
#include "Application/Application.h"


namespace Vanguard
{
	class IDebugCanvas
	{
	public:
		virtual ~IDebugCanvas() {}

		virtual void DrawPolygon(DynamicArray<Vector2> verticies, Color aColor, bool aSolid) = 0;
		virtual void DrawCircle(Vector3 aCenter, float radius, Color aColor, bool aSolid) = 0;
		virtual void DrawLine(Vector3 aStart, Vector2 aEnd, Color aColor) = 0;
		virtual void DrawPoint(Vector3 aLocation, float aSize, const Color aColor) = 0;
	};

	class IDebugDraw
	{
	public:
		virtual ~IDebugDraw() {}

		virtual void DebugDraw(IDebugCanvas* aCanvas) = 0;
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() {}

		virtual String RendererName() = 0;
		virtual RenderView* CreateRenderView(Camera* aCamera) { return CreateRenderView(aCamera, Application::GetWindowCreationParams()); };
		virtual RenderView* CreateRenderView(Camera* aCamera, WindowCreationParameters aWindowParameters) = 0;
		virtual void RegisterDebugDraw(IDebugDraw* aDebugDraw) = 0;
		virtual void UnregisterDebugDraw(IDebugDraw* aDebugDraw) = 0;
		virtual void StartRenderJob(Frame* aFrame) = 0;
		virtual void Present(Frame* aFrame) = 0;
	};
}