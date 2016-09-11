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
			if (jobManager->jobs.wait_dequeue_timed(currentJob, std::chrono::milliseconds(1)))
			{
				workerRunning = true;
				currentJob->Execute();
				Job* finishedJob = currentJob;
				currentJob = nullptr;
				jobManager->WorkerFinishedJob(this, finishedJob);
				workerRunning = false;
			}
		}
	}
}
