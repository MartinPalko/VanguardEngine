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
		Entity* GetEntity() { return entity; }
		
	};
}