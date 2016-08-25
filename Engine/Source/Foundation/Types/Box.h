#pragma once
#include "Foundation_Common.h"
#include "Collections/DynamicArray.h"
#include "Vector3.h"

namespace Vanguard
{
	struct FOUNDATION_API Box
	{
		Vector3 min;
		Vector3 max;

		inline Box(Vector3 aMin, Vector3 aMax) : min(aMin), max(aMax) {}
		inline Box(Vector3 aSize) : Box(-aSize / 2, aSize / 2) {}
		inline Box() : Box(Vector3(0, 0, 0), Vector3(0, 0, 0)) {}

		inline void AddBox(const Box& aOtherBox)
		{
			min = Vector3::Min(min, aOtherBox.min);
			max = Vector3::Max(max, aOtherBox.max);
		}

		inline void AddPoint(const Vector3& aPoint)
		{
			min = Vector3::Min(min, aPoint);
			max = Vector3::Max(max, aPoint);
		}

		inline bool Intersects(const Box& aOtherBox)
		{
			return !(
				aOtherBox.min.x > max.x || 
				aOtherBox.max.x < min.x || 
				aOtherBox.min.y > max.y ||
				aOtherBox.max.y < min.y ||
				aOtherBox.min.z > max.z ||
				aOtherBox.max.z < min.z
				);
		}

		DynamicArray<Vector3> GetCorners()
		{
			DynamicArray<Vector3> corners(8);
			corners.PushBack(Vector3(max.x, max.y, min.z));
			corners.PushBack(Vector3(min.x, max.y, min.z));
			corners.PushBack(Vector3(max.x, min.y, min.z));
			corners.PushBack(Vector3(min.x, min.y, min.z));
			corners.PushBack(Vector3(max.x, max.y, max.z));
			corners.PushBack(Vector3(min.x, max.y, max.z));
			corners.PushBack(Vector3(max.x, min.y, max.z));
			corners.PushBack(Vector3(min.x, min.y, max.z));
			return corners;
		}
	};
}
