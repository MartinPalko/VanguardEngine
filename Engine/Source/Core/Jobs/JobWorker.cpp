#include "JobWorker.h"
#include "JobManager.h"
#include "Job.h"

#include <condition_variable>

namespace Vanguard
{
	void JobWorker::Run()
	{
		while (!wantsJoin)
		{
			if (currentJob == nullptr)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				//std::this_thread::yield();
			}
			else
			{
				currentJob->Execute();

				Job* finishedJob = currentJob;
				currentJob = nullptr;
				jobManager->WorkerFinishedJob(this, finishedJob);
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
