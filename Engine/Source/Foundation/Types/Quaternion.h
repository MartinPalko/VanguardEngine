#pragma once
#include "Utility/MathFunctions.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	class FOUNDATION_API Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;
		
		Quaternion(float aX, float aY, float aZ, float aW) 
			: x(aX), y(aY), z(aZ), w(aW) { }

		Quaternion() : Quaternion(0, 0, 0, 0) {}

		void Normalize();
		Quaternion GetNormalized() const;
		inline bool IsNormalized()
		{
			const float acceptableError = 0.001f;
			const float sum = x*x + y*y + z*z - 1;
			return (sum > -acceptableError && sum < acceptableError);
		}

	};
}