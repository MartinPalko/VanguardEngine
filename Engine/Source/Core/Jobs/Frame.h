#pragma once
#include "Foundation.h"
#include "Core_Common.h"
#include "JobEnums.h"
#include "Job.h"
#include <queue>
#include <mutex>

namespace Vanguard
{
	class World;
	class Job;
	class CORE_API Frame
	{
		friend class JobManager;
	private:
		bool processing;
		Mutex jobListMutex;
		DynamicArray<std::function<void()>* > jobAddedCallbacks;

		Job* GetNextJob();
		size_t GetRemainingJobs();
		void NotifyJobAdded();

	public:
		const int frameNumber;
		const Timespan deltaTime;
		const World* world;

		std::queue<Job*> jobs[JobPriority::qty];
		size_t queuedJobs[JobPriority::qty];
		size_t unfinishedJobs[JobPriority::qty];

		void WaitForJob(Job* aJob);

		Frame(uint32 aFrameNumber, Timespan aDeltaTime, World* aWorld)
			: frameNumber(aFrameNumber)
			, deltaTime(aDeltaTime)
			, world(aWorld)
			, queuedJobs { 0 }
			, unfinishedJobs { 0 }
		{
			processing = false;
		}

		~Frame()
		{
			for (int i = 0; i < jobAddedCallbacks.Count(); i++)
			{
				delete jobAddedCallbacks[i];
			}
		}

		Job* AddJob(std::function<void()> aEntryPoint, JobPriority::Type aPriority = JobPriority::Normal);
		

		bool JobsFinished()
		{
			for (int i = 0; i < JobPriority::qty; i++)
			{
				if (unfinishedJobs[i] > 0)
					return false;
			}
			return true;
		}

		void RegisterJobAddedCallback(std::function<void()>* aCallbackFunction)
		{
			jobAddedCallbacks.PushBack(aCallbackFunction);
		}

		void UnregisterJobAddedCallback(std::function<void()>* aCallbackFunction)
		{
			jobAddedCallbacks.Remove(aCallbackFunction);
		}
	};
}