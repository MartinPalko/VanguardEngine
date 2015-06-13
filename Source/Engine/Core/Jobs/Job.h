#pragma once
#include "JobEnums.h"

namespace Vanguard
{
	typedef void(*voidFunction)(void);

	class Job
	{
	private:
		JobPriority priority;
		bool running;

		voidFunction entryPoint;
	public:
		Job(voidFunction aEntryPoint, JobPriority aPriority = JobPriority::Normal)
		{
			priority = aPriority;
			entryPoint = aEntryPoint;
		}

		void Execute()
		{
			entryPoint();
		}

		JobPriority GetPriority(){ return priority; }
	};
}