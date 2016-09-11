#pragma once
#include "Core_Common.h"

#include "ThirdParty/ConcurrentQueue/blockingconcurrentqueue.h"

// Comment out to disable job profiling.
#define JOB_PROFILING

#ifdef JOB_PROFILING
namespace Vanguard
{
	class CORE_API Profiler
	{
	public:
		struct JobProfile
		{
			String threadName;
			String jobName;
			Timespan startTime;
			Timespan endTime;
		};

	private:
		moodycamel::BlockingConcurrentQueue<JobProfile> jobProfiles;
		std::atomic<bool> profiling;
		Timespan profilingStartTime;

	public:
		void StartProfiling();
		bool IsProfiling() { return profiling; }
		void EndProfiling();
		void EndProfiling(FilePath aWriteResultsTo);

		void RecordJobProfile(const String& aJobName, Timespan aStartTime);
	};
}
#endif