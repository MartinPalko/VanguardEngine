#include "B2DrawImplementation.h"

namespace
{
	static Vanguard::Color ToVanguardColor(const b2Color& c)
	{
		return Color(c.r * 255.0f, c.g * 255.0f, c.b * 255.0f, c.a * 255.0f);
	}
}

namespace Vanguard
{
	B2DrawImplementation::B2DrawImplementation(IDebugCanvas* aCanvas)
		: canvas(aCanvas)
	{
	}

	B2DrawImplementation::~B2DrawImplementation()
	{
	}

	void B2DrawImplementation::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DynamicArray<Vector2> vertexArray(vertexCount);

		for (int32 i = 0; i < vertexCount; i++)
		{
			vertexArray.PushBack(Vector2(vertices[i].x, vertices[i].y));
		}

		canvas->DrawPolygon(vertexArray, ToVanguardColor(color), false);
	}

	void B2DrawImplementation::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DynamicArray<Vector2> vertexArray(vertexCount);

		for (int32 i = 0; i < vertexCount; i++)
		{
			vertexArray.PushBack(Vector2(vertices[i].x, vertices[i].y));
		}

		canvas->DrawPolygon(vertexArray,ToVanguardColor(color), true);
	}

	void B2DrawImplementation::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		canvas->DrawCircle(Vector2(center.x, center.y), radius, ToVanguardColor(color), false);
	}

	void B2DrawImplementation::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		canvas->DrawCircle(Vector2(center.x, center.y), radius, ToVanguardColor(color), true);
	}

	void B2DrawImplementation::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		canvas->DrawLine(Vector2(p1.x, p1.y), Vector2(p1.x, p1.y), ToVanguardColor(color));
	}

	void B2DrawImplementation::DrawTransform(const b2Transform& xf)
	{
		// TODO
	}

	void B2DrawImplementation::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
		canvas->DrawPoint(Vector2(p.x, p.y), size, ToVanguardColor(color));
	}
}