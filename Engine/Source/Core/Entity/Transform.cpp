#include "Transform.h"

namespace Vanguard
{
	TYPE_DEFINITION(Transform, Component);

	START_REGISTER_PROPERTIES(Transform)
	{
		REGISTER_PROPERTY(Transform, position)
		REGISTER_PROPERTY(Transform, rotation)
	}
	FINISH_REGISTER_PROPERTIES(Transform)
}