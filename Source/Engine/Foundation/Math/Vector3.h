#pragma once
#include "MathFunctions.h"
#include <math.h>

namespace Vanguard
{
	class Vector3
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

		// Functions

		inline float Dot(const Vector3& aV) const { return x*aV.x + y*aV.y + z*aV.z; }

		inline Vector3 Cross(const Vector3& aV) const { return Vector3(y*aV.z - z*aV.y, z*aV.x - x*aV.z, x*aV.y - y*aV.x); }

		inline float Length() const { return Math::Sqrt(SqrLength()); }

		// The squared length of the vector. Much faster than finding the length. If just seeing if one vector is longer than another, comparing the squared lengths is much more efficient.
		float SqrLength() const { return x*x + y*y + z*z; }

		inline Vector3 Normalize() const { return *this / Math::Sqrt(x*x + y*y + z*z); }
	};
}
