#pragma once
#include "Foundation.h"
#include "Entity.h"

namespace Vanguard
{
	class World
	{
		friend class Core;
	private:
		String worldName;

		List<Entity*> entities;

		World(String aWorldName)
		{
			worldName = aWorldName;
		}
		~World(){}

	public:

		String GetWorldName(){ return worldName; }

		template <class EntityType>		
		EntityType* SpawnEntity()
		{
			EntityType* newInstance = new EntityType();
			entities.PushBack(newInstance);
			return newInstance;
		}

		Entity* SpawnEntity(const String& aEntityType)
		{
			INativeClassInfo* nativeClass = INativeClassInfo::GetType(aEntityType);
			if (nativeClass == nullptr)
				return nullptr;

			// TODO: Set world, check if can be cast to entity
			return static_cast <Entity*> (nativeClass->CreateInstance());
		}
	};
}