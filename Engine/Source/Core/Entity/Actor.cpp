#include "Actor.h"

namespace Vanguard
{
	TYPE_DEFINITION(Actor, Entity)

	Actor::Actor() : Entity()
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
}