#pragma once
#include <math.h>
#include "Utility/MathFunctions.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	struct FOUNDATION_API Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		// Constructors

		Vector4(const float& aX, const float& aY, const float& aZ, const float& aW)
		{
			x = aX;
			y = aY;
			z = aZ;
			w = aW;
		}

		Vector4(const Vector4& aOther)
		{
			x = aOther.x;
			y = aOther.y;
			z = aOther.z;
			w = aOther.w;
		}

		// Operators

		inline Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }

		inline bool operator==(const Vector4& aV) { return x == aV.x && y == aV.y && z == aV.z; }

		inline Vector4 operator+(const Vector4& aV) const { return Vector4(x + aV.x, y + aV.y, z + aV.z, w + aV.w); }
		inline Vector4 operator-(const Vector4& aV) const { return Vector4(x - aV.x, y - aV.y, z - aV.z, w - aV.w); }
		inline Vector4 operator*(const Vector4& aV) const { return Vector4(x * aV.x, y * aV.y, z * aV.z, w * aV.w); }
		inline Vector4 operator/(const Vector4& aV) const { return Vector4(x / aV.x, y / aV.y, z / aV.z, w / aV.w); }

		inline Vector4 operator+(const float& aF) const	{ return Vector4(x + aF, y + aF, z + aF, w + aF); }
		inline Vector4 operator-(const float& aF) const	{ return Vector4(x - aF, y - aF, z - aF, w - aF); }
		inline Vector4 operator*(const float& aF) const { return Vector4(x * aF, y * aF, z * aF, w * aF); }
		inline Vector4 operator/(const float& aF) const { return Vector4(x / aF, y / aF, z / aF, w / aF); }

		// Static Functions

		// Component-wise maximum
		inline static Vector4 Max(const Vector4& aVector1, const Vector4& aVector2)
		{
			return Vector4(Math::Max(aVector1.x, aVector2.x), Math::Max(aVector1.y, aVector2.y), Math::Max(aVector1.z, aVector2.z), Math::Max(aVector1.w, aVector2.w));
		}

		// Component-wise minimum
		inline static Vector4 Min(const Vector4& aVector1, const Vector4& aVector2)
		{
			return Vector4(Math::Min(aVector1.x, aVector2.x), Math::Min(aVector1.y, aVector2.y), Math::Min(aVector1.z, aVector2.z), Math::Min(aVector1.w, aVector2.w));
		}

		// Member Functions

		// Find dot product
		inline float Dot(const Vector4& aV) const { return x*aV.x + y*aV.y + z*aV.z + w*aV.w; }

		// Find the length of the vector
		inline float Length() const { return Math::FSqrt(SqrLength()); }

		// The squared length of the vector. Much faster than finding the length. If just seeing if one vector is longer than another, comparing the squared lengths is much more efficient.
		float SqrLength() const { return x*x + y*y + z*z + w*w; }

		// Normalize the vector (becomes length of 1)
		inline Vector4 Normalize() const { return *this / Math::FSqrt(x*x + y*y + z*z + w*w); }

		// Component-wise absolute
		inline Vector4 Abs() const { return Vector4(Math::FAbs(x), Math::FAbs(y), Math::FAbs(z), Math::FAbs(w)); }

		// The value of the largest component (x, y, z, or w)
		inline float MaxValue() const { return Math::Max(Math::Max(Math::Max(x,y),z),w); }

		// The value of the smallest component (x, y, z, or w)
		inline float MinValue() const { return Math::Min(Math::Min(Math::Min(x, y), z),w); }

	};
}
