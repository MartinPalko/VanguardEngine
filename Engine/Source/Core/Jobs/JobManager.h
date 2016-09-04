#pragma once
#include <queue>
#include <mutex>

#include "Foundation.h"
#include "JobEnums.h"

namespace Vanguard
{
	class Job;
	class JobWorker;
	class JobProfiler;
	class Frame;

	class JobManager
	{
		friend JobWorker;
		friend Frame;

	private:
		DynamicArray<JobWorker*> workers;
		DynamicArray<JobWorker*> idleWorkers;
		std::mutex idleWorkersMutex;

		std::queue<Job*> jobs;
		size_t queuedJobs;
		std::mutex jobQueueMutex;

		JobWorker* GetIdleWorker();
		size_t GetIdleWorkers();
		JobWorker* GetWorker();

		void WorkerFinishedJob(JobWorker* aThread, Job* aJob);
		Job* GetNextJob();

		JobProfiler* profiler;

	public:
		JobManager();
		~JobManager();

		void JoinThreads();
		void AddJob(Job* aJob);

		JobProfiler* GetProfiler() { return profiler; }
	};
}