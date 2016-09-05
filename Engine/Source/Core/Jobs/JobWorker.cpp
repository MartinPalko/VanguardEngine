#include <condition_variable>

#include "JobWorker.h"
#include "JobManager.h"
#include "JobProfiler.h"
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
#ifdef JOB_PROFILING
				Timespan startTime = Timespan::GetElapsedSystemTime();
#endif

				currentJob->Execute();

#ifdef JOB_PROFILING
				if (jobManager->profiler && jobManager->profiler->IsProfiling())
				{
					JobProfiler::Record record = { index, currentJob->GetName(), startTime, Timespan::GetElapsedSystemTime() };
					jobManager->profiler->AddRecord(record);
				}
#endif
				Job* finishedJob = currentJob;
				currentJob = nullptr;
				jobManager->WorkerFinishedJob(this, finishedJob);
				workerRunning = false;
			}
		}
	}
}
