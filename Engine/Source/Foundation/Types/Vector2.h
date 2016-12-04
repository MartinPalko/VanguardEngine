#pragma once
#include "Utility/MathFunctions.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	struct Vector3;

	struct FOUNDATION_API Vector2
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

		inline bool operator==(const Vector2& aV) const { return x == aV.x && y == aV.y; }
		inline bool operator!=(const Vector2& aV) const { return x != aV.x || y != aV.y; }

		inline Vector2 operator+(const Vector2& aV) const { return Vector2(x + aV.x, y + aV.y); }
		inline Vector2 operator-(const Vector2& aV) const { return Vector2(x - aV.x, y - aV.y); }
		inline Vector2 operator*(const Vector2& aV) const { return Vector2(x * aV.x, y * aV.y); }
		inline Vector2 operator/(const Vector2& aV) const { return Vector2(x / aV.x, y / aV.y); }

		inline void operator+=(const Vector2& aV) { x += aV.x; y += aV.y; }
		inline void operator-=(const Vector2& aV) { x -= aV.x; y -= aV.y; }
		inline void operator*=(const Vector2& aV) { x *= aV.x; y *= aV.y; }
		inline void operator/=(const Vector2& aV) { x /= aV.x; y /= aV.y; }

		inline Vector2 operator+(const float& aF) const { return Vector2(x + aF, y + aF); }
		inline Vector2 operator-(const float& aF) const { return Vector2(x - aF, y - aF); }
		inline Vector2 operator*(const float& aF) const { return Vector2(x * aF, y * aF); }
		inline Vector2 operator/(const float& aF) const { return Vector2(x / aF, y / aF); }

		inline void operator+=(const float& aF) { x += aF; y += aF; }
		inline void operator-=(const float& aF) { x -= aF; y -= aF; }
		inline void operator*=(const float& aF) { x *= aF; y *= aF; }
		inline void operator/=(const float& aF) { x /= aF; y /= aF; }

		operator Vector3() const;

		// Static Functions

		// Component-wise maximum
		inline static Vector2 Max(const Vector2& aVector1, const Vector2& aVector2)
		{
			return Vector2(Math::Max(aVector1.x, aVector2.x), Math::Max(aVector1.y, aVector2.y));
		}

		// Component-wise minimum
		inline static Vector2 Min(const Vector2& aVector1, const Vector2& aVector2)
		{
			return Vector2(Math::Min(aVector1.x, aVector2.x), Math::Min(aVector1.y, aVector2.y));
		}

		// Member Functions

		// Find dot product
		inline float Dot(const Vector2& aV) const { return x*aV.x + y*aV.y; }

		// Find the length of the vector
		inline float Length() const { return Math::FSqrt(SqrLength()); }

		// The squared length of the vector. Much faster than finding the length. If just seeing if one vector is longer than another, comparing the squared lengths is much more efficient.
		float SqrLength() const { return x*x + y*y; }

		// Normalize the vector (becomes length of 1)
		inline Vector2 Normalize() const { return *this / Math::FSqrt(x*x + y*y); }

		// Component-wise absolute
		inline Vector2 Abs() const { return Vector2(Math::FAbs(x), Math::FAbs(y)); }

		// The value of the largest component (x, or y)
		inline float MaxValue() const { return Math::Max(x, y); }

		// The value of the smallest component (x, or y)
		inline float MinValue() const { return Math::Min(x, y); }
	};
}
