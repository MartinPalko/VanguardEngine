#pragma once

#include "Entity/Entity.h"
#include "Entity/Transform.h"
#include "Types/Matrix4x4.h"

namespace Vanguard
{
	class CORE_API Camera : public Entity
	{
		TYPE_DECLARATION(Camera, Entity);

	public:
		Camera();

	protected:
		float fov;
		bool orthographic;
		float zNear;
		float zFar;
		Transform* transform;

	public:
		Transform* GetTransform() { return transform; }

		Matrix4x4 GetMatrix();

	};
}
