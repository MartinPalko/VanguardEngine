#include "Camera.h"

namespace Vanguard
{
	TYPE_DEFINITION(Camera, Entity);
	
	Camera::Camera() : Entity()
		, fov (60)
		, orthographic(false)
		, zNear(0.1)
		, zFar(1000)
	{
		transform = AddComponent<Transform>();
	}

	Matrix4x4 Camera::GetProjectionMatrix()
	{
		if (!orthographic)
		{
			return Matrix4x4::CreatePerspective(fov, orthographic, zNear, zFar);
		}
		else
		{
			// TODO: Orthographic matrix.
			return Matrix4x4::GetIdentity();
		}
		
	}
}