#include "Actor.h"

namespace Vanguard
{
	TYPE_DEFINITION(Actor, Entity)

	Actor::Actor() : Entity()
		, localBounds()
	{
		tickEnabled = false;
		tickRegistered = false;
		tickFunction = [this](Frame* aFrame) { this->Tick(aFrame); };
		transform = AddComponent<Transform>();
	}

	void Actor::EnableTick()
	{
		if (GetWorld())
		{
			GetWorld()->RegisterTick(tickFunction);
			tickRegistered = true;
		}
		tickEnabled = true;
	}

	void Actor::DisableTick()
	{
		if (GetWorld())
		{
			GetWorld()->UnregisterTick(tickFunction);
			tickRegistered = false;
		}
		tickEnabled = false;
	}

	void Actor::UpdateBounds()
	{
		localBounds = Box();
		for (int i = 0; i < GetNumComponents(); i++)
		{
			localBounds.AddBox(GetComponent(i)->GetBounds());
		}
	}

	void Actor::ComponentAdded(Component* aComponent)
	{
		Entity::ComponentAdded(aComponent);
		UpdateBounds();
	}
}