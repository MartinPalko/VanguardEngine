#include "juce_core.h"
#include "SystemInfo.h"

namespace Vanguard
{
	uint32 SystemInfo::GetNumberOfCores()
	{
		return juce::SystemStats::getNumCpus();
	}
}
