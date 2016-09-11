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
	class FrameJob;

	class CORE_API Frame
	{
		friend class JobManager;
		friend class FrameJob;

	private:
		std::atomic<bool> started;
		DynamicArray<Job*> pendingJobs;
		std::atomic<size_t> unfinishedJobs;
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

		void AddJob(FrameJob* aJob);
		void AddJobs(FrameJob** aJobs, size_t aNumJobs);
		void Start();
		bool Finished();
	};

	class CORE_API FrameJob : public Job
	{
	protected:
		Frame* frame;

	public:
		FrameJob(const String& aName, Frame* aFrame, bool aRequiresMainThreadExecution = false) : Job(aName, aRequiresMainThreadExecution)
		{
			frame = aFrame;
		}

	protected:
		virtual void Execute()
		{
			Job::Execute();
			frame->unfinishedJobs--;
		}
	};
}