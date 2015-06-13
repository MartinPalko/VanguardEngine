#pragma once
#include "Foundation.h"
#include "JobEnums.h"
#include <queue>

namespace Vanguard
{
	class Job;
	class JobThread;
	class Frame;

	class JobManager
	{
		friend JobThread;

	protected:
		static std::queue<Frame*> queuedFrames;

		static List<JobThread*> jobThreads;
		static std::queue<JobThread*> idleThreads;

		static JobThread* GetIdleThread();
		static Job* GetNextJob();

		static void ThreadFinishedJob(JobThread* aThread, Job* aJob);

	public:
		static void CreateThreads();

		static void ProcessFrame(Frame* aFrame);
	};
}