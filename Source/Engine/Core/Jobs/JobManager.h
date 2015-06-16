#pragma once
#include "Foundation.h"
#include "JobEnums.h"
#include <queue>
#include <mutex>

namespace Vanguard
{
	class Job;
	class JobThread;
	class Frame;

	class JobManager
	{
		friend JobThread;
		friend Frame;

	private:
		static Frame* currentFrame;

		static Mutex threadMutex;

		static List<JobThread*> jobThreads;
		static std::queue<JobThread*> idleThreads;

		static JobThread* GetIdleThread();

		static void ThreadFinishedJob(JobThread* aThread, Job* aJob);

	public:
		static void CreateThreads();

		static void ProcessFrame(Frame* aFrame);

		static JobThread* GetJobThread();
	};
}