#include "DebugCanvas2D.h"
#include "Renderer2D.h"

#include "VanguardSDL.h"

#define SPLIT_COLOR_RGBA(VanguardColor) VanguardColor.r, VanguardColor.g, VanguardColor.b, VanguardColor.a
#define SPLIT_COLOR_RGB(VanguardColor) VanguardColor.r, VanguardColor.g, VanguardColor.b

namespace Vanguard
{
	DebugCanvas2D::DebugCanvas2D(JobRenderView* aRenderView) : renderView(aRenderView)
	{
		SDL_GetWindowSize(renderView->window, &screenX, &screenY);		
	}

	Vector2 DebugCanvas2D::ToCameraSpace(Vector3 aWorldSpace)
	{
		// TODO: Make some reusable functionality in the Renderer to do this.

		const float aspectRatio = (float)screenX / (float)screenY;
		Vector2 screenSize(screenX, screenY);
		Vector2 aspectAdjustment = aspectRatio > 1 ? Vector2(1 / aspectRatio, 1) : Vector2(1, aspectRatio);

		Vector3 cameraSpace = renderView->worldToCamera.TransformPoint(aWorldSpace);

		// Projection
		Vector3 clipSpace = renderView->projectionMatrix.TransformPoint(cameraSpace);
		Vector3 scaleReference = renderView->projectionMatrix.TransformPoint(cameraSpace + Vector3(0,1,0));

		// Correct for aspect ratio
		clipSpace *= Vector3(aspectAdjustment.x, aspectAdjustment.y, 1);

		// NDC space
		Vector3 ndcSpace = (clipSpace + 1.0f) / 2.0f;
		ndcSpace.y = 1 - ndcSpace.y;

		// Screenspace
		Vector2 screenspace = Vector2(screenX * ndcSpace.x, screenY * ndcSpace.y);

		return screenspace;
	}

	void DebugCanvas2D::DrawPolygon(DynamicArray<Vector2> aVerticies, Color aColor, bool aSolid)
	{
		SDL_SetRenderDrawColor(renderView->renderer, SPLIT_COLOR_RGBA(aColor));

		const size_t vertCount = aVerticies.Count();
		SDL_Point* csPoints = new SDL_Point[vertCount + 1];
		for (size_t i = 0; i < vertCount + 1; i++)
		{
			Vector2 csPoint = ToCameraSpace(aVerticies[i % vertCount]);
			csPoints[i] = SDL_Point{(int)csPoint.x, (int)csPoint.y};
		}
		SDL_RenderDrawLines(renderView->renderer, csPoints, vertCount + 1);
		
		delete[] csPoints;
	}

	void DebugCanvas2D::DrawCircle(Vector3 aCenter, float aRadius, Color aColor, bool aSolid)
	{
		const size_t segments = 32;
		const float angle = (2.0f * Math::Pi) / (float)segments;
		DynamicArray<Vector2> verticies(segments);

		for (size_t i = 0; i < segments; i++)
		{ 
			verticies.PushBack(Vector2(Math::FCos(angle * i) * aRadius, Math::FSin(angle * i) * aRadius) + aCenter);
		}

		DrawPolygon(verticies, aColor, aSolid);
	}

	void DebugCanvas2D::DrawLine(Vector3 aStart, Vector2 aEnd, Color aColor)
	{
		SDL_SetRenderDrawColor(renderView->renderer, SPLIT_COLOR_RGBA(aColor));

		Vector2 csStart = ToCameraSpace(aStart);
		Vector2 csEnd = ToCameraSpace(aEnd);
		
		SDL_RenderDrawLine(renderView->renderer, csStart.x, csStart.y, csEnd.x, csEnd.y);
	}

	void DebugCanvas2D::DrawPoint(Vector3 aLocation, float aSize, const Color aColor)
	{
		DrawCircle(aLocation, aSize / 2.0f, aColor, true);
		DrawLine(aLocation + Vector2(0, aSize), aLocation - Vector2(0, aSize), aColor);
		DrawLine(aLocation + Vector2(aSize, 0), aLocation - Vector2(aSize, 0), aColor);
	}
}