#pragma once
#include "Component.h"

#include "Foundation.h"

namespace Vanguard
{
	class CORE_API Transform : public Component
	{
		REFLECTED_SUBCLASS_DECLARATION(Transform, Component)

	public:
		Transform():position(0,0,0) {}

		Vector3 position;
		Quaternion rotation;
	};
}