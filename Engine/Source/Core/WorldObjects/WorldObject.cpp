#include "WorldObject.h"
#include "World.h"
#include "Events.h"

namespace Vanguard
{
	ABSTRACT_BASETYPE_DEFINITION(WorldObject);

	WorldObject::WorldObject()
		: pendingDelete(true)
	{
	}

	void WorldObject::Destroy()
	{
		pendingDelete = true;
		if(GetWorld())
		{
			GetWorld()->PostEvent(new ObjectDestroyedEvent(this));
		}
	}
}