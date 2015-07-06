#pragma once
#include "Log.h"
#include "JobEnums.h"

namespace Vanguard
{
	class Frame;
	class Job
	{
		friend class Frame;
	private:
		bool running;
		bool finished;

		std::function<void()> entryPoint;

	public:
		Job(std::function<void()> aEntryPoint)
		{
			running = false;
			finished = false;
			entryPoint = aEntryPoint;
		}

		void Execute()
		{
			DEBUG_LOG("Executing Job");
			running = true;
			entryPoint();
			running = false;
			finished = true;
			DEBUG_LOG("Job Executed");
		}
	};
}