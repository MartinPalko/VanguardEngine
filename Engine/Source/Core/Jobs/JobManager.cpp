#include "JobManager.h"
#include "JobWorker.h"
#include "Job.h"
#include "Core.h"

#define MAX_JOB_WORKERS 32u

namespace Vanguard
{
	void JobManager::WorkerFinishedJob(JobWorker* aThread, Job* aJob)
	{
		delete aJob;
	}

	JobManager::JobManager()
		: workers()
		, jobs()
	{
		// One less worker than the number of cores on the system (since the main thread will help out)
		const size_t targetThreads = Math::Min(SystemInfo::GetNumberOfCores() - 1, MAX_JOB_WORKERS);

		if (!targetThreads)
		{
			LOG_MESSAGE("Job Manager running in synchronouse mode", "JobSystem");
		}
		else
		{
			LOG_MESSAGE("Creating " + String::FromSize(targetThreads) + " job threads", "JobSystem");
		}

		for (int i = 0; i < targetThreads; i++)
		{
			JobWorker* newJobWorker = new JobWorker(i, this);
			//newJobWorker->SetAffinityMask(1 << i); // Disabled setting affinity for now. Need to investigate the implications.
			workers.PushBack(newJobWorker);
			newJobWorker->Start();
		}
	}

	JobManager::~JobManager()
	{
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
			delete job;
		}
	}

	void JobManager::HelpWithJob()
	{
		Job* job;
		if (jobs.try_dequeue(job))
		{
			job->Execute();
			delete job;
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

	JobWorker* JobManager::GetWorkerByThreadID(size_t aThreadID)
	{
		for (uint32 i = 0; i < workers.Count(); i++)
		{
			if (workers[i]->GetID() == aThreadID)
				return workers[i];
		}
		return nullptr;
	}
}