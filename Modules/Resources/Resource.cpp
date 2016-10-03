#include "Resource.h"

namespace Vanguard
{
	ABSTRACT_BASETYPE_DEFINITION(Resource);

	Resource::Resource()
		: resourcePath()
		, id(UUID::CreateNew())
	{
	}

	Resource::~Resource()
	{
		//if (IsLoaded())
		//	UnloadResource();
	}
}