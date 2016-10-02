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
		, minimumTickDelta(1.0 / 60) // 60 FPS
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

	void World::RegisterTick(Entity* aActor)
	{
		registeredTicks.PushBack(aActor);
	}

	void World::UnregisterTick(Entity* aActor)
	{
		registeredTicks.Remove(aActor);
	}

	void World::PostEvent(WorldEvent* aEvent)
	{
		Core::GetInstance()->PostEvent(aEvent);
	}

	void World::BroadcastEvent(WorldEvent* aEvent)
	{
		ASSERT_MAIN_THREAD;

		DEBUG_LOG("World Event: " + aEvent->GetType()->GetTypeName());

		for (auto listener : eventListeners)
		{
			listener->WorldEvent(aEvent);
		}
	}

	void World::RegisterEventListener(IWorldEventListener* aListener)
	{
		eventListeners.PushBack(aListener);
	}

	void World::UnregisterEventListener(IWorldEventListener* aListener)
	{
		eventListeners.Remove(aListener);
	}

	Entity* World::SpawnEntity(const String & aEntityType)
	{
		Type* requestedClass = Type::GetType(aEntityType);
		if (requestedClass == nullptr)
		{
			LOG_WARNING("Entity spawning failed, requested class " + aEntityType + " could not be found", "World");
			return nullptr;
		}

		return SpawnEntity(requestedClass);
	}

	Entity* World::SpawnEntity(Type* aRequestedClass)
	{
		Type* entityType = Type::GetType<Entity>();

		if (!aRequestedClass->IsA(entityType))
		{
			LOG_WARNING("Entity spawning failed, " + aRequestedClass->GetTypeName() + " is not a " + entityType->GetTypeName(), "World");
			return nullptr;
		}

		Entity* newEntity = static_cast<Entity*>(aRequestedClass->CreateInstance());
		
		// Register
		RegisterObject(newEntity);
		for (int i = 0; i < newEntity->GetNumComponents(); i++)
		{
			RegisterObject(newEntity->GetComponent(i));
		}

		// Re-enable tick if it is meant to be disabled, now that we're registered.
		if (newEntity->tickEnabled && !newEntity->tickRegistered)
		{
			newEntity->EnableTick();
		}

		// Post creation events
		PostEvent(new ObjectCreatedEvent(newEntity));
		for (int i = 0; i < newEntity->GetNumComponents(); i++)
		{
			PostEvent(new ObjectCreatedEvent(newEntity->GetComponent(i)));
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

	class TickJob : public FrameJob
	{
	protected:
		Entity* entity;

	public:
		TickJob(const String& aName, Frame* aFrame, Entity* aEntity) : FrameJob(aName, aFrame)
		{
			entity = aEntity;
		}

	protected:
		virtual void DoJob() override
		{
			entity->Tick(frame);
		}
	};

	Timespan World::GetNextDesiredTickTime()
	{
		return lastTickStartTime + minimumTickDelta;
	}

	void World::Tick(Frame* aFrame)
	{
		// Start render job before ticks, to render last frame's data.
		Core::GetInstance()->GetPrimaryRenderer()->StartRenderJob(aFrame);

		// Dispatch all ticks to the job system.
		FrameJob** jobs = new FrameJob*[registeredTicks.Count()];		
		for (size_t i = 0; i < registeredTicks.Count(); i++)
		{
			jobs[i] = new TickJob(registeredTicks[i]->GetType()->GetTypeName(), aFrame, registeredTicks[i]);
		}
		aFrame->AddJobs(jobs, registeredTicks.Count());

		delete[] jobs;
	}

	FrameJob* World::MakeTickJob(Frame* aFrame)
	{
		class WorldTickJob : public FrameJob
		{
		protected:
			World* world;

		public:
			WorldTickJob(const String& aName, Frame* aFrame, World* aWorld) : FrameJob(aName, aFrame)
			{
				world = aWorld;
			}

		protected:
			virtual void DoJob() override
			{
				world->Tick(frame);
			}
		};

		return new WorldTickJob(worldName + " Tick", aFrame, this);
	}

	void World::RegisterObject(VanguardObject * aObject)
	{
		aObject->world = this;
		objects.PushBack(aObject);
		objectTypemap[aObject->GetType()->GetRuntimeHash()].PushBack(aObject);
	}

}
