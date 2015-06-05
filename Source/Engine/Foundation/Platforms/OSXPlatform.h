#pragma once
#ifndef _VANGUARD_PLATFORM_IMPLEMENTATION
#define _VANGUARD_PLATFORM_IMPLEMENTATION

#include "BasePlatform.h"

// Expands to nothing because not needed on this platform.
#define API_EXPORT

namespace Vanguard
{
	struct OSXPlatform : public BasePlatform
	{

	};
	typedef OSXPlatform Platform;
}

#endif //_VANGUARD_PLATFORM_IMPLEMENTATION