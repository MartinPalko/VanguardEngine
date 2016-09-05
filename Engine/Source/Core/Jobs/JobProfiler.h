#pragma once
#include "Core_Common.h"
#include "JobManager.h"

#include "ThirdParty/ConcurrentQueue/blockingconcurrentqueue.h"

// Comment out to disable job profiling.
#define JOB_PROFILING

#ifdef JOB_PROFILING
namespace Vanguard
{
	class CORE_API JobProfiler
	{
		friend class JobWorker;

	public:
		struct Record
		{
			int thread;
			String jobName;
			Timespan startTime;
			Timespan endTime;
		};

	private:
		moodycamel::BlockingConcurrentQueue<Record> records;
		std::atomic<bool> profiling;
		Timespan profilingStartTime;

		void AddRecord(const Record& aRecord);

	public:
		void StartProfiling();
		bool IsProfiling() { return profiling; }
		void EndProfiling(FilePath aWriteResultsTo);
	};
}
#endif