#pragma once
#include "Types/IntegerDefs.h"
#include <math.h>
#include <random>

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

		inline float FSquared(float aBase){ return aBase * aBase; }
		inline int32 ISquared(int32 aBase){ return aBase * aBase; }

		inline float FCubed(float aBase){ return aBase * aBase * aBase; }
		inline int32 ICubed(int32 aBase){ return aBase * aBase * aBase; }

		inline float FSin(float aFloat) { return ::sinf(aFloat); }
		inline float FCos(float aFloat) { return ::cosf(aFloat); }
		inline float FTan(float aFloat) { return ::tanf(aFloat); }

		inline float FAbs(float aFloat) { return ::fabsf(aFloat); }
		inline int32 IAbs(int32 aInt32) { return ::abs(aInt32); }

		inline float FMax(float aFloat1, float aFloat2) { return aFloat1 >= aFloat2 ? aFloat1 : aFloat2; }
		inline int32 IMax(int32 aInt1, int32 aInt2) { return aInt1 >= aInt2 ? aInt1 : aInt2; }

		inline float FMin(float aFloat1, float aFloat2) { return aFloat1 <= aFloat2 ? aFloat1 : aFloat2; }
		inline int32 IMin(int32 aInt1, int32 aInt2) { return aInt1 <= aInt2 ? aInt1 : aInt2; }


		// Generates a random float between 0 and 1
		inline float FRandom() { return (float)rand() / (float)RAND_MAX; }

		inline int32 IRandom() { return rand(); }

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
