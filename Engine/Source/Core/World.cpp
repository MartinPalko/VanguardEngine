#include "World.h"

#include "Jobs/Frame.h"

namespace Vanguard
{
	void World::Tick(Frame* aFrame)
	{
		DynamicArray<Job*> jobs;

		// Dispatch all ticks to the job system.
		for (size_t i = 0; i < registeredTicks.Count(); i++)
		{
			std::function<void(Frame*)> tickFunction = registeredTicks[i];
			jobs.PushBack(aFrame->AddJob([tickFunction, aFrame]()-> void {tickFunction(aFrame); }));
		}

		// Wait for all jobs to complete.
		for (size_t i = 0; i < jobs.Count(); i++)
		{
			aFrame->WaitForJob(jobs[i]);
		}
	}
}
