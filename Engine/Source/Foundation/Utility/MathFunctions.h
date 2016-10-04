#pragma once
#include "Platforms/Platform.h"
#include "Foundation_Common.h"
#include <math.h>

namespace Vanguard
{
	namespace Math
	{
		static const float Pi = 3.14159265359f;

		inline float FSqrt(float aFloat) { return ::sqrtf(aFloat); }

		inline float FPow(float aBase, float aExponent) { return ::powf(aBase, aExponent); }
		inline int32 IPow(int32 aBase, int32 aExponent)
		{
            if (aExponent < 0) return 0;

            int returnValue = 1;
            while (aExponent > 0 )
            {
                aExponent--;
                returnValue *= aBase;
            }
            return returnValue;
		}

		template<class T> T Squared(T aBase){ return aBase * aBase; }
		template<class T> T Cubed(T aBase) { return aBase * aBase * aBase; }

		inline float FSin(float aFloat) { return ::sinf(aFloat); }
		inline float FCos(float aFloat) { return ::cosf(aFloat); }
		inline float FTan(float aFloat) { return ::tanf(aFloat); }

		inline float FAbs(float aFloat) { return ::fabsf(aFloat); }
		inline int32 IAbs(int32 aInt32) { return ::abs(aInt32); }

		inline float FSign(float aFloat) { return aFloat > 0 ? 1.0f : -1.0f; }
		inline int32 ISign(int32 aFloat) { return aFloat > 0 ? 1 : -1; }

		template<class T> T Max(const T& a, const T& b) { return (a < b) ? b : a; }
		template<class T> T Min(const T& a, const T& b) { return (a < b) ? a : b; }
		template<class T> T Clamp(const T& a, const T& lower, const T& upper) { return Min(Max(a, lower), upper); }

		template<class T> bool AboutEqual(const T& a, const T& b, const T& tolerance)
		{
			const T diff = a - b;
			return diff >= -tolerance && diff <= tolerance;
		}

		// Generates a random float between 0 and 1
		FOUNDATION_API float FRandom();
		FOUNDATION_API int32 IRandom();
		FOUNDATION_API void SeedRandom(int aSeed);


		inline float FRandomRange(const float& aMin, const float& aMax)
		{
			return (FRandom() * (aMax - aMin)) + aMin;
		}

		inline int32 IRandomRange(const int& aMin, const int& aMax)
		{
			return (IRandom() % (aMin - aMax)) + aMin;
		}

	}
}
