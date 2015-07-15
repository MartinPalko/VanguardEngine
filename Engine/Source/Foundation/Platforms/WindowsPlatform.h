#pragma once
#ifndef _VANGUARD_PLATFORM_IMPLEMENTATION
#define _VANGUARD_PLATFORM_IMPLEMENTATION

#include "BasePlatform.h"

namespace Vanguard
{
	struct WindowsPlatform : public BasePlatform
	{
		typedef __int64 int64;
		typedef __int32 int32;
		typedef __int16 int16;
		typedef __int8 int8;

		typedef unsigned __int64 uint64;
		typedef unsigned __int32 uint32;
		typedef unsigned __int16 uint16;
		typedef unsigned __int8 uint8;

		static inline const char* DynamicLibExtension(){ return ".dll"; }

		typedef std::exception Exception;
	};
	typedef WindowsPlatform Platform;
}
#endif //_VANGUARD_PLATFORM_IMPLEMENTATION
