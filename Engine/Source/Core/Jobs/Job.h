#pragma once
#include "Log.h"
#include "JobEnums.h"

namespace Vanguard
{	
	class Frame;

	class Job
	{
		// TODO: Add callbacks when job finishes.
		friend class JobManager;
		friend class JobWorker;

	private:
		String name;
		bool requiresMainThreadExecution;
		bool running;
		bool finished;

	public:
		Job(const String& aName, bool aRequiresMainThreadExecution = false)
		{
			name = aName;
			requiresMainThreadExecution = aRequiresMainThreadExecution;
			running = false;
			finished = false;
		}

		String GetName() { return name; }

	protected:
		// Only called by JobManager and/or JobWorker
		virtual void Execute()
		{
			running = true;
			DoJob();
			running = false;
			finished = true;
		}

		virtual void DoJob() = 0;
	};

	class LambdaJob : public Job
	{
	private:
		std::function<void()> entryPoint;

	protected:
		virtual void DoJob() override
		{
			entryPoint();
		}

	public:
		LambdaJob(const String& aName, std::function<void()> aEntryPoint) : Job(aName)
		{
			entryPoint = aEntryPoint;
		}
	};
}