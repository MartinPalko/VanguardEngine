#include "EulerAngles.h"

namespace Vanguard
{
	float EulerAngles::NormalizeAxis(float aAngle)
	{
		const float twoPi = Math::Pi * 2.0f;
		aAngle = fmod(aAngle, twoPi);

		if (aAngle < 0.0f)
			aAngle += twoPi;

		if (aAngle > Math::Pi)
			aAngle -= twoPi;

		return aAngle;
	}

	EulerAngles EulerAngles::Normalize() const
	{
		return EulerAngles(NormalizeAxis(x), NormalizeAxis(y), NormalizeAxis(z));
	}
}