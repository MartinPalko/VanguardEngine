#pragma once

#include "Entity/Actor.h"
#include "Entity/Transform.h"
#include "Types/Matrix4x4.h"

namespace Vanguard
{
	class CORE_API Camera : public Actor
	{
		TYPE_DECLARATION(Camera, Actor);

	public:
		Camera();

	protected:
		float fov;
		bool orthographic;
		float zNear;
		float zFar;

	public:

		Matrix4x4 GetProjectionMatrix();

	};
}
