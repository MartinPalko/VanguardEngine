#include "Frame.h"
#include "JobManager.h"
#include "JobWorker.h"


namespace Vanguard
{
	Job* Frame::AddJob(std::function<void()> aEntryPoint, JobPriority::Type aPriority)
	{
		// If any threads are idle, throw this job on one of them right away.
		JobManager::threadMutex.Lock();
		Job* newJob = new Job(aEntryPoint);
		JobWorker* idleThread = JobManager::GetIdleThread();
		if (idleThread == nullptr)
		{
			jobListMutex.Lock();
			jobs[(uint8)aPriority].push(newJob);
			jobListMutex.Unlock();
		}
		else
		{
			idleThread->StartJob(newJob);
		}
		JobManager::threadMutex.Unlock();
		return newJob;		
	}

	Job* Frame::GetNextJob()
	{
		jobListMutex.Lock();
		for (int i = (uint8)JobPriority::qty - 1; i >= 0; i--)
		{
			if (jobs[i].size() > 0)
			{
				Job* nextJob = jobs[i].front();
				jobs[i].pop();
				jobListMutex.Unlock();
				return nextJob;
			}
		}
		jobListMutex.Unlock();
		return nullptr;
	}

	void Frame::WaitForJob(Job* aJob)
	{
		while (!aJob->finished)
		{
			// While we wait for the job to finish, try to execute other jobs.
			Job* nextJob = GetNextJob();
			if (nextJob != nullptr)
			{
				nextJob->Execute();
			}

		}
	}
}