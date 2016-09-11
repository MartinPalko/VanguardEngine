#include "Profiler.h"
#include "Core.h"
#include "Jobs/JobManager.h"
#include "Jobs/JobWorker.h"

namespace Vanguard
{
	void Profiler::StartProfiling()
	{
		profiling = true;
		profilingStartTime = Timespan::GetElapsedSystemTime();
	}

	void Profiler::EndProfiling()
	{
		// Nowhere to dump to, so just discard the record queue.
		const size_t maxDequeRecords = 128;
		JobProfile dequeuedRecords[128];
		while (jobProfiles.try_dequeue_bulk(dequeuedRecords, maxDequeRecords));
	}

	void Profiler::EndProfiling(FilePath aWriteResultsTo)
	{		
		profiling = false;

		String outputString = "[";
		
		const size_t maxDequeRecords = 128;
		JobProfile dequeuedRecords[128];
		size_t dequeuedRecordsCount = -1;
		
		while (dequeuedRecordsCount != 0)
		{
			dequeuedRecordsCount = jobProfiles.try_dequeue_bulk(dequeuedRecords, maxDequeRecords);

			for (int i = 0; i < dequeuedRecordsCount; i++)
			{
				JobProfile& jobProfile = dequeuedRecords[i];

				if (i != 0)
					outputString += ", ";

				int startTime = (jobProfile.startTime - profilingStartTime).InSeconds() * 1000.0 * 1000.0 * 1000.0;
				int endTime = (jobProfile.endTime - profilingStartTime).InSeconds() * 1000.0 * 1000.0 * 1000.0;

				outputString += "{\n";
				outputString += "\"Thread\": \"" + jobProfile.threadName + "\",\n";
				outputString += "\"Name\": \"" + jobProfile.jobName + "\",\n";
				outputString += "\"Start\": " + String::FromInt(startTime) + ",\n";
				outputString += "\"End\": " + String::FromInt(endTime) + "\n";
				outputString += "}";
			}
		}

		outputString += "]";

		FileSystem::WriteToFile(aWriteResultsTo, outputString);
	}

	void Profiler::RecordJobProfile(const String & aJobName, Timespan aStartTime)
	{
		if (profiling)
		{
			JobProfile newProfile;

			size_t threadID = Thread::CurrentThreadID();
			JobWorker* worker = Core::GetInstance()->GetJobManager()->GetWorkerByThreadID(threadID);

			if (worker)
				newProfile.threadName = worker->GetName();
			else if (Core::GetInstance()->GetMainThreadID() == threadID)
				newProfile.threadName = "Main";
			else
				newProfile.threadName = "Unknown";

			newProfile.jobName = aJobName;
			newProfile.startTime = aStartTime;
			newProfile.endTime = Timespan::GetElapsedSystemTime();			

			jobProfiles.enqueue(newProfile);
		}
	}

}