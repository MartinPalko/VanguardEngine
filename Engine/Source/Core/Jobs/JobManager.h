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

		void WorkerFinishedJob(JobWorker* aThread, Job* aJob);

	public:
		JobManager();
		~JobManager();

		void JoinThreads();
		void ServiceMainThreadJobs();
		void HelpWithJob();
		void AddJob(Job* aJob);
		void AddJobs(Job** aJobs, size_t aNumJobs);

		JobWorker* GetWorkerByThreadID(size_t aThreadID);
	};
}