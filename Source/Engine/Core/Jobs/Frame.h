#pragma once
#include "Foundation.h"
#include "JobEnums.h"
#include "Job.h"
#include <queue>

namespace Vanguard
{
	class World;
	class Job;
	class Frame
	{
		friend class JobManager;
	public:
		int frameNumber;
		float deltaTime;
		World* world;

		std::queue<Job*> jobs[JobPriority::qty];

	public:
		Frame(int aFrameNumber, float aDeltaTime, World* aWorld)
		{
			frameNumber = aFrameNumber;
			deltaTime = aDeltaTime;
			world = aWorld;
		}

		void AddJob(jobEntryPoint aEntryPoint, JobPriority::Type aPriority = JobPriority::Normal);

		bool JobsFinished()
		{
			// TODO: This will return true if the last few jobs are running (removed from the jobs list) but not yet finished. Need to figure out how to fix that.
			for (int i = 0; i < JobPriority::qty; i++)
			{
				if (jobs[i].size() > 0)
					return false;
			}
			return true;
		}
	};
}