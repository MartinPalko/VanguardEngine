#pragma once

#include "Entity\Entity.h"
#include "Entity\Transform.h"

namespace Vanguard
{
	class CORE_API Camera : public Entity
	{
		REFLECTED_SUBCLASS_DECLARATION(Camera, Entity);

	public:
		Camera();

	protected:
		float fov;
		bool orthographic;
		Transform* transform;

	public:
		Transform* GetTransform() { return transform; }

	};
}