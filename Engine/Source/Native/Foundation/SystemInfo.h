#pragma once
#include "Types/IntegerDefs.h"
#include "juce_core.h"

namespace Vanguard
{
	class SystemInfo
	{
	public:
		static uint32 GetNumberOfCores()
		{
			return juce::SystemStats::getNumCpus();
		}
	};
}