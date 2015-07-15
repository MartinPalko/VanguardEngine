#pragma once
#include <mutex>
#include "Foundation_Common.h"

namespace Vanguard
{
	class FOUNDATION_API Mutex
	{
	private:
		std::mutex stdMutex;
	public:
		Mutex() :stdMutex() {}

		void Lock()
		{
			stdMutex.lock();
		}

		void Unlock()
		{
			stdMutex.unlock();
		}
	};
}