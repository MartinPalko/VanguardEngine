#pragma once
#include "MathFunctions.h"
#include <math.h>

namespace Vanguard
{
	struct Vector3
	{
	public:
		float x;
		float y;
		float z;

		// Constructors

		Vector3(const float& aX, const float& aY, const float& aZ)
		{
			x = aX;
			y = aY;
			z = aZ;
		}

		Vector3(const Vector3& aOther)
		{
			x = aOther.x;
			y = aOther.y;
			z = aOther.z;
		}

		// Defaults

		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Forward;
		static const Vector3 Back;
		static const Vector3 Zero;
		static const Vector3 One;

		// Operators

		inline Vector3 operator-() const { return Vector3(-x, -y, -z); }

		inline bool operator==(const Vector3& aV) { return x == aV.x && y == aV.y && z == aV.z; }

		inline Vector3 operator+(const Vector3& aV) const { return Vector3(x + aV.x, y + aV.y, z + aV.z); }
		inline Vector3 operator-(const Vector3& aV) const { return Vector3(x - aV.x, y - aV.y, z - aV.z); }
		inline Vector3 operator*(const Vector3& aV) const { return Vector3(x * aV.x, y * aV.y, z * aV.z); }
		inline Vector3 operator/(const Vector3& aV) const { return Vector3(x / aV.x, y / aV.y, z / aV.z); }

		inline Vector3 operator+(const float& aF) const	{ return Vector3(x + aF, y + aF, z + aF); }
		inline Vector3 operator-(const float& aF) const	{ return Vector3(x - aF, y - aF, z - aF); }
		inline Vector3 operator*(const float& aF) const { return Vector3(x * aF, y * aF, z * aF); }
		inline Vector3 operator/(const float& aF) const { return Vector3(x / aF, y / aF, z / aF); }

		// Static Functions

		// Component-wise maximum
		inline static Vector3 Max(const Vector3& aVector1, const Vector3& aVector2)
		{
			return Vector3(Math::FMax(aVector1.x, aVector2.x), Math::FMax(aVector1.y, aVector2.y), Math::FMax(aVector1.z, aVector2.z));
		}

		// Component-wise minimum
		inline static Vector3 Min(const Vector3& aVector1, const Vector3& aVector2)
		{
			return Vector3(Math::FMin(aVector1.x, aVector2.x), Math::FMin(aVector1.y, aVector2.y), Math::FMin(aVector1.z, aVector2.z));
		}

		// Member Functions

		// Find dot product
		inline float Dot(const Vector3& aV) const { return x*aV.x + y*aV.y + z*aV.z; }

		// Find cross product
		inline Vector3 Cross(const Vector3& aV) const { return Vector3(y*aV.z - z*aV.y, z*aV.x - x*aV.z, x*aV.y - y*aV.x); }

		// Find the length of the vector
		inline float Length() const { return Math::FSqrt(SqrLength()); }

		// The squared length of the vector. Much faster than finding the length. If just seeing if one vector is longer than another, comparing the squared lengths is much more efficient.
		float SqrLength() const { return x*x + y*y + z*z; }

		// Normalize the vector (becomes length of 1)
		inline Vector3 Normalize() const { return *this / Math::FSqrt(x*x + y*y + z*z); }

		// Component-wise absolute
		inline Vector3 Abs() const { return Vector3(Math::FAbs(x), Math::FAbs(y), Math::FAbs(z)); }

		// The value of the largest component (x, y, or z)
		inline float MaxValue() const { return Math::FMax(Math::FMax(x,y),z); }

		// The value of the smallest component (x, y, or z)
		inline float MinValue() const { return Math::FMin(Math::FMin(x, y), z); }

	};
}
