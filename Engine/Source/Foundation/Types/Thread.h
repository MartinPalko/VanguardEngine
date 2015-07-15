#pragma once
#include "juce_core.h"
#include <thread>
#include "Foundation_Common.h"

namespace Vanguard
{
	class FOUNDATION_API Thread
	{
	public:
		std::thread stdThread;

		Thread()
		{
		}
	};
}
