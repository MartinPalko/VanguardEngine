#include "JobManager.h"
#include "JobThread.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	Mutex JobManager::threadMutex;
	Frame* JobManager::currentFrame;

	List<JobThread*> JobManager::jobThreads = List<JobThread*>();
	std::queue<JobThread*> JobManager::idleThreads = std::queue<JobThread*>();

	Mutex jobListMutex;

	JobThread* JobManager::GetIdleThread()
	{
		if (idleThreads.size() > 0)
		{
			JobThread* idleThread = idleThreads.front();
			idleThreads.pop();
			return idleThread;
		}
		return nullptr;
	}

	void JobManager::ThreadFinishedJob(JobThread* aThread, Job* aJob)
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

		int32 threadsToCreate = targetThreads - jobThreads.Size();

		for (int i = 0; i < threadsToCreate; i++)
		{
			JobThread* newJobThread = new JobThread();
			jobThreads.PushBack(newJobThread);
			idleThreads.push(newJobThread);
		}
	}

	void JobManager::ProcessFrame(Frame* aFrame)
	{		
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
		while (idleThreads.size() != jobThreads.Size())
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		currentFrame->processing = false;
	}

	JobThread* JobManager::GetJobThread()
	{
		std::thread::id thisThreadID = std::this_thread::get_id();

		for (uint32 i = 0; i < jobThreads.Size(); i++)
		{
			if (jobThreads[i]->stdthread.get_id() == thisThreadID)
				return jobThreads[i];
		}
		return nullptr;
	}

}