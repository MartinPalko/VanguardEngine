#pragma once
#include "IntegerDefs.h"
#include <math.h>
#include <random>

namespace Vanguard
{
	namespace Math
	{
		static const float Pi = 3.14159265359f;

		inline float Sqrt(float aFloat) { return ::sqrtf(aFloat); }

		inline float FloatPow(float aBase, float aExponent) { return ::powf(aBase, aExponent); }
		inline int32 IntPow(int32 aBase, int32 aExponent)
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

		inline float Squared(float aBase){ return aBase * aBase; }
		inline int32 Squared(int32 aBase){ return aBase * aBase; }

		inline float Cubed(float aBase){ return aBase * aBase * aBase; }
		inline int32 Cubed(int32 aBase){ return aBase * aBase * aBase; }

		inline float Sin(float aFloat) { return ::sinf(aFloat); }
		inline float Cos(float aFloat) { return ::cosf(aFloat); }
		inline float Tan(float aFloat) { return ::tanf(aFloat); }

		inline float Abs(float aFloat) { return ::abs(aFloat); }
		inline int32 Abs(int32 aInt32) { return ::abs(aInt32); }

		inline float Max(float aFloat1, float aFloat2) { return aFloat1 >= aFloat2 ? aFloat1 : aFloat2; }
		inline int32 Max(int32 aInt1, int32 aInt2) { return aInt1 >= aInt2 ? aInt1 : aInt2; }

		inline float Min(float aFloat1, float aFloat2) { return aFloat1 <= aFloat2 ? aFloat1 : aFloat2; }
		inline int32 Min(int32 aInt1, int32 aInt2) { return aInt1 <= aInt2 ? aInt1 : aInt2; }


		// Generates a random float between 0 and 1
		inline float FRandom() { return (float)rand() / (float)RAND_MAX; }
		inline int32 Random() { return rand(); }

		inline float FRandomRange(const float& aMin, const float& aMax)
		{
			return (FRandom() * (aMax - aMin)) + aMin;
		}

		inline int32 RandomRange(const int& aMin, const int& aMax)
		{
			return (Random() % (aMin - aMax)) + aMin;
		}

	}
}
