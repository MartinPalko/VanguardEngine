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

	bool Quaternion::IsNormalized()
	{
		const float acceptableError = 0.001f;
		const float sum = x*x + y*y + z*z - 1;
		return (sum > -acceptableError && sum < acceptableError);
	}

	Quaternion Quaternion::FromEuler(const EulerAngles& aEuler)
	{
		const float halfY = aEuler.y * 0.5f;
		const float halfZ = aEuler.z * 0.5f;
		const float halfX = aEuler.x * 0.5f;

		const float cosPitch = Math::FCos(halfY);
		const float cosYaw = Math::FCos(halfZ);
		const float cosRoll = Math::FCos(halfX);
		const float sinPitch = Math::FSin(halfY);
		const float sinYaw = Math::FSin(halfZ);
		const float sinRoll = Math::FSin(halfX);

		return Quaternion(
			cosRoll * sinPitch * sinYaw - sinRoll * cosPitch * cosYaw,
			-cosRoll * sinPitch * cosYaw - sinRoll * cosPitch * sinYaw,
			cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
			cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw
		);
	}

	EulerAngles Quaternion::ToEuler()
	{
		//// Referenced from:
		//// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
		//// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Private/Math/UnrealMath.cpp

		const float singularityThreshold = 0.4999995f;
		const float test = z * x - w * y;

		const float yawY = 2.0f * (w * z + x * y);
		const float yawX = (1.0f - 2.0f * (Math::Squared(y) + Math::Squared(z)));

		EulerAngles result(0, 0, 0);

		if (test > singularityThreshold)
		{
			result.z = atan2(yawY, yawX);
			result.y = Math::Pi / 2.0f;
			result.x = EulerAngles::NormalizeAxis(result.z - (2.0f * atan2(x, w)));
		}
		else if (test < -singularityThreshold)
		{
			result.z = atan2(yawY, yawX);
			result.y = -Math::Pi / 2.0f;
			result.x = EulerAngles::NormalizeAxis(-result.z - (2.0f * atan2(x, w)));
		}
		else
		{
			result.z = atan2(yawY, yawX);
			result.y = asin(2.f*(test));
			result.x = atan2(-2.f * (w * x + y * z), (1.f - 2.f * (Math::Squared(x) + Math::Squared(y))));
		}
		return result;
	}
}