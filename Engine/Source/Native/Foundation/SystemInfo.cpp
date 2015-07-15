#include "SystemInfo.h"
#include "juce_core.h"

namespace Vanguard
{
	uint32 SystemInfo::GetNumberOfCores()
	{
		return juce::SystemStats::getNumCpus();
	}
}