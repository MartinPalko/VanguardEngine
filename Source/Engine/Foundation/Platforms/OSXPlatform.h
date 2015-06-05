#pragma once
#ifndef _VANGUARD_PLATFORM_IMPLEMENTATION
#define _VANGUARD_PLATFORM_IMPLEMENTATION

#include "BasePlatform.h"
#include "VanguardString.h"

// Expands to nothing because not needed on this platform.
#define API_EXPORT

namespace Vanguard
{
	struct OSXPlatform : public BasePlatform
	{
		static inline const char* DynamicLibExtension(){ return ".so"; }
	};
	typedef OSXPlatform Platform;
}

#endif //_VANGUARD_PLATFORM_IMPLEMENTATION