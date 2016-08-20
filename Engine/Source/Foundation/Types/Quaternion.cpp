#include "Quaternion.h"

namespace Vanguard
{
	void Quaternion::Normalize()
	{
		const float n = Math::FSqrt(x * x + y * y + z * z + w * w);
		x /= n;
		y /= n;
		z /= n;
		w /= n;
	}

	Quaternion Quaternion::GetNormalized() const
	{
		Quaternion newQuat(*this);
		newQuat.Normalize();
		return newQuat;
	}
}