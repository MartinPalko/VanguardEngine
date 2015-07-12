#pragma once
#include "Utility/MathFunctions.h"
#include "Vector2.h"

namespace Vanguard
{
	class Rect
	{
	public:
		Vector2 size;
		Vector2 center;
		// Constructors

		Rect(const float& aWidth, const float& aHeight, const float& aCenterX, const float& aCenterY)
			: size(aWidth, aHeight), center(aCenterX, aCenterY)
		{}

		Rect(const Vector2& aSize, const Vector2& aCenter)
			: size(aSize), center(aCenter)
		{}

		Rect(const Rect& aOther)
			: size(aOther.size), center(aOther.center)
		{}

		// Operators
		inline bool operator==(const Rect& aOther) { return size == aOther.size && center == aOther.center; }

		// Static Functions

		static inline Rect FromCorners(const Vector2& aCorner1, const Vector2& aCorner2)
		{
			Vector2 minCorner = Vector2::Min(aCorner1, aCorner2);
			Vector2 maxCorner = Vector2::Max(aCorner1, aCorner2);

			Vector2 size = Vector2(maxCorner.x - maxCorner.x, maxCorner.y - maxCorner.y);
			Vector2 center = Vector2(maxCorner.x - size.x / 2, maxCorner.y - size.y / 2);

			return Rect(size, center);
		}

		// Member Functions

		inline Vector2 Max(){ return center + size / 2; }
		inline float MaxX(){ return center.x + size.x / 2; }
		inline float MaxY(){ return center.y + size.y / 2; }

		inline Vector2 Min(){ return center - size / 2; }
		inline float MinX(){ return center.x - size.x / 2; }
		inline float MinY(){ return center.y - size.y / 2; }

		inline Vector2 TopLeft(){ return Vector2(MinX(), MaxY()); }
		inline Vector2 TopRight(){ return Vector2(MaxX(), MaxY()); }
		inline Vector2 BottomLeft(){ return Vector2(MinX(), MinY()); }
		inline Vector2 BottomRight(){ return Vector2(MaxX(), MinY()); }
	};
}
