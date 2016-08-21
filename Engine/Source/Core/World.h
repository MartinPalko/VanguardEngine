#pragma once
#include "Foundation.h"
#include "Entity/Entity.h"

namespace Vanguard
{
	class Frame;
}

namespace Vanguard
{
	typedef std::function<void(Frame*)> TickFunction;

	class CORE_API World
	{
		friend class Core;
		friend class Entity;

	private:
		String worldName;
		DynamicArray<Entity*> entities;
		DynamicArray<VanguardObject*> objects;
		uint32 nextFrameNumber;
		Timespan lastTickStartTime;
		Timespan minimumTickDelta;
		DynamicArray<TickFunction> registeredTicks; //TODO: Use a linkedlist for the registeredTicks	

		World(String aWorldName);
		~World();

	protected:
		virtual void Tick(Frame* aFrame);

	public:

		String GetWorldName(){ return worldName; }

		void RegisterTick(TickFunction aTickFunction);
		void UnregisterTick(TickFunction aTickFunction);

		template <class T>		
		T* SpawnEntity()
		{
			Type* requestedClass = Type::GetType<T>();
			if (requestedClass == nullptr)
			{
				Log::Warning("Entity spawning failed, requested class could not be found", "World");
				return nullptr;
			}

			return static_cast<T*>(SpawnEntity(requestedClass));
		}

		Entity* SpawnEntity(const String& aEntityType);		
		Entity* SpawnEntity(Type* aRequestedClass);
		template <class T> DynamicArray<T*> GetInstances()
		{
			// TODO: Store all types in this world in a hashtable for quick retreival.
			DynamicArray<T*> found;
			for (int i = 0; i < objects.Count(); i++)
			{
				VanguardObject* o = objects[i];

				if (o->GetClassInfo()->IsA(Type::GetType<T>()))
				{
					found.PushBack((T*)o);
				}
			}
			return found;
		}
	};
}