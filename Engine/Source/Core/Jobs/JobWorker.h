#pragma once
#include <thread>
#include "Foundation.h"

namespace Vanguard
{
	class JobManager;
	class Job;
	class Frame;

	class JobWorker : public Thread
	{
		friend JobManager;

	private:
		class JobManager* jobManager;
		Job* currentJob;		
		std::atomic<bool> workerRunning;
		uint16 index;

		virtual void Run() override;

	public:
		explicit JobWorker(int aIndex, JobManager* aJobManager) : Thread("Worker " + String::FromInt(aIndex))
			, jobManager(aJobManager)
			, currentJob (nullptr)
			, workerRunning(true)
			, index (aIndex)
		{
		}

		bool IsIdle() { return !workerRunning; }
	};
}