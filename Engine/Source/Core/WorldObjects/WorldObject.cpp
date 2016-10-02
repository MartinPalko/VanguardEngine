#include "WorldObject.h"
#include "World.h"
#include "Events.h"

namespace Vanguard
{
	ABSTRACT_BASETYPE_DEFINITION(WorldObject);

	void WorldObject::BroadcastEvent(WorldObjectEvent* aEvent)
	{
		for (auto listener : eventListeners)
			listener->OnWorldObjectEvent(aEvent);
	}

	WorldObject::WorldObject()
		: pendingDelete(false)
	{
	}

	void WorldObject::RegisterEventListener(IWorldObjectEventListener* aListener)
	{
		eventListeners.PushBack(aListener);
	}

	void WorldObject::UnregisterEventListener(IWorldObjectEventListener* aListener)
	{
		eventListeners.Remove(aListener);
	}

	void WorldObject::Destroy()
	{
		if (!pendingDelete)
		{
			pendingDelete = true;
			if (GetWorld())
			{
				GetWorld()->PostEvent(new ObjectDestroyedEvent(this));
			}
		}
	}
}