#include "JobManager.h"
#include "JobWorker.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	JobWorker* JobManager::GetIdleWorker()
	{
		JobWorker* idleThread = nullptr;
		idleWorkersMutex.Lock();
		if (idleWorkers.size() > 0)
		{
			idleThread = idleWorkers.front();
			idleWorkers.pop();
		}
		idleWorkersMutex.Unlock();
		return idleThread;
	}

	uint8 JobManager::GetIdleWorkers()
	{
		idleWorkersMutex.Lock();
		const uint8 retval = idleWorkers.size();
		idleWorkersMutex.Unlock();
		return retval;
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
		
		Job* nextJob = currentFrame->GetNextJob();
		if (nextJob)
		{
			aThread->StartJob(nextJob);
		}
		else
		{
			idleWorkersMutex.Lock();
			idleWorkers.push(aThread);
			idleWorkersMutex.Unlock();
		}
	}

	void JobManager::KickoffJobs()
	{
		while (true)
		{
			JobWorker* worker = nullptr;
			Job* nextJob = nullptr;
			idleWorkersMutex.Lock();
			if (idleWorkers.size() > 0)
			{
				nextJob = currentFrame->GetNextJob();
				if (nextJob)
				{
					worker = idleWorkers.front();
					idleWorkers.pop();
				}
			}
			idleWorkersMutex.Unlock();

			if (!worker)
			{
				break;
			}

			worker->StartJob(nextJob);
		}
	}

	JobManager::JobManager()
	{
		int32 targetThreads = SystemInfo::GetNumberOfCores();

		if (targetThreads > 1)
		{
			Log::Message("Creating " + String::FromInt32(targetThreads) + " job threads", "JobSystem");

			int32 threadsToCreate = targetThreads - workers.Count();

			for (int i = 0; i < threadsToCreate; i++)
			{
				JobWorker* newJobWorker = new JobWorker(i, this);
				workers.PushBack(newJobWorker);
				idleWorkers.push(newJobWorker);
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

	void JobManager::ProcessFrame(Frame* aFrame)
	{
		DEBUG_LOG("Processing Frame " + String::FromInt32(aFrame->frameNumber))

		currentFrame = aFrame;
		currentFrame->processing = true;


		if (workers.Count())
		{
			KickoffJobs();

			std::function<void()> jobAddedCallbackFunc = std::bind(&JobManager::KickoffJobs, this);

			aFrame->RegisterJobAddedCallback(&jobAddedCallbackFunc);

			// Wait until all jobs have been consumed.
			while (!aFrame->JobsFinished())
				std::this_thread::sleep_for(std::chrono::microseconds(1));

			// And again until all threads have become idle.
			while (GetIdleWorkers() != workers.Count())
				std::this_thread::sleep_for(std::chrono::microseconds(1));

			// TODO: Make sure this actually works.
			aFrame->UnregisterJobAddedCallback(&jobAddedCallbackFunc);
		}
		else
		{
			// No job threads, execute jobs synchronously.
			Job* nextJob = currentFrame->GetNextJob();
			while (nextJob)
			{
				nextJob->Execute();
				nextJob = currentFrame->GetNextJob();
			}
		}

		currentFrame->processing = false;
	}
}