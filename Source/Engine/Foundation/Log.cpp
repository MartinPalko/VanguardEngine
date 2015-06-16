#include "Log.h"
#include <mutex>
#include <thread>

namespace Vanguard
{
	std::mutex Log::writeMutex;
}