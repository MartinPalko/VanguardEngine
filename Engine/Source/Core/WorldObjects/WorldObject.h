#pragma once
#include "Foundation.h"
#include "Type.h"

namespace Vanguard
{
	class World;
	class Entity;
	class Component;

	class CORE_API WorldObject
	{
		friend World;
		friend Entity;
		friend Component;

		BASETYPE_DECLARATION(WorldObject)
		
	private:
		World* world = nullptr;
	public:
		WorldObject(){}
		virtual ~WorldObject() {}
		World* GetWorld() { return world; }
	};
}