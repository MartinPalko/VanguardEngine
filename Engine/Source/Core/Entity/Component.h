#pragma once
#include "VanguardObject.h"

namespace Vanguard
{
	class Entity;

	class CORE_API Component : public VanguardObject
	{
		friend class Entity;

		TYPE_DECLARATION(Component,VanguardObject)

	private:
		Entity* entity;

	public:
		Component() : entity(nullptr) {}

		Entity* GetEntity() { return entity; }
		virtual Box GetBounds() { return Box(); }
		
	};
}