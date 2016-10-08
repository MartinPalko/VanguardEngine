#include "Utility.h"

#include "Platforms/Platform.h"

#include <limits>

namespace Vanguard
{
	int32 MinInt32()
	{
		return std::numeric_limits<int32>::lowest();
	}

	int32 MaxInt32()
	{
		return std::numeric_limits<int32>::max();
	}

	float MinFloat()
	{
		return std::numeric_limits<float>::lowest();
	}

	float SmallestFloat()
	{
		return std::numeric_limits<float>::min();
	}

	float MaxFloat()
	{
		return std::numeric_limits<float>::max();
	}
}