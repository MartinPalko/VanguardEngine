#pragma once
#include "Foundation.h"
#include <thread>

namespace Vanguard
{
	class JobManager;
	class Job;
	class Frame;

	class JobThread
	{
		friend JobManager;

	private:
		std::thread stdthread;

		Job* currentJob;
		
		bool running;

		void ThreadLoop();


	public:
		JobThread()
		{
			currentJob = nullptr;
			running = true;
			stdthread = std::thread(&JobThread::ThreadLoop,this);
		}

		bool IsIdle() { return currentJob == nullptr; }

		void StartJob(Job* aJob);


	};
}