#pragma once
#include "juce_core.h"
#include <thread>
namespace Vanguard
{
	class Thread
	{
	public:
		std::thread stdThread;

		Thread()
		{
			//stdThread = std::thread()
		}

	};
}
