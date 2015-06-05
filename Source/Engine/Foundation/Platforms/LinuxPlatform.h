#pragma once
#ifndef _VANGUARD_PLATFORM_IMPLEMENTATION
#define _VANGUARD_PLATFORM_IMPLEMENTATION

#include "VanguardString.h"
#include "BasePlatform.h"

// Expands to nothing because not needed on this platform.
#define API_EXPORT

namespace Vanguard
{
	struct LinuxPlatform : public BasePlatform
	{
		static inline const char* DynamicLibExtension(){ return ".so"; }
	};
	typedef LinuxPlatform Platform;
}

#endif //_VANGUARD_PLATFORM_IMPLEMENTATION