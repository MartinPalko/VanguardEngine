#pragma once
#include "Foundation.h"

namespace Vanguard
{
	// A class that provides a simple interface to convert string pointers coming from the managed side to native string instances.
	class ManagedString
	{
		ManagedString() = delete; // No Constructor, instance has to come from managed code.

	public:
		// Create a native string from managed string.
		operator String() { return ToNative(); }

		String ToNative();

	};
}