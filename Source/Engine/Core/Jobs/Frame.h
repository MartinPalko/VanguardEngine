#pragma once
#include "Foundation.h"
#include "Core_Common.h"
#include "JobEnums.h"
#include "Job.h"
#include <queue>
#include <mutex>

namespace Vanguard
{
	class World;
	class Job;
	class CORE_API Frame
	{
		friend class JobManager;
	private:
		bool processing;
		Mutex jobListMutex;
		Job* GetNextJob();

	public:
		int frameNumber;
		float deltaTime;
		World* world;

		std::queue<Job*> jobs[JobPriority::qty];

		void WaitForJob(Job* aJob);

		Frame(int aFrameNumber, float aDeltaTime, World* aWorld)
		{
			processing = false;
			frameNumber = aFrameNumber;
			deltaTime = aDeltaTime;
			world = aWorld;
		}

		Job* AddJob(std::function<void()> aEntryPoint, JobPriority::Type aPriority = JobPriority::Normal);
		

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