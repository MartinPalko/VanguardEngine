#pragma once
#include "Renderer2D_Common.h"

namespace Vanguard
{
	struct JobRenderView;

	class DebugCanvas2D : public IDebugCanvas
	{
		JobRenderView* renderView;
		int screenX;
		int screenY;

	public:
		DebugCanvas2D(JobRenderView* aRenderView);

		Vector2 ToCameraSpace(Vector3 aWorldSpace);

		virtual void DrawPolygon(DynamicArray<Vector2> aVerticies, Color aColor, bool aSolid) override;
		virtual void DrawCircle(Vector3 aCenter, float aRadius, Color aColor, bool aSolid) override;
		virtual void DrawLine(Vector3 aStart, Vector2 aEnd, Color aColor) override;
		virtual void DrawPoint(Vector3 aLocation, float aSize, const Color aColor) override;
	};
}