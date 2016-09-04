#pragma once
#include <queue>
#include <mutex>

#include "Foundation.h"
#include "Core_Common.h"
#include "JobEnums.h"
#include "Job.h"



namespace Vanguard
{
	class World;
	class Job;

	class CORE_API Frame
	{
		friend class JobManager;
	private:
		bool started;
		DynamicArray<Job*> pendingJobs;
		Mutex unfinishedJobstMutex;
		size_t unfinishedJobs;
	public:
		const int frameNumber;
		const Timespan deltaTime;
		const World* world;

		Frame(uint32 aFrameNumber, Timespan aDeltaTime, World* aWorld)
			: frameNumber(aFrameNumber)
			, deltaTime(aDeltaTime)
			, world(aWorld)
		{
			started = false;
			unfinishedJobs = 0;
		}

		~Frame()
		{
		}

		void AddJob(String aName, std::function<void()> aEntryPoint);
		void Start();
		bool Finished();
	};
}