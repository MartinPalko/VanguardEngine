#include "Camera.h"

namespace Vanguard
{
	TYPE_DEFINITION(Camera, Actor);
	
	START_REGISTER_PROPERTIES(Camera);
	{
		REGISTER_PROPERTY(Camera, fov);
		REGISTER_PROPERTY(Camera, orthographic);
		REGISTER_PROPERTY(Camera, zNear);
		REGISTER_PROPERTY(Camera, zFar);
	}
	FINISH_REGISTER_PROPERTIES(Camera);

	Camera::Camera() : Actor()
		, fov (60.0f)
		, orthographic(false)
		, zNear(0.1f)
		, zFar(1000.0f)
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
			const Matrix4x4 translation = Matrix4x4::CreateTranslation(Vector3(0.0f, 0.0f, zNear));
			const Matrix4x4 scale = Matrix4x4::CreateScale(Vector3(1.0f / (fov / 2.0f), 1.0f / (fov / 2.0f), 1 / (zFar - zNear)));
			return translation * scale;
		}
		
	}
}