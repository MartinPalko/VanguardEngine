#include "Profiler.h"
#include "Core.h"
#include "Jobs/JobManager.h"
#include "Jobs/JobWorker.h"

namespace Vanguard
{
	Profiler::Profiler()
		: profileNextFrame(false)
		, profilingFrame(false)
		, currentFrameProfile()
	{

	}

	Profiler::~Profiler()
	{

	}

	void Profiler::BeginFrameProfile()
	{
		ASSERT_MAIN_THREAD;
		if (profilingFrame)
		{
			LOG_ERROR("Already profiling frame. Cannot begin a new frame profile before the last one has ended.", "Profiler");
			return;
		}

		profilingFrame = true;
		currentFrameProfile.profilingStartTime = Timespan::GetElapsedSystemTime();
	}

	void Profiler::EndFrameProfile()
	{
		ASSERT_MAIN_THREAD;

		// Nowhere to dump to, so just discard the record queue.
		const size_t maxDequeRecords = 128;
		JobProfile dequeuedRecords[128];
		while (currentFrameProfile.jobProfiles.try_dequeue_bulk(dequeuedRecords, maxDequeRecords));
	}

	void Profiler::EndFrameProfile(FilePath aWriteResultsTo)
	{
		ASSERT_MAIN_THREAD;

		profilingFrame = false;

		String outputString = "[";
		
		const size_t maxDequeRecords = 128;
		JobProfile dequeuedRecords[128];
		size_t dequeuedRecordsCount = -1;
		
		while (dequeuedRecordsCount != 0)
		{
			dequeuedRecordsCount = currentFrameProfile.jobProfiles.try_dequeue_bulk(dequeuedRecords, maxDequeRecords);

			for (int i = 0; i < dequeuedRecordsCount; i++)
			{
				JobProfile& jobProfile = dequeuedRecords[i];

				if (i != 0)
					outputString += ", ";

				int startTime = (jobProfile.startTime - currentFrameProfile.profilingStartTime).InSeconds() * 1000.0 * 1000.0 * 1000.0;
				int endTime = (jobProfile.endTime - currentFrameProfile.profilingStartTime).InSeconds() * 1000.0 * 1000.0 * 1000.0;

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
		if (profilingFrame)
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

			currentFrameProfile.jobProfiles.enqueue(newProfile);
		}
	}

}