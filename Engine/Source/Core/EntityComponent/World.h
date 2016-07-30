#pragma once
#include "Foundation.h"
#include "Entity.h"

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
	private:
		String worldName;
		DynamicArray<Entity*> entities;
		uint32 nextFrameNumber;
		Timespan lastTickStartTime;
		DynamicArray<TickFunction> registeredTicks; //TODO: Use a linkedlist for the registeredTicks	

		World(String aWorldName)
			: worldName(aWorldName)
			, nextFrameNumber(0)
			, lastTickStartTime(0.0)
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