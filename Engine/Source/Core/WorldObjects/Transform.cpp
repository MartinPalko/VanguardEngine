#include "Transform.h"

namespace Vanguard
{
	TYPE_DEFINITION(Transform, Component);

	START_REGISTER_PROPERTIES(Transform);
	{
		REGISTER_PROPERTY(Transform, position);
		REGISTER_PROPERTY(Transform, rotation);
	}
	FINISH_REGISTER_PROPERTIES(Transform);

	Vector3 Transform::GetPosition()
	{
		return position;
	}

	void Transform::SetPosition(const Vector3& aPosition)
	{
		position = aPosition;
	}

	Vector2 Transform::GetPosition2D()
	{
		const Vector3 position3D = GetPosition();
		return Vector2(position3D.x, position3D.y);
	}

	void Transform::SetPosition2D(const Vector2& aPosition)
	{
		const Vector3 currentPosition = GetPosition();
		SetPosition(Vector3(aPosition.x, aPosition.y, currentPosition.z));
	}

	void Transform::Move(const Vector3& aDirection)
	{
		SetPosition(GetPosition() + aDirection);
	}

	void Transform::Move(const Vector2& aDirection)
	{
		SetPosition2D(GetPosition2D() + aDirection);
	}

	Quaternion Transform::GetRotation()
	{
		return rotation;
	}

	void Transform::SetRotation(const Quaternion& aRotation)
	{
		rotation = aRotation;
	}

	float Transform::GetRotation2D()
	{
		return rotation.ToEuler().z;
	}

	void Transform::SetRotation2D(float aRotation)
	{
		rotation = Quaternion::FromEuler(EulerAngles(0, 0, aRotation));
	}
}