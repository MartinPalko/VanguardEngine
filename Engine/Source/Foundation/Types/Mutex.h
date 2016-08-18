#pragma once
#include "Foundation_Common.h"

namespace std { class mutex; }

namespace Vanguard
{
	class FOUNDATION_API Mutex
	{
	private:
		class std::mutex* stdMutex;
	public:
		Mutex();
		~Mutex();

		void Lock();
		void Unlock();
	};
}