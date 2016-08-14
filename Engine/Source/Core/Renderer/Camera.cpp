#include "Camera.h"

namespace Vanguard
{
	REFLECTED_SUBCLASS_DEFINITION(Camera, Entity);
	
	Camera::Camera() : Entity()
	{
		transform = AddComponent<Transform>();
	}
}