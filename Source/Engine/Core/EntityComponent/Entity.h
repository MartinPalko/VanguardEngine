#pragma once
#include "Foundation.h"
#include "NativeReflection.h"

namespace Vanguard
{
	class World;
	class Component;

	class CORE_API Entity
	{
		REFLECTED_CLASS_DECLARATION(Entity)

		friend class World;
	public:
		Entity(){}
		virtual ~Entity(){}

		List<Component*> components;
		World* world;

	public:
		List<Component*> GetComponents() { return components; }

		World* GetWorld() { return world; }
	};
}