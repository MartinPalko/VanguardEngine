#include "Camera.h"

namespace Vanguard
{
	TYPE_DEFINITION(Camera, Entity);
	
	Camera::Camera() : Entity()
	{
		transform = AddComponent<Transform>();
	}
}