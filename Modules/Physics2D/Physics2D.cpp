#include "Physics2D.h"

#include "PhysicsWorld2D.h"

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(Physics2D);

	Physics2D::Physics2D() : physicsWorlds()
	{

	}

	Physics2D::~Physics2D()
	{

	}

	void Physics2D::LoadModule()
	{
		Core::GetInstance()->RegisterEventListener(this);
	}

	void Physics2D::UnloadModule()
	{
		Core::GetInstance()->UnregisterEventListener(this);
	}

	void Physics2D::OnCoreEvent(Event* aEvent)
	{
		if (auto worldAddedEvent = Type::SafeCast<WorldAddedEvent>(aEvent))
		{
			World* world = worldAddedEvent->GetWorld();

			DEBUG_EXCEPTION_IF(physicsWorlds.count(world), "Physics world already exists for this world! This should not happen.");

			physicsWorlds[world] = new PhysicsWorld2D(world);

		}
		else if (auto worldRemovedEvent = Type::SafeCast<WorldRemovedEvent>(aEvent))
		{
		}
	}
}