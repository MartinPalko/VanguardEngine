#pragma once
#include <cstring>
#include "Foundation_Common.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace Vanguard
{
	struct FOUNDATION_API Matrix4x4
	{
	public:
		float elements[4][4];

		Matrix4x4(
			float x0, float y0, float z0, float w0,
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2,
			float x3, float y3, float z3, float w3);
		Matrix4x4(float aElements[16]) { memcpy(&elements[0][0], &aElements[0], sizeof(elements)); }
		Matrix4x4(float aElements[4][4]) { memcpy(&elements[0][0], &aElements[0][0], sizeof(elements)); }

		inline bool operator ==(const Matrix4x4& other) const
		{
			for (uint8 x = 0; x < 4; x++)
				for (uint8 y = 0; y < 4; y++)
					if (elements[x][y] != other.elements[x][y])
						return false;
			return true;
		}

		Matrix4x4 operator * (const Matrix4x4& aOther) const;

		Matrix4x4 GetInverse();

		inline Vector4 GetRow(uint8 aRow) const { return Vector4(elements[aRow][0], elements[aRow][1], elements[aRow][2], elements[aRow][3]);	}
		inline void SetRow(uint8 aRow, Vector4 aValue) { elements[aRow][0] = aValue.x; elements[aRow][1] = aValue.y; elements[aRow][2] = aValue.z; elements[aRow][3] = aValue.w; }

		inline Vector4 GetColumn(uint8 aCol) const { return Vector4(elements[0][aCol], elements[1][aCol], elements[2][aCol], elements[3][aCol]); }
		inline void SetColumn(uint8 aCol, Vector4 aValue) { elements[0][aCol] = aValue.x; elements[1][aCol] = aValue.y; elements[2][aCol] = aValue.z; elements[3][aCol] = aValue.w; }

		// Static functions
		static Matrix4x4 CreatePerspective(float aFov, float aAspect, float aZNear, float aZFar, bool aLeftHanded = false);
		static Matrix4x4 CreateScale(const Vector3& aScale);
		static Matrix4x4 CreateTranslation(const Vector3& aTransform);
		static Matrix4x4 CreateRotation(const Quaternion& aRotation);

		static Matrix4x4 GetIdentity();

	};
}
