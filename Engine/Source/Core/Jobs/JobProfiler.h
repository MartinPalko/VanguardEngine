#pragma once
#include "Core_Common.h"
#include "JobManager.h"

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
		DynamicArray<Record> records;
		Mutex recordMutex;
		bool profiling;
		Timespan profilingStartTime;

		void AddRecord(const Record& aRecord);

	public:
		void StartProfiling();
		bool IsProfiling() { return profiling; }
		void EndProfiling(FilePath aWriteResultsTo);
	};
}
#endif