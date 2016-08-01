#pragma once
#include "Foundation.h"
#include "JobEnums.h"
#include <queue>
#include <mutex>

namespace Vanguard
{
	class Job;
	class JobWorker;
	class Frame;

	class JobManager
	{
		friend JobWorker;
		friend Frame;

	private:
		static Frame* currentFrame;

		static Mutex threadMutex;

		static DynamicArray<JobWorker*> workers;
		static std::queue<JobWorker*> idleThreads;

		static JobWorker* GetIdleThread();

		static void ThreadFinishedJob(JobWorker* aThread, Job* aJob);

	public:
		static void CreateThreads();

		static void ProcessFrame(Frame* aFrame);

		static JobWorker* GetWorker();
	};
}