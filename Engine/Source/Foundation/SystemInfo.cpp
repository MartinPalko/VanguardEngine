#include <thread>
#include "SystemInfo.h"

namespace Vanguard
{
	uint32 SystemInfo::GetNumberOfCores()
	{
		return std::thread::hardware_concurrency();
	}
}
