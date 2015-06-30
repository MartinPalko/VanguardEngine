#pragma once
#include "Core_Common.h"
#include "VanguardString.h"
#include "Mutex.h"

namespace Vanguard
{
	class CORE_API Log
	{
	private:
		static Mutex writeMutex;

	public:
		inline static void Write(const String& aMessage)
		{
			writeMutex.Lock();
			std::cout << aMessage.GetCharPointer() << "\n";
			writeMutex.Unlock();
		}
	};
}