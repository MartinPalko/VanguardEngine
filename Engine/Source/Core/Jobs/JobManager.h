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

		Frame* currentFrame;

		Mutex threadMutex;

		DynamicArray<JobWorker*> workers;
		std::queue<JobWorker*> idleWorkers;
		Mutex idleWorkersMutex;

		Mutex jobListMutex;

		JobWorker* GetIdleWorker();
		size_t GetIdleWorkers();
		JobWorker* GetWorker();

		void WorkerFinishedJob(JobWorker* aThread, Job* aJob);
		void KickoffJobs();

	public:
		JobManager();
		~JobManager();

		void JoinThreads();
		void ProcessFrame(Frame* aFrame);
	};
}