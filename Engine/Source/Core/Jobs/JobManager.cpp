#include "JobManager.h"
#include "JobWorker.h"
#include "Job.h"
#include "Core.h"

namespace Vanguard
{
	JobWorker* JobManager::GetIdleWorker()
	{
		std::lock_guard<std::mutex> lock(idleWorkersMutex);
		JobWorker* idleThread = nullptr;
		if (idleWorkers.Count() > 0)
		{
			idleThread = idleWorkers.Back();
			idleWorkers.PopBack();
		}
		return idleThread;
	}

	size_t JobManager::GetIdleWorkers()
	{
		std::lock_guard<std::mutex> lock(idleWorkersMutex);
		return idleWorkers.Count();
	}

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
		
		Job* nextJob = GetNextJob();
		if (nextJob)
		{
			aThread->StartJob(nextJob);
		}
		else
		{
			std::lock_guard<std::mutex> lock(idleWorkersMutex);
			idleWorkers.PushBack(aThread);
		}
	}

	Job* JobManager::GetNextJob()
	{
		std::lock_guard<std::mutex> lock(jobQueueMutex);
		Job* nextJob = nullptr;
		if (queuedJobs > 0)
		{
			nextJob = jobs.front();
			jobs.pop();
			queuedJobs--;
		}
		return nextJob;
	}

	JobManager::JobManager()
		: workers()
		, idleWorkers()
		, idleWorkersMutex()
		, jobs()
		, queuedJobs(0)
		, jobQueueMutex()
	{
		size_t targetThreads = SystemInfo::GetNumberOfCores();

		if (targetThreads > 1)
		{
			Log::Message("Creating " + String::FromSize(targetThreads) + " job threads", "JobSystem");

			size_t threadsToCreate = targetThreads - workers.Count();

			for (int i = 0; i < threadsToCreate; i++)
			{
				JobWorker* newJobWorker = new JobWorker(i, this);
				newJobWorker->SetAffinityMask(1 << i);
				workers.PushBack(newJobWorker);
				idleWorkers.PushBack(newJobWorker);
			}
		}
		else
		{
			Log::Message("Job Manager running in synchronouse mode", "JobSystem");
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

	void JobManager::AddJob(Job* aJob)
	{
		if (workers.Count())
		{
			JobWorker* idleWorker = GetIdleWorker();
			if (idleWorker)
			{
				// Assign the job to an idle worker right away.
				idleWorker->StartJob(aJob);
			}
			else
			{
				// Put it to the queue.
				std::lock_guard<std::mutex> lock(jobQueueMutex);
				queuedJobs++;
				jobs.push(aJob);
			}			
		}
		else
		{
			// No job threads, execute job synchronously.
			aJob->Execute();
			delete aJob;
		}
	}





		}

	}
}