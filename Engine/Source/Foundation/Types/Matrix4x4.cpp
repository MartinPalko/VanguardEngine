#include "Foundation_Common.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace Vanguard
{
	Matrix4x4::Matrix4x4(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3)
	{
		elements[0][0] = x0;
		elements[1][0] = y0;
		elements[2][0] = z0;
		elements[3][0] = w0;
		elements[0][1] = x1;
		elements[1][1] = y1;
		elements[2][1] = z1;
		elements[3][1] = w1;
		elements[0][2] = x2;
		elements[1][2] = y2;
		elements[2][2] = z2;
		elements[3][2] = w2;
		elements[0][3] = x3;
		elements[1][3] = y3;
		elements[2][3] = z3;
		elements[3][3] = w3;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & aOther) const
	{
		return Matrix4x4(
			GetRow(0).Dot(aOther.GetColumn(0)), GetRow(0).Dot(aOther.GetColumn(1)), GetRow(0).Dot(aOther.GetColumn(2)), GetRow(0).Dot(aOther.GetColumn(3)),
			GetRow(1).Dot(aOther.GetColumn(0)), GetRow(1).Dot(aOther.GetColumn(1)), GetRow(1).Dot(aOther.GetColumn(2)), GetRow(1).Dot(aOther.GetColumn(3)),
			GetRow(2).Dot(aOther.GetColumn(0)), GetRow(2).Dot(aOther.GetColumn(1)), GetRow(2).Dot(aOther.GetColumn(2)), GetRow(2).Dot(aOther.GetColumn(3)),
			GetRow(3).Dot(aOther.GetColumn(0)), GetRow(3).Dot(aOther.GetColumn(1)), GetRow(3).Dot(aOther.GetColumn(2)), GetRow(3).Dot(aOther.GetColumn(3)));
	}

	Matrix4x4 Matrix4x4::GetInverse()
	{
		const float* m = (float*)&elements;
		float inv[16];
		float det;
		int i;

		inv[0] =
			m[5] * m[10] * m[15] -
			m[5] * m[11] * m[14] -
			m[9] * m[6] * m[15] +
			m[9] * m[7] * m[14] +
			m[13] * m[6] * m[11] -
			m[13] * m[7] * m[10];

		inv[4] = 
			-m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		inv[8] = 
			m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		inv[12] = 
			-m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		inv[1] = 
			-m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		inv[5] = 
			m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		inv[9] = 
			-m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		inv[13] = 
			m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		inv[2] = 
			m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		inv[6] = 
			-m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		inv[10] = 
			m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		inv[14] =
			-m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		inv[3] = 
			-m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		inv[11] = 
			-m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		inv[15] = 
			m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return GetIdentity();

		det = 1.0f / det;

		float invOut[16];

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return invOut;
	}

	Vector4 Matrix4x4::Transform(const Vector4& aVector) const
	{
		return Vector4(GetRow(0).Dot(aVector), GetRow(1).Dot(aVector), GetRow(2).Dot(aVector), GetRow(3).Dot(aVector));
	}

	Vector3 Matrix4x4::TransformDirection(const Vector3& aPoint) const
	{
		const Vector4 result = Transform(Vector4(aPoint.x, aPoint.y, aPoint.z, 0));
		return Vector3(result.x, result.y, result.z);
	}

	Vector3 Matrix4x4::TransformPoint(const Vector3& aPoint) const
	{
		const Vector4 result = Transform(Vector4(aPoint.x, aPoint.y, aPoint.z, 1));
		return Vector3(result.x, result.y, result.z);
	}

	Matrix4x4 Matrix4x4::CreatePerspective(float aFov, float aAspect, float aZNear, float aZFar, bool aLeftHanded)
	{
		Matrix4x4 result = GetIdentity();

		if (aFov <= 0 || aAspect == 0) // Bad parameters
		{
			return result;
		}

		const float frustumDepth = aZFar - aZNear;
		const float oneOverDepth = 1.0f / frustumDepth;

		result.elements[1][1] = 1.0f / Math::FTan(0.5f * aFov);
		result.elements[0][0] = (aLeftHanded ? 1.0f : -1.0f) * result.elements[1][1] / aAspect;
		result.elements[2][2] = aZFar * oneOverDepth;
		result.elements[3][2] = (-aZFar * aZNear) * oneOverDepth;
		result.elements[2][3] = 1.0f;
		result.elements[3][3] = 0.0f;

		return result;
	}

	Matrix4x4 Matrix4x4::CreateScale(const Vector3& aScale)
	{
		return Matrix4x4(
			aScale.x, 0.0f, 0.0f, 0.0f,
			0.0f, aScale.y, 0.0f, 0.0f,
			0.0f, 0.0f, aScale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& aTransform)
	{
		return Matrix4x4(
			1.0f, 0.0f, 0.0f, aTransform.x,
			0.0f, 1.0f, 0.0f, aTransform.y,
			0.0f, 0.0f, 1.0f, aTransform.z,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4x4 Matrix4x4::CreateRotation(const Quaternion& aRotation)
	{
		const Quaternion q = aRotation.GetNormalized();

		return Matrix4x4(
			q.w, q.z, -q.y, q.x,
			-q.z, q.w, q.x, q.y,
			q.y, -q.x, q.w, q.z,
			-q.x, -q.y, -q.z, q.w) *
		Matrix4x4(
			q.w, q.z, -q.y, q.x,
			-q.z, q.w, q.x, -q.y,
			q.y, -q.x, q.w, q.z,
			q.x, q.y, q.z, q.w);
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
