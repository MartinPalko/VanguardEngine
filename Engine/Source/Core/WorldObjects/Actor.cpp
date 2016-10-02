#include "Actor.h"

namespace Vanguard
{
	TYPE_DEFINITION(Actor, Entity)

	Actor::Actor() : Entity()
		, localBounds()
	{
		transform = AddComponent<Transform>();
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