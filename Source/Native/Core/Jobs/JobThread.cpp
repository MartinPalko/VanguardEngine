#include "JobThread.h"
#include "JobManager.h"
#include "Job.h"

#include <condition_variable>

namespace Vanguard
{
	void JobThread::ThreadLoop()
	{
		int i = 0;

		while (running)
		{
			if (currentJob == nullptr)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				//std::this_thread::yield();
			}
			else
			{
				currentJob->Execute();

				Job* finishedJob = currentJob;
				currentJob = nullptr;
				JobManager::ThreadFinishedJob(this, finishedJob);
			}
		}
	}

	void JobThread::StartJob(Job* aJob)
	{
		currentJob = aJob;
	}
}
