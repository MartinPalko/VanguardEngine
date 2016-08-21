#pragma once
#include "Utility/MathFunctions.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	struct FOUNDATION_API Color
	{
	public:
		uint8 r;
		uint8 g;
		uint8 b;
		uint8 a;

		// Constructors

		Color(const uint8& aR, const uint8& aG, const uint8& aB, const uint8& aA)
		{
			r = aR;
			g = aG;
			b = aB;
			a = aA;
		}

		Color(const uint8& aR, const uint8& aG, const uint8& aB) : Color (aR, aG, aB, 255)
		{
		}

		Color(const Color& aOther)
		{
			r = aOther.r;
			g = aOther.g;
			b = aOther.b;
			a = aOther.a;
		}

		// Operators

		inline Color operator-() const { return Color(-r, -g, -b, -a); }

		inline bool operator==(const Color& aV) { return r == aV.r && g == aV.g && b == aV.b; }

		inline Color operator+(const Color& aV) const { return Color(r + aV.r, g + aV.g, b + aV.b, a + aV.a); }
		inline Color operator-(const Color& aV) const { return Color(r - aV.r, g - aV.g, b - aV.b, a - aV.a); }
		inline Color operator*(const Color& aV) const { return Color(r * aV.r, g * aV.g, b * aV.b, a * aV.a); }
		inline Color operator/(const Color& aV) const { return Color(r / aV.r, g / aV.g, b / aV.b, a / aV.a); }

		inline Color operator+(const uint8& aF) const { return Color(r + aF, g + aF, b + aF, a + aF); }
		inline Color operator-(const uint8& aF) const { return Color(r - aF, g - aF, b - aF, a - aF); }
		inline Color operator*(const uint8& aF) const { return Color(r * aF, g * aF, b * aF, a * aF); }
		inline Color operator/(const uint8& aF) const { return Color(r / aF, g / aF, b / aF, a / aF); }

		// Static Functions

		// Component-wise maximum
		inline static Color Max(const Color& aVector1, const Color& aVector2)
		{
			return Color(Math::Max(aVector1.r, aVector2.r), Math::Max(aVector1.g, aVector2.g), Math::Max(aVector1.b, aVector2.b), Math::Max(aVector1.a, aVector2.a));
		}

		// Component-wise minimum
		inline static Color Min(const Color& aVector1, const Color& aVector2)
		{
			return Color(Math::Min(aVector1.r, aVector2.r), Math::Min(aVector1.g, aVector2.g), Math::Min(aVector1.b, aVector2.b), Math::Min(aVector1.a, aVector2.a));
		}

		// Member Functions

		// The value of the largest component (r, g, b, or a)
		inline uint8 MaxValue() const { return Math::Max(Math::Max(Math::Max(r,g),b),a); }

		// The value of the smallest component (r, g, b, or a)
		inline uint8 MinValue() const { return Math::Min(Math::Min(Math::Min(r, g), b),a); }

	};
}
