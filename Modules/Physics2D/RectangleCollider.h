#pragma once
#include "Collider2D.h"

class b2PolygonShape;

namespace Vanguard
{
	class PHYSICS2D_API RectangleCollider : public Collider2D
	{
		TYPE_DECLARATION(RectangleCollider, Collider2D)

	private:
		b2PolygonShape* box2DShape;
		bool shapeValid;

		Vector2 dimensions;

	public:
		RectangleCollider();
		virtual ~RectangleCollider();

		inline Vector2 GetDimensions() const { return dimensions; }
		void SetDimensions(Vector2 aDimensions);

	protected:
		// Implement Collider2D
		virtual b2Shape* GetBox2DShape() override;

		// Override Component
		virtual Box GetBounds() override { return Box(Vector3(dimensions.x, dimensions.y, 0)); }
	};
}