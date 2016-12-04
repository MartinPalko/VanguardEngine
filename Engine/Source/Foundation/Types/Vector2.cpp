#include "Vector2.h"
#include "Vector3.h"

namespace Vanguard
{
	const Vector2 Vector2::Up = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::Down = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::Left = Vector2(-1.0f, 0.0f);
	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
	
	Vector2::operator Vector3() const
	{
		return Vector3(x, y, 0);
	}
}