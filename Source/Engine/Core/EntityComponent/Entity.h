#pragma once
#include "Foundation.h"
#include "NativeReflection.h"

namespace Vanguard
{
	class World;
	class Component;

	class CORE_API Entity
	{
		friend class World;

		REFLECTED_BASECLASS_DECLARATION(Entity)
		
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