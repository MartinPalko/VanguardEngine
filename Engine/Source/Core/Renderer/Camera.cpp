#include "Camera.h"

namespace Vanguard
{
	TYPE_DEFINITION(Camera, Actor);
	
	Camera::Camera() : Actor()
		, fov (60)
		, orthographic(false)
		, zNear(0.1)
		, zFar(1000)
	{
	}

	Matrix4x4 Camera::GetProjectionMatrix()
	{
		if (!orthographic)
		{
			return Matrix4x4::CreatePerspective(fov, orthographic, zNear, zFar);
		}
		else
		{
			return Matrix4x4::CreateTranslation(Vector3(0.0f, 0.0f, zNear)) * Matrix4x4::CreateScale(Vector3(1.0f / fov, 1.0f / fov, zFar - zNear));
		}
		
	}
}