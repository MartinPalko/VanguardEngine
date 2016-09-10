#pragma once
#include <queue>
#include <mutex>

#include "Foundation.h"
#include "JobEnums.h"
#include "ThirdParty/ConcurrentQueue/blockingconcurrentqueue.h"

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
		moodycamel::BlockingConcurrentQueue<Job*> jobs;
		moodycamel::BlockingConcurrentQueue<Job*> mainThreadJobs;

		JobWorker* GetWorker();

		void WorkerFinishedJob(JobWorker* aThread, Job* aJob);

		JobProfiler* profiler;

	public:
		JobManager();
		~JobManager();

		void JoinThreads();
		void ServiceMainThreadJobs();
		void AddJob(Job* aJob);
		void AddJobs(Job** aJobs, size_t aNumJobs);

		JobProfiler* GetProfiler() { return profiler; }
	};
}