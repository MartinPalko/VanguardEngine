#include "JobThread.h"
#include "JobManager.h"
#include "Job.h"

#include <condition_variable>

namespace Vanguard
{
	void JobThread::ThreadLoop()
	{
		std::cout << "Started thread loop" << "\n";

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
				std::cout << "Executing job" << "\n";
				currentJob->Execute();

				Job* finishedJob = currentJob;
				currentJob = nullptr;
				JobManager::ThreadFinishedJob(this, finishedJob);
			}
		}
	}

	void JobThread::RunJob(Job* aJob)
	{
		currentJob = aJob;
	}
}