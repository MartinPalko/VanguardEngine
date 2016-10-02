#include "Events.h"

namespace Vanguard
{
	ABSTRACT_BASETYPE_DEFINITION(Event);

	ABSTRACT_TYPE_DEFINITION(WorldEvent, Event);

	TYPE_DEFINITION(WorldObjectEvent, WorldEvent);

	TYPE_DEFINITION(ObjectCreatedEvent, WorldObjectEvent);
	TYPE_DEFINITION(ObjectDestroyedEvent, WorldObjectEvent);

	TYPE_DEFINITION(ObjectEnabledEvent, WorldObjectEvent);
	TYPE_DEFINITION(ObjectDisabledEvent, WorldObjectEvent);
}