#pragma once
#ifndef _VANGUARD_PLATFORM_IMPLEMENTATION
#define _VANGUARD_PLATFORM_IMPLEMENTATION
#include "BasePlatform.h"

namespace Vanguard
{
	struct WindowsPlatform : public BasePlatform
	{
		static inline const char* DynamicLibExtension(){ return ".dll"; }
	};
	typedef WindowsPlatform Platform;
}
#endif //_VANGUARD_PLATFORM_IMPLEMENTATION