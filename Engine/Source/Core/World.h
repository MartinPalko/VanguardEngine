#pragma once
#include "Foundation.h"
#include "Entity/Entity.h"

namespace Vanguard
{
	class Frame;
	class FrameJob;
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

		// Stores all objects in the world, in a contiguous array for fast iteration.
		DynamicArray<VanguardObject*> objects;
		// Stores all object in world, sorted by type.
		std::map<size_t, DynamicArray<VanguardObject*>> objectTypemap;

		uint32 nextFrameNumber;
		Timespan lastTickStartTime;
		Timespan minimumTickDelta;
		Timespan maximumTickDelta;
		DynamicArray<Entity*> registeredTicks; //TODO: Use a linkedlist for the registeredTicks

	public:
		World(String aWorldName);
		~World();

		String GetWorldName(){ return worldName; }

		void RegisterTick(Entity* aActor);
		void UnregisterTick(Entity* aActor);

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

		DynamicArray<VanguardObject*> GetInstances (Type* aType, bool aIncludeInherited = false) const;
		template <class T> DynamicArray<T*> GetInstances(bool aIncludeInherited = false) const
		{
			// Can't just cast, have to do an actual copy over
			DynamicArray<VanguardObject*> objectArray(GetInstances(Type::GetType<T>()));
			DynamicArray<T*> returnArray(objectArray.Count());
			for (int i = 0; i < objectArray.Count(); i++)
			{
				returnArray.PushBack((T*)objectArray[i]);
			}
			return returnArray;
		}

	protected:
		virtual void Tick(Frame* aFrame);
		FrameJob* MakeTickJob(Frame* aFrame);
		void RegisterObject(VanguardObject* aObject);
	};
}