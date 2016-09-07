#include "JobProfiler.h"

#ifdef JOB_PROFILING
namespace Vanguard
{
	void JobProfiler::AddRecord(const Record& aRecord)
	{
		if (profiling)
		{
			records.enqueue(aRecord);
		}
	}

	void JobProfiler::StartProfiling()
	{
		profiling = true;
		profilingStartTime = Timespan::GetElapsedSystemTime();
	}

	void JobProfiler::EndProfiling()
	{
		// Nowhere to dump to, so just discard the record queue.
		const size_t maxDequeRecords = 128;
		Record dequeuedRecords[128];
		while (records.try_dequeue_bulk(dequeuedRecords, maxDequeRecords));
	}

	void JobProfiler::EndProfiling(FilePath aWriteResultsTo)
	{		
		profiling = false;

		String outputString = "[";
		
		const size_t maxDequeRecords = 128;
		Record dequeuedRecords[128];
		size_t dequeuedRecordsCount = -1;
		
		while (dequeuedRecordsCount != 0)
		{
			dequeuedRecordsCount = records.try_dequeue_bulk(dequeuedRecords, maxDequeRecords);

			for (int i = 0; i < dequeuedRecordsCount; i++)
			{
				Record& record = dequeuedRecords[i];

				if (i != 0)
					outputString += ", ";

				int startTime = (record.startTime - profilingStartTime).InSeconds() * 1000.0 * 1000.0 * 1000.0;
				int endTime = (record.endTime - profilingStartTime).InSeconds() * 1000.0 * 1000.0 * 1000.0;

				outputString += "{\n";
				outputString += "\"Thread\": \"Worker " + String::FromInt(record.thread) + "\",\n";
				outputString += "\"Name\": \"" + record.jobName + "\",\n";
				outputString += "\"Start\": " + String::FromInt(startTime) + ",\n";
				outputString += "\"End\": " + String::FromInt(endTime) + "\n";
				outputString += "}";
			}
		}

		outputString += "]";

		FileSystem::WriteToFile(aWriteResultsTo, outputString);
	}
}
#endif