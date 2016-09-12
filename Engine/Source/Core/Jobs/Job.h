#pragma once
#include "Log.h"
#include "JobEnums.h"

namespace Vanguard
{	
	class Frame;

	class CORE_API Job
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
		virtual ~Job() {}

		String GetName() { return name; }

	protected:
		// Only called by JobManager and/or JobWorker
		virtual void Execute();

		virtual void DoJob() = 0;
	};

	class CORE_API LambdaJob : public Job
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