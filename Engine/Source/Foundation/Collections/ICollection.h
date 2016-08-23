#pragma once

#include "Platforms/Platform.h"

namespace Vanguard
{
	template <class T>
    class ICollection
	{
		// TODO: Pure virtual, common enumeration functionality.
    public:
        virtual size_t Count() const = 0;
	};
}