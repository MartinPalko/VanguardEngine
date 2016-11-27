#include "Box2D/Box2D.h"

#include "RectangleCollider.h"

namespace Vanguard
{
	TYPE_DEFINITION(RectangleCollider, Collider2D);

	RectangleCollider::RectangleCollider() : Collider2D()
		, shapeValid(false)
		, dimensions(1, 1)
	{
		box2DShape = new b2PolygonShape();
	}

	RectangleCollider::~RectangleCollider()
	{
		delete box2DShape;
	}

	void RectangleCollider::SetDimensions(Vector2 aDimensions)
	{
		// TODO: Invalidate box2d shape
		dimensions = aDimensions;
		if (GetEntity()->GetType()->IsA(Type::GetType<Actor>()))
			static_cast<Actor*>(GetEntity())->UpdateBounds();
	}

	b2Shape* RectangleCollider::GetBox2DShape()
	{
		if (!shapeValid)
		{
			box2DShape->SetAsBox(dimensions.x / 2, dimensions.y / 2);
		}

		return box2DShape;
	}
}