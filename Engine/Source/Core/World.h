#pragma once
#include "Foundation.h"
#include "WorldObjects/Entity.h"

namespace Vanguard
{
	class Frame;
	class FrameJob;
	class RenderView;
	class WorldEvent;

	typedef std::function<void(Frame*)> TickFunction;

	struct IWorldEventListener
	{
		virtual void WorldEvent(WorldEvent* aEvent) = 0;
	};

	// A world contains all information about a running game. Typically, there is one world instance per game instance.
	// It is possible to have multiple worlds exist simultaneously, but typically, they do not directly communicate.
	// Eg. an Editor world, Play-in-editor world, and a game server world could all exist simultaneously, but should behave independently.
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
		// ODO: Use a linkedlist for the registeredTicks and eventListeners
		DynamicArray<Entity*> registeredTicks; 
		DynamicArray<IWorldEventListener*> eventListeners;

	public:
		World(String aWorldName);
		virtual ~World();

		String GetWorldName(){ return worldName; }
		virtual RenderView* GetPrimaryRenderView() { return nullptr; }

		void RegisterTick(Entity* aActor);
		void UnregisterTick(Entity* aActor);

		void PostEvent(WorldEvent* aEvent);
		void BroadcastEvent(WorldEvent* aEvent);
		virtual void RegisterEventListener(IWorldEventListener* aListener);
		virtual void UnregisterEventListener(IWorldEventListener* aListener);

		template <class T>		
		T* SpawnEntity()
		{
			Type* requestedClass = Type::GetType<T>();
			if (requestedClass == nullptr)
			{
				LOG_WARNING("Entity spawning failed, requested class could not be found", "World");
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
		virtual Timespan GetNextDesiredTickTime();
		virtual void Tick(Frame* aFrame);
		FrameJob* MakeTickJob(Frame* aFrame);
		void RegisterObject(VanguardObject* aObject);
	};
}