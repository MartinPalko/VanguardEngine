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

		inline float Pow(float aBase, float aExponent) { return ::pow(aBase, aExponent); }
		inline float Pow(float aBase, int32 aExponent) { return ::pow(aBase, aExponent); }
		inline float Pow(int32 aBase, int32 aExponent) { return ::pow(aBase, aExponent); }

		inline float Sin(float aFloat) { return ::sinf(aFloat); }
		inline float Cos(float aFloat) { return ::cosf(aFloat); }
		inline float Tan(float aFloat) { return ::tanf(aFloat); }

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