#pragma once
#include "Component.h"
#include "Foundation.h"
#include "Type.h"

namespace Vanguard
{
	class CORE_API Transform : public Component
	{
		TYPE_DECLARATION(Transform, Component);

	private:
		Vector3 position;
		Quaternion rotation;

	public:
		Transform() : position(0,0,0) { }

		Vector3 GetPosition();
		void SetPosition(const Vector3& aPosition);

		Vector2 GetPosition2D();
		void SetPosition2D(const Vector2& aPosition);

		void Move(const Vector3& aDirection);
		void Move(const Vector2& aDirection);

		Quaternion GetRotation();
		void SetRotation(const Quaternion& aRotation);

		float GetRotation2D();
		void SetRotation2D(float aRotation);
	};
}