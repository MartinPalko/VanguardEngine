#include <mutex>
#include "Mutex.h"

namespace Vanguard
{
	Mutex::Mutex()
	{
		stdMutex = new std::mutex();
	}

	Mutex::~Mutex()
	{
		delete stdMutex;
	}

	void Mutex::Lock()
	{
		stdMutex->lock();
	}

	void Mutex::Unlock()
	{
		stdMutex->unlock();
	}
}