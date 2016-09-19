#include "Resource.h"

namespace Vanguard
{
	ABSTRACT_BASETYPE_DEFINITION(Resource);

	Resource::~Resource()
	{
		//if (IsLoaded())
		//	UnloadResource();
	}
}