#include <condition_variable>

#include "JobWorker.h"
#include "JobManager.h"
#include "Job.h"

namespace Vanguard
{
	void JobWorker::Run()
	{
		while (!wantsJoin)
		{
			if (currentJob)
			{
				currentJob->Execute();

				Job* finishedJob = currentJob;
				currentJob = nullptr;
				jobManager->WorkerFinishedJob(this, finishedJob);				
			}
			else
			{
				//std::this_thread::sleep_for(std::chrono::microseconds(100));
				std::this_thread::yield();
			}
		}
	}

	void JobWorker::StartJob(Job* aJob)
	{
		currentJob = aJob;
		if (!IsRunning())
		{
			Start();
		}
	}
}
