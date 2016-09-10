#include "JobManager.h"
#include "JobWorker.h"
#include "Job.h"
#include "Core.h"
#include "JobProfiler.h"

namespace Vanguard
{
	JobWorker* JobManager::GetWorker()
	{
		const String currentThreadID = Thread::CurrentThreadID();

		for (uint32 i = 0; i < workers.Count(); i++)
		{
			if (workers[i]->GetID() == currentThreadID)
				return workers[i];
		}
		return nullptr;
	}

	void JobManager::WorkerFinishedJob(JobWorker* aThread, Job* aJob)
	{
		delete aJob;
	}

	JobManager::JobManager()
		: workers()
		, jobs()
	{

#ifdef JOB_PROFILING
		profiler = new JobProfiler();
#else
		profiler = nullptr;
#endif

#ifdef VANGUARD_LINUX
		size_t targetThreads = 0; // Job system not currently working properly on linux.
#else
		size_t targetThreads = SystemInfo::GetNumberOfCores();
#endif

		if (targetThreads > 1)
		{
			Log::Message("Creating " + String::FromSize(targetThreads) + " job threads", "JobSystem");

			size_t threadsToCreate = targetThreads - workers.Count();

			for (int i = 0; i < threadsToCreate; i++)
			{
				JobWorker* newJobWorker = new JobWorker(i, this);
				newJobWorker->SetAffinityMask(1 << i);
				workers.PushBack(newJobWorker);
				newJobWorker->Start();
			}
		}
		else
		{
			Log::Message("Job Manager running in synchronouse mode", "JobSystem");
		}
	}

	JobManager::~JobManager()
	{
		if (profiler)
		{
			delete profiler;
			profiler = nullptr;
		}

		JoinThreads();
		for (int i = 0; i < workers.Count(); i++)
		{
			delete workers[i];
		}
	}

	void JobManager::JoinThreads()
	{
		for (int i = 0; i < workers.Count(); i++)
		{
			workers[i]->Join();
			delete workers[i];
		}
		workers.Clear();
	}

	void JobManager::ServiceMainThreadJobs()
	{
		ASSERT_MAIN_THREAD;

		Job* job;
		while (mainThreadJobs.try_dequeue(job))
		{
			job->Execute();
		}
	}

	void JobManager::AddJob(Job* aJob)
	{
		if (workers.Count())
		{
			// Put it to the queue.
			if (aJob->requiresMainThreadExecution)
			{
				mainThreadJobs.enqueue(aJob);
			}
			else
			{
				jobs.enqueue(aJob);
			}
		}
		else
		{
			// No job threads, execute job synchronously.
			aJob->Execute();
			delete aJob;
		}
	}

	void JobManager::AddJobs(Job** aJobs, size_t aNumJobs)
	{
		if (workers.Count())
		{
			// Put them to the queue.
			jobs.enqueue_bulk(aJobs, aNumJobs);
		}
		else
		{
			// No job threads, execute job synchronously.
			for (size_t i = 0; i < aNumJobs; i++)
			{
				aJobs[i]->Execute();
				delete aJobs[i];
			}
		}
	}
}