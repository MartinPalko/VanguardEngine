#include "Events.h"

namespace Vanguard
{
	ABSTRACT_BASETYPE_DEFINITION(Event);

	ABSTRACT_TYPE_DEFINITION(WorldEvent, Event);

	TYPE_DEFINITION(VanguardObjectEvent, WorldEvent);

	TYPE_DEFINITION(ObjectCreatedEvent, VanguardObjectEvent);
	TYPE_DEFINITION(ObjectDestroyedEvent, VanguardObjectEvent);

	TYPE_DEFINITION(ObjectEnabledEvent, VanguardObjectEvent);
	TYPE_DEFINITION(ObjectDisabledEvent, VanguardObjectEvent);
}