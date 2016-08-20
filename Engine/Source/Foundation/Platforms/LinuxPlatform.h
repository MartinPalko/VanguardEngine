#pragma once
#ifndef _VANGUARD_PLATFORM_IMPLEMENTATION
#define _VANGUARD_PLATFORM_IMPLEMENTATION

#include <inttypes.h>
#include <stdexcept>

#include "BasePlatform.h"

// Expands to nothing because not needed on this platform.
#define API_EXPORT

namespace Vanguard
{
	struct LinuxPlatform : public BasePlatform
	{
        typedef int64_t int64;
		typedef int32_t int32;
		typedef int16_t int16;
		typedef int8_t int8;

		typedef uint64_t uint64;
		typedef uint32_t uint32;
		typedef uint16_t uint16;
		typedef uint8_t uint8;

		static inline const char* DynamicLibExtension(){ return ".so"; }

		typedef std::runtime_error Exception;

		// TODO
		typedef void* WindowHandle;
		typedef void* NativeMessage;
	};
	typedef LinuxPlatform Platform;


}

#endif //_VANGUARD_PLATFORM_IMPLEMENTATION
