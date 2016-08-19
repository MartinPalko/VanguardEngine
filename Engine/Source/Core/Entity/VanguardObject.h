#pragma once
#include "Foundation.h"
#include "Type.h"

namespace Vanguard
{
	class World;
	class Entity;
	class Component;

	class CORE_API VanguardObject
	{
		friend World;
		friend Entity;
		friend Component;

		BASETYPE_DECLARATION(VanguardObject)
		
	private:
		World* world = nullptr;
	public:
		VanguardObject(){}
		virtual ~VanguardObject() {}
		World* GetWorld() { return world; }
	};
}