#pragma once
#include "VanguardString.h"
#include "Mutex.h"

namespace Vanguard
{
	class Log
	{
	public:
		static std::mutex writeMutex;

		inline static void Write(const String& aMessage)
		{
			writeMutex.lock();
			std::cout << aMessage.GetCharPointer() << "\n";
			writeMutex.unlock();
		}
	};
}