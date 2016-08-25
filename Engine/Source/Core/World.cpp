#include "World.h"
#include "Entity/Actor.h"
#include "Jobs/Frame.h"

namespace Vanguard
{
	World::World(String aWorldName)
		: worldName(aWorldName)
		, entities()
		, objects()
		, nextFrameNumber(0)
		, lastTickStartTime(0.0)
		, minimumTickDelta(1.0 / 60.0) // 60 FPS
		, maximumTickDelta(1.0 / 15.0) // 15 FPS
		, registeredTicks()
	{
	}

	World::~World()
	{
		for (int i = 0; i < objects.Count(); i++)
		{
			// We own these objects, so clean them up.
			objects[i]->world = nullptr;
			delete objects[i];
		}
	}

	void World::Tick(Frame* aFrame)
	{
		DynamicArray<Job*> jobs;

		// Dispatch all ticks to the job system.
		for (size_t i = 0; i < registeredTicks.Count(); i++)
		{
			std::function<void(Frame*)> tickFunction = registeredTicks[i];
			jobs.PushBack(aFrame->AddJob([tickFunction, aFrame]()-> void {tickFunction(aFrame); }));
		}

		// Wait for all jobs to complete.
		for (size_t i = 0; i < jobs.Count(); i++)
		{
			aFrame->WaitForJob(jobs[i]);
		}
	}

	template<typename T, typename... U>
	size_t getAddress(std::function<T(U...)> f)
	{
		typedef T(fnType)(U...);
		fnType ** fnPointer = f.template target<fnType*>();
		return (size_t)*fnPointer;
	}

	void World::RegisterTick(TickFunction aTickFunction)
	{
		registeredTicks.PushBack(aTickFunction);
	}

	void World::UnregisterTick(TickFunction aTickFunction)
	{
		//TODO: Use a custom delegate class that supports equality comparison, so we can use .Remove()
		for (size_t i = 0; i < registeredTicks.Count(); i++)
			if (getAddress(aTickFunction) == getAddress(registeredTicks[i]))
				registeredTicks.RemoveAt(i);
	}

	Entity* World::SpawnEntity(const String & aEntityType)
	{
		Type* requestedClass = Type::GetType(aEntityType);
		if (requestedClass == nullptr)
		{
			Log::Warning("Entity spawning failed, requested class " + aEntityType + " could not be found", "World");
			return nullptr;
		}

		return SpawnEntity(requestedClass);
	}

	Entity* World::SpawnEntity(Type* aRequestedClass)
	{
		Type* entityType = Type::GetType<Entity>();

		if (!aRequestedClass->IsA(entityType))
		{
			Log::Warning("Entity spawning failed, " + aRequestedClass->GetTypeName() + " is not a " + entityType->GetTypeName(), "World");
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

		if (aRequestedClass->IsA(Type::GetType<Actor>()))
		{
			Actor* newActor = static_cast<Actor*>(newEntity);
			if (newActor->tickEnabled && !newActor->tickRegistered)
			{
				newActor->EnableTick();
			}
		}

		return newEntity;
	}
}
