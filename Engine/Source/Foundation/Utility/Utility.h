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

	FOUNDATION_API float LowestFloat();
	FOUNDATION_API float MinFloat();
	FOUNDATION_API float MaxFloat();
}