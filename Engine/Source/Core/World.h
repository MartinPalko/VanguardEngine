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

	class World
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

		World(String aWorldName)
			: worldName(aWorldName)
			, entities()
			, objects()
			, nextFrameNumber(0)
			, lastTickStartTime(0.0)
			, minimumTickDelta(1.0 / 60.0) // 60 FPS
			, registeredTicks()
		{
		}
		~World(){}

	protected:
		virtual void Tick(Frame* aFrame);

	public:

		String GetWorldName(){ return worldName; }

		void RegisterTick(TickFunction aTickFunction)
		{
			registeredTicks.PushBack(aTickFunction);
		}

		template<typename T, typename... U>
		size_t getAddress(std::function<T(U...)> f)
		{
			typedef T(fnType)(U...);
			fnType ** fnPointer = f.template target<fnType*>();
			return (size_t)*fnPointer;
		}

		void UnregisterTick(TickFunction aTickFunction)
		{	
			//TODO: Use a custom delegate class that supports equality comparison, so we can use .Remove()
			for (size_t i = 0; i < registeredTicks.Count(); i++)
				if (getAddress(aTickFunction) == getAddress(registeredTicks[i]))
					registeredTicks.RemoveAt(i);
		}

		template <class T>		
		T* SpawnEntity()
		{
			NativeClassInfo* requestedClass = NativeClassInfo::GetType<T>();
			if (requestedClass == nullptr)
			{
				Log::Warning("Entity spawning failed, requested class could not be found", "World");
				return nullptr;
			}

			return static_cast<T*>(SpawnEntity(requestedClass));
		}

		Entity* SpawnEntity(const String& aEntityType)
		{
			NativeClassInfo* requestedClass = NativeClassInfo::GetType(aEntityType);
			if (requestedClass == nullptr)
			{
				Log::Warning("Entity spawning failed, requested class " + aEntityType + " could not be found", "World");
				return nullptr;
			}

			return SpawnEntity(requestedClass);			
		}
		
		Entity* SpawnEntity(NativeClassInfo* aRequestedClass)
		{
			static NativeClassInfo* entityClass = NativeClassInfo::GetType<Entity>();

			if (!aRequestedClass->IsA(entityClass))
			{
				Log::Warning("Entity spawning failed, " + aRequestedClass->GetTypeName() + " is not a " + entityClass->GetTypeName(), "World");
				return nullptr;
			}

			Entity* newEntity = static_cast<Entity*>(aRequestedClass->CreateInstance());

			newEntity->world = this;
			objects.PushBack(newEntity);
			entities.PushBack(newEntity);

			for (int i = 0; i < newEntity->GetNumComponents(); i++)
			{
				Component* component = newEntity->GetComponent(i);
				component->world = this;
				objects.PushBack(component);
			}

			return newEntity;
		}

		template <class T> DynamicArray<T*> GetInstances()
		{
			// TODO: Store all types in this world in a hashtable for quick retreival.
			DynamicArray<T*> found;
			for (int i = 0; i < objects.Count(); i++)
			{
				VanguardObject* o = objects[i];

				if (o->GetClassInfo()->IsA(NativeClassInfo::GetType<T>()))
				{
					found.PushBack((T*)o);
				}
			}
			return found;
		}
	};
}