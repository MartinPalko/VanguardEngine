#pragma once
#include "Foundation_Common.h"
#include "Types/IntegerDefs.h"

namespace Vanguard
{
	class FOUNDATION_API SystemInfo
	{
	public:
		static uint32 GetNumberOfCores();
	};
}