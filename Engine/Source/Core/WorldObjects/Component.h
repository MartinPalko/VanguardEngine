#pragma once
#include "WorldObject.h"

namespace Vanguard
{
	class Entity;

	class CORE_API Component : public WorldObject
	{
		friend class Entity;

		TYPE_DECLARATION(Component,WorldObject)

	private:
		Entity* entity;

	public:
		Component() : entity(nullptr) {}

		Entity* GetEntity() { return entity; }
		virtual Box GetBounds() { return Box(); }
		
	};
}