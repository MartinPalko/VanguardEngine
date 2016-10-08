#pragma once

#include "FileSystem.h"
#include "MathFunctions.h"

namespace Vanguard
{
	// Finds the offset of a member variable
	template<typename T, typename U> constexpr size_t offsetOf(U T::*member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}

	FOUNDATION_API int32 MinInt32();
	FOUNDATION_API int32 MaxInt32();
	FOUNDATION_API float MinFloat();
	FOUNDATION_API float SmallestFloat();
	FOUNDATION_API float MaxFloat();
}