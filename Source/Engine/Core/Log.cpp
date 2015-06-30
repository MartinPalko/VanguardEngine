#include "Log.h"

namespace Vanguard
{
	Mutex Log::writeMutex;

	void Log::Write(const String& aMessage)
	{
		writeMutex.Lock();
		std::cout << aMessage.GetCharPointer() << "\n";
		writeMutex.Unlock();
	}
}