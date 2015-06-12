#pragma once
#include "Foundation.h"
#include "NativeReflection.h"

namespace Vanguard
{
	class Entity;

	class CORE_API Component
	{
		REFLECTED_BASECLASS_DECLARATION(Component)

		Entity* entity;
	};
}