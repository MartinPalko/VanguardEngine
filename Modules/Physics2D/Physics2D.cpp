#include "Physics2D.h"

#include "PhysicsWorld2D.h"

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(Physics2D);

	void Physics2D::CreatePhysicsWorld(World* aVanguardWorld)
	{
		DEBUG_EXCEPTION_IF(physicsWorlds.count(aVanguardWorld), "Physics world already exists for this vanguard world!");

		physicsWorlds[aVanguardWorld] = new PhysicsWorld2D(aVanguardWorld);
	}

	void Physics2D::DestroyPhysicsWorld(World* aVanguardWorld)
	{
		DEBUG_EXCEPTION_IF(physicsWorlds.count(aVanguardWorld) == 0, "No physics world exists for this vanguard world!");

		PhysicsWorld2D* physicsWorld = physicsWorlds.at(aVanguardWorld);
		physicsWorlds.erase(aVanguardWorld);
		delete physicsWorld;
	}

	Physics2D::Physics2D() : physicsWorlds()
	{

	}

	Physics2D::~Physics2D()
	{

	}

	void Physics2D::LoadModule()
	{
		Core::GetInstance()->RegisterEventListener(this);

		for (World* world : Core::GetInstance()->GetWorlds())
		{
			CreatePhysicsWorld(world);
		}
	}

	void Physics2D::UnloadModule()
	{
		Core::GetInstance()->UnregisterEventListener(this);

		for (World* world : Core::GetInstance()->GetWorlds())
		{
			DestroyPhysicsWorld(world);
		}
	}

	void Physics2D::OnCoreEvent(Event* aEvent)
	{
		if (auto worldAddedEvent = Type::SafeCast<WorldAddedEvent>(aEvent))
		{
			CreatePhysicsWorld(worldAddedEvent->GetWorld());
		}
		else if (auto worldRemovedEvent = Type::SafeCast<WorldRemovedEvent>(aEvent))
		{
			DestroyPhysicsWorld(worldRemovedEvent->GetWorld());
		}
	}
}