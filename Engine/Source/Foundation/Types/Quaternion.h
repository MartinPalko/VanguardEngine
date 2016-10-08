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

		bool operator==(const Quaternion& aOther);
		bool operator!=(const Quaternion& aOther);

		void Normalize();
		Quaternion GetNormalized() const;
		bool IsNormalized();

		static Quaternion FromEuler(const EulerAngles& aEuler);
		static Quaternion FromEuler(float aX, float aY, float aZ) { return FromEuler(EulerAngles(aX, aY, aZ)); }
		EulerAngles ToEuler();
	};
}