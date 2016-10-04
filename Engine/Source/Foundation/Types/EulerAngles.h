#pragma once
#include "Utility/MathFunctions.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	struct FOUNDATION_API EulerAngles
	{
	public:
		float x;
		float y;
		float z;

		// Constructors

		EulerAngles(const float& aX, const float& aY, const float& aZ)
		{
			x = aX;
			y = aY;
			z = aZ;
		}

		EulerAngles(const EulerAngles& aOther)
		{
			x = aOther.x;
			y = aOther.y;
			z = aOther.z;
		}

		// Operators

		inline EulerAngles operator-() const { return EulerAngles(-x, -y, -z); }

		inline bool operator==(const EulerAngles& aV) { return x == aV.x && y == aV.y && z == aV.z; }
		inline bool operator!=(const EulerAngles& aV) { return x != aV.x || y != aV.y || z != aV.z; }

		inline EulerAngles operator+(const EulerAngles& aV) const { return EulerAngles(x + aV.x, y + aV.y, z + aV.z); }
		inline EulerAngles operator-(const EulerAngles& aV) const { return EulerAngles(x - aV.x, y - aV.y, z - aV.z); }
		inline EulerAngles operator*(const EulerAngles& aV) const { return EulerAngles(x * aV.x, y * aV.y, z * aV.z); }
		inline EulerAngles operator/(const EulerAngles& aV) const { return EulerAngles(x / aV.x, y / aV.y, z / aV.z); }

		inline void operator+=(const EulerAngles& aV) { x += aV.x; y += aV.y; z += aV.z;}
		inline void operator-=(const EulerAngles& aV) { x -= aV.x; y -= aV.y; z -= aV.z;}
		inline void operator*=(const EulerAngles& aV) { x *= aV.x; y *= aV.y; z *= aV.z;}
		inline void operator/=(const EulerAngles& aV) { x /= aV.x; y /= aV.y; z /= aV.z;}


		static float NormalizeAxis(float aAngle);

		// Normalize the Euler angles (unwinds)
		EulerAngles Normalize() const;

	};
}
