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
		String name;
		bool running;
		bool finished;

		std::function<void()> entryPoint;

	public:
		Job(String aName, std::function<void()> aEntryPoint)
		{
			name = aName;
			running = false;
			finished = false;
			entryPoint = aEntryPoint;
		}

		String GetName() { return name; }

		void Execute()
		{
			running = true;
			entryPoint();
			running = false;
			finished = true;
		}
	};
}