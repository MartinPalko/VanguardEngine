#pragma once
#include "MathFunctions.h"

namespace Vanguard
{
	class Vector2
	{
	public:
		float x;
		float y;

		// Constructors

		Vector2(const float& aX, const float& aY)
		{
			x = aX;
			y = aY;
		}

		Vector2(const Vector2& aOther)
		{
			x = aOther.x;
			y = aOther.y;
		}

		// Defaults

		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Right;
		static const Vector2 Left;
		static const Vector2 Zero;
		static const Vector2 One;

		// Operators

		inline Vector2 operator-() const { return Vector2(-x, -y); }

		inline bool operator==(const Vector2& aV) { return x == aV.x && y == aV.y; }

		inline Vector2 operator+(const Vector2& aV) const { return Vector2(x + aV.x, y + aV.y); }
		inline Vector2 operator-(const Vector2& aV) const { return Vector2(x - aV.x, y - aV.y); }
		inline Vector2 operator*(const Vector2& aV) const { return Vector2(x * aV.x, y * aV.y); }
		inline Vector2 operator/(const Vector2& aV) const { return Vector2(x / aV.x, y / aV.y); }

		inline Vector2 operator+(const float& aF) const	{ return Vector2(x + aF, y + aF); }
		inline Vector2 operator-(const float& aF) const	{ return Vector2(x - aF, y - aF); }
		inline Vector2 operator*(const float& aF) const { return Vector2(x * aF, y * aF); }
		inline Vector2 operator/(const float& aF) const { return Vector2(x / aF, y / aF); }

		// Functions

		inline float Dot(const Vector2& aV) const { return x*aV.x + y*aV.y; }

		inline float Length() const { return Math::Sqrt(SqrLength()); }

		// The squared length of the vector. Much faster than finding the length. If just seeing if one vector is longer than another, comparing the squared lengths is much more efficient.
		inline float SqrLength() const { return x*x + y*y; }

		inline Vector2 Normalize() const { return *this / Length(); }
	};
}
