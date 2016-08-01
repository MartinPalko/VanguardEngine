#pragma once
#include "Foundation.h"
#include <thread>

namespace Vanguard
{
	class JobManager;
	class Job;
	class Frame;

	class JobWorker
	{
		friend JobManager;

	private:		
		Job* currentJob;		
		bool running;
		uint16 index;
		Thread thread;

		void ThreadLoop();

	public:
		explicit JobWorker(int aIndex)
			: currentJob (nullptr)
			, running (true)
			, index (aIndex)
			, thread(&JobWorker::ThreadLoop, this)
		{			
			thread.SetName("Vanguard Worker " + String::FromInt32(aIndex));
		}

		bool IsIdle() { return currentJob == nullptr; }

		void StartJob(Job* aJob);
	};
}