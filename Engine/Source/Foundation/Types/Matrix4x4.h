#pragma once
#include "Foundation_Common.h"
#include "Vector4.h"

namespace Vanguard
{
	struct FOUNDATION_API Matrix4x4
	{
	public:
		Matrix4x4(
			float x0, float y0, float z0, float w0,
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2,
			float x3, float y3, float z3, float w3)
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
		Matrix4x4(float aElements[16]) : elements{ *aElements } {}
		Matrix4x4(float aElements[4][4]) : elements{ **aElements } {}

		float elements[4][4];

		inline Vector4 GetRow(uint8 aRow) { return Vector4(elements[aRow][0], elements[aRow][1], elements[aRow][2], elements[aRow][3]);	}
		inline void SetRow(uint8 aRow, Vector4 aValue) { elements[aRow][0] = aValue.x; elements[aRow][1] = aValue.y; elements[aRow][2] = aValue.z; elements[aRow][3] = aValue.w; }

		inline Vector4 GetColumn(uint8 aCol) { return Vector4(elements[0][aCol], elements[1][aCol], elements[2][aCol], elements[3][aCol]); }		
		inline void SetColumn(uint8 aCol, Vector4 aValue) { elements[0][aCol] = aValue.x; elements[1][aCol] = aValue.y; elements[2][aCol] = aValue.z; elements[3][aCol] = aValue.w; }

		// Static functions

		static Matrix4x4 CreatePerspective(float aFov, float aAspect, float aZNear, float aZFar, bool aLeftHanded = false);

		static Matrix4x4 GetIdentity();
		
	};
}