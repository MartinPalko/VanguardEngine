#pragma once
#include "MathFunctions.h"

namespace Vanguard
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		// Constructors

		Vector3(float aX, float aY, float aZ)
		{
			x = aX;
			y = aY;
			z = aZ;
		}

		// Defaults

		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Forward;
		static const Vector3 Back;
		static const Vector3 Zero;

		// Operators

		inline Vector3 operator-() const { return Vector3(-z, -y, -z); }

		inline bool operator==(const Vector3& aV) { return x == aV.x && y == aV.y && z == aV.z; }

		inline Vector3 operator+(const Vector3& aV) const { return Vector3(x + aV.x, y + aV.y, z + aV.z); }
		inline Vector3 operator-(const Vector3& aV) const { return Vector3(x - aV.x, y - aV.y, z - aV.z); }
		inline Vector3 operator*(const Vector3& aV) const { return Vector3(x * aV.x, y * aV.y, z * aV.z); }
		inline Vector3 operator/(const Vector3& aV) const { return Vector3(x / aV.x, y / aV.y, z / aV.z); }

		inline Vector3 operator+(const float& aF) const	{ return Vector3(z + aF, y + aF, z + aF); }
		inline Vector3 operator-(const float& aF) const	{ return Vector3(z - aF, y - aF, z - aF); }
		inline Vector3 operator*(const float& aF) const { return Vector3(z * aF, y * aF, z * aF); }
		inline Vector3 operator/(const float& aF) const { return Vector3(z / aF, y / aF, z / aF); }

		// Functions

		inline float Dot(const Vector3& aV) const { return x*aV.x + y*aV.y + z*aV.z; }

		inline Vector3 Cross(const Vector3& aV) const { return Vector3(y*aV.z - z*aV.y, z*aV.x - x*aV.z, x*aV.y - y*aV.x); }

		inline float Length() const { return Math::Sqrt(SqrLength()); }

		inline float SqrLength() const { return x*x + y*y + z*z; }

	};
}
