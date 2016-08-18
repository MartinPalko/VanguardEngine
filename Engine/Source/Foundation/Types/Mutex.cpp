#pragma once
#include "Mutex.h"
#include <mutex>

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