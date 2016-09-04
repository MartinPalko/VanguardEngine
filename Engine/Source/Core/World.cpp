#include "World.h"
#include "Entity/Actor.h"
#include "Jobs/Frame.h"
#include "Core.h"

namespace Vanguard
{
	World::World(String aWorldName)
		: worldName(aWorldName)
		, objects()
		, objectTypemap()
		, nextFrameNumber(0)
		, lastTickStartTime(0.0)
		, minimumTickDelta(0) // 60 FPS
		, maximumTickDelta(5) // 15 FPS
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

	void World::RegisterTick(Entity* aActor)
	{
		registeredTicks.PushBack(aActor);
	}

	void World::UnregisterTick(Entity* aActor)
	{
		registeredTicks.Remove(aActor);
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
		RegisterObject(newEntity);

		for (int i = 0; i < newEntity->GetNumComponents(); i++)
		{
			Component* component = newEntity->GetComponent(i);
			RegisterObject(component);
		}

		if (newEntity->tickEnabled && !newEntity->tickRegistered)
		{
			newEntity->EnableTick();
		}

		return newEntity;
	}

	DynamicArray<VanguardObject*> World::GetInstances(Type* aType, bool aIncludeInherited) const
	{
		DynamicArray<VanguardObject*> found = objectTypemap.at(aType->GetRuntimeHash());

		if (aIncludeInherited)
		{
			DynamicArray<Type*> derivedClasses = aType->GetDerivedClasses();
			for (int i = 0; i < derivedClasses.Count(); i++)
			{
				found += GetInstances(derivedClasses[i], true);
			}
		}
		return found;
	}
	
	void World::Tick(Frame* aFrame)
	{
		// Start render job before ticks, to render last frame's data.
		Core::GetInstance()->GetPrimaryRenderer()->StartRenderJob(aFrame);

		// Dispatch all ticks to the job system.
		for (size_t i = 0; i < registeredTicks.Count(); i++)
		{
			Entity* entity = registeredTicks[i];
			aFrame->AddJob(entity->GetClassInfo()->GetTypeName() + " Tick" , [entity, aFrame]()-> void {entity->Tick(aFrame); });
		}
	}

	void World::RegisterObject(VanguardObject * aObject)
	{
		aObject->world = this;
		objects.PushBack(aObject);
		objectTypemap[aObject->GetClassInfo()->GetRuntimeHash()].PushBack(aObject);
	}

}
