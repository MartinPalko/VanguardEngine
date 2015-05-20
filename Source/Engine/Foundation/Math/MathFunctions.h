#pragma once
#include <math.h>

namespace Vanguard
{
	namespace Math
	{
		inline float Sqrt(float aFloat) { return ::sqrtf(aFloat); }

		inline float Sin(float aFloat) { return ::sinf(aFloat); }
		inline float Cos(float aFloat) { return ::cosf(aFloat); }
		inline float Tan(float aFloat) { return ::tanf(aFloat); }

	}
}