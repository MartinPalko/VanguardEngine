#pragma once
#include <mutex>

namespace Vanguard
{
	class Mutex
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