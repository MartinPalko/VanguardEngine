#pragma once
#include "Foundation.h"
#include <thread>

namespace Vanguard
{
	class JobManager;
	class Job;
	class Frame;

	class JobWorker : public Thread
	{
		friend JobManager;

	private:		
		Job* currentJob;		
		bool workerRunning;
		uint16 index;

		virtual void Run() override;

	public:
		explicit JobWorker(int aIndex) : Thread("Vanguard Worker " + String::FromInt32(aIndex))
			, currentJob (nullptr)
			, workerRunning(true)
			, index (aIndex)
		{
		}

		bool IsIdle() { return currentJob == nullptr; }

		void StartJob(Job* aJob);
	};
}