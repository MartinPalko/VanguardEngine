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

		template<class T> T Squared(T aBase){ return aBase * aBase; }
		template<class T> T Cubed(T aBase) { return aBase * aBase * aBase; }

		inline float FSin(float aFloat) { return ::sinf(aFloat); }
		inline float FCos(float aFloat) { return ::cosf(aFloat); }
		inline float FTan(float aFloat) { return ::tanf(aFloat); }

		inline float FAbs(float aFloat) { return ::fabsf(aFloat); }
		inline int32 IAbs(int32 aInt32) { return ::abs(aInt32); }


		template<class T> const T& Max(const T& a, const T& b) { return (a < b) ? b : a; }
		template<class T> const T& Min(const T& a, const T& b) { return (a < b) ? a : b; }


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
