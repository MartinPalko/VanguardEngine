#include "EditorEvents.h"

namespace Vanguard
{
	ABSTRACT_TYPE_DEFINITION(EditorEvent, Event);

	TYPE_DEFINITION(EntitySelectedEvent, EditorEvent);

	TYPE_DEFINITION(EntityUnselectedEvent, EditorEvent);
}
