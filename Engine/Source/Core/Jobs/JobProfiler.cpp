#include "JobProfiler.h"

#ifdef JOB_PROFILING
namespace Vanguard
{
	void JobProfiler::AddRecord(const Record& aRecord)
	{
		recordMutex.Lock();
		records.PushBack(aRecord);
		recordMutex.Unlock();
	}

	void JobProfiler::StartProfiling()
	{
		profiling = true;
		profilingStartTime = Timespan::GetElapsedSystemTime();
	}

	void JobProfiler::EndProfiling(FilePath aWriteResultsTo)
	{		
		profiling = false;

		String outputString = "[";
		recordMutex.Lock();
		for (int i = 0; i < records.Count(); i++)
		{
			Record& record = records[i];

			if (i != 0)
				outputString += ", ";

			int startTime = (record.startTime - profilingStartTime).InSeconds() * 1000.0f * 1000.0f;
			int endTime = (record.endTime - profilingStartTime).InSeconds() * 1000.0f * 1000.0f;

			outputString += "{\n";
			outputString += "\"Thread\": \"Worker " + String::FromInt(record.thread) + "\",\n";
			outputString += "\"Name\": \"" + record.jobName + "\",\n";
			outputString += "\"Start\": " + String::FromInt(startTime) + ",\n";
			outputString += "\"End\": " + String::FromInt(endTime) + "\n";
			outputString += "}";
		}
		records.Clear();
		recordMutex.Unlock();

		outputString += "]";

		FileSystem::WriteToFile(aWriteResultsTo, outputString);
	}
}
#endif