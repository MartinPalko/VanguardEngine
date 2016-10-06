#include "Events.h"
#include "WorldObjects/WorldObject.h"

namespace Vanguard
{
	ABSTRACT_BASETYPE_DEFINITION(Event);

	ABSTRACT_TYPE_DEFINITION(WorldEvent, Event);

	TYPE_DEFINITION(ObjectAddedEvent, WorldEvent);

	ObjectAddedEvent::ObjectAddedEvent(WorldObject* aObject) : WorldEvent(aObject ? aObject->GetWorld() : nullptr)
		, object(aObject)
	{}

	TYPE_DEFINITION(ObjectRemovedEvent, WorldEvent);

	ObjectRemovedEvent::ObjectRemovedEvent(WorldObject* aObject) : WorldEvent(aObject ? aObject->GetWorld() : nullptr)
		, object(aObject)
	{}

	TYPE_DEFINITION(WorldObjectEvent, WorldEvent);

	WorldObjectEvent::WorldObjectEvent(WorldObject* aObject) : WorldEvent(aObject ? aObject->GetWorld() : nullptr)
		, object(aObject)
	{}

	TYPE_DEFINITION(ObjectCreatedEvent, WorldObjectEvent);

	TYPE_DEFINITION(ObjectDestroyedEvent, WorldObjectEvent);

	TYPE_DEFINITION(ObjectEnabledEvent, WorldObjectEvent);

	TYPE_DEFINITION(ObjectDisabledEvent, WorldObjectEvent);
}
