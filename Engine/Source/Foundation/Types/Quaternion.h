#pragma once
#include "Utility/MathFunctions.h"
#include "Foundation_Common.h"
#include "EulerAngles.h"

namespace Vanguard
{
	struct FOUNDATION_API Quaternion
	{
		float x;
		float y;
		float z;
		float w;
		
		Quaternion(float aX, float aY, float aZ, float aW) 
			: x(aX), y(aY), z(aZ), w(aW) { }

		Quaternion() : Quaternion(0, 0, 0, 0) {}

		void Normalize();
		Quaternion GetNormalized() const;
		bool IsNormalized();

		static Quaternion FromEuler(const EulerAngles& aEuler);
		EulerAngles ToEuler();
	};
}