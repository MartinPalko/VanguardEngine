#pragma once
#include "Foundation.h"
#include "NativeReflection.h"

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

		REFLECTED_BASECLASS_DECLARATION(VanguardObject)
		
	private:
		World* world;
	protected:
		VanguardObject(){}
	public:
		World* GetWorld() { return world; }
	};
}