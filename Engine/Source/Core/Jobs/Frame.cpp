#include "Frame.h"
#include "JobManager.h"
#include "JobWorker.h"
#include "Core.h"


namespace Vanguard
{
	Job* Frame::AddJob(std::function<void()> aEntryPoint, JobPriority::Type aPriority)
	{
		jobListMutex.Lock();
		unfinishedJobs[(uint8)aPriority]++;
		Job* newJob = new Job([aEntryPoint, this, aPriority]()
		{
			aEntryPoint(); 
			jobListMutex.Lock();
			this->unfinishedJobs[(uint8)aPriority]--;
			jobListMutex.Unlock();

		});
		jobs[(uint8)aPriority].push(newJob);
		queuedJobs[(uint8)aPriority]++;
		jobListMutex.Unlock();
		
		NotifyJobAdded();

		return newJob;
	}

	Job* Frame::GetNextJob()
	{
		jobListMutex.Lock();
		Job* nextJob = nullptr;
		for (int i = (uint8)JobPriority::qty - 1; i >= 0; i--)
		{
			if (queuedJobs[i] > 0)
			{
				nextJob = jobs[i].front();
				jobs[i].pop();
				queuedJobs[i]--;
				break;
			}
		}
		jobListMutex.Unlock();
		return nextJob;
	}

	void Frame::NotifyJobAdded()
	{
		for (int i = 0; i < jobAddedCallbacks.Count(); i++)
		{
			(*jobAddedCallbacks[i])();
		}
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