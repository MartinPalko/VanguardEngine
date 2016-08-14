#pragma once
#include "Foundation_Common.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace Vanguard
{
	Matrix4x4 Matrix4x4::CreatePerspective(float aFov, float aAspect, float aZNear, float aZFar, bool aLeftHanded)
	{
		Matrix4x4 result = GetIdentity();

		if (aFov <= 0 || aAspect == 0) // Bad parameters
		{
			return result;
		}

		const float frustumDepth = aZFar - aZNear;
		const float oneOverDepth = 1 / frustumDepth;

		result.elements[1][1] = 1 / tan(0.5f * aFov);
		result.elements[0][0] = (aLeftHanded ? 1 : -1) * result.elements[1][1] / aAspect;
		result.elements[2][2] = aZFar * oneOverDepth;
		result.elements[3][2] = (-aZFar * aZNear) * oneOverDepth;
		result.elements[2][3] = 1;
		result.elements[3][3] = 0;

		return result;
	}
	Matrix4x4 Matrix4x4::GetIdentity()
	{
		return Matrix4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
}