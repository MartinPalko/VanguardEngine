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
}