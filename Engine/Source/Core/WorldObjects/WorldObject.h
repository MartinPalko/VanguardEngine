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

		ABSTRACT_BASETYPE_DECLARATION(WorldObject);

		bool pendingDelete;
		
	private:
		World* world = nullptr;
	public:
		WorldObject();
		virtual ~WorldObject() {}
		World* GetWorld() { return world; }

		void Destroy();
		bool PendingDelete() { return pendingDelete; }
	};
}