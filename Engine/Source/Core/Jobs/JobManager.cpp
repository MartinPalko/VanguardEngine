#include "JobManager.h"
#include "JobWorker.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	Mutex JobManager::threadMutex;
	Frame* JobManager::currentFrame;

	DynamicArray<JobWorker*> JobManager::workers = DynamicArray<JobWorker*>();
	std::queue<JobWorker*> JobManager::idleThreads = std::queue<JobWorker*>();

	Mutex jobListMutex;

	JobWorker* JobManager::GetIdleThread()
	{
		if (idleThreads.size() > 0)
		{
			JobWorker* idleThread = idleThreads.front();
			idleThreads.pop();
			return idleThread;
		}
		return nullptr;
	}

	void JobManager::ThreadFinishedJob(JobWorker* aThread, Job* aJob)
	{
		delete aJob;
		
		Job* nextJob = currentFrame->GetNextJob();
		if (nextJob)
		{
			aThread->StartJob(nextJob);
		}
		else
		{
			idleThreads.push(aThread);
		}
	}

	void JobManager::CreateThreads()
	{
		int32 targetThreads = SystemInfo::GetNumberOfCores();

		Log::Message("Creating " + String::FromInt32(targetThreads) + " job threads", "JobSystem");

		int32 threadsToCreate = targetThreads - workers.Count();

		for (int i = 0; i < threadsToCreate; i++)
		{
			JobWorker* newJobWorker = new JobWorker(i);
			workers.PushBack(newJobWorker);
			idleThreads.push(newJobWorker);
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
		if (!workers.Count())
		{
			CreateThreads();
		}

		DEBUG_LOG("Processing Frame " + String::FromInt32(aFrame->frameNumber))

		currentFrame = aFrame;
		currentFrame->processing = true;

		Job* nextJob = currentFrame->GetNextJob();
		while (nextJob != nullptr && idleThreads.size() > 0)
		{
			GetIdleThread()->StartJob(nextJob);
			nextJob = currentFrame->GetNextJob();
		}

		// Wait until all jobs have been consumed.
		while (!aFrame->JobsFinished())
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		// And again until all threads have become idle.
		while (idleThreads.size() != workers.Count())
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		currentFrame->processing = false;
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

}