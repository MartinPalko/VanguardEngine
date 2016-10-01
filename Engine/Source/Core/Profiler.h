#pragma once
#include "Core_Common.h"

#include "blockingconcurrentqueue.h"

#define JOB_PROFILING

namespace Vanguard
{
	class CORE_API Profiler
	{
		friend class Core;

	public:
		struct JobProfile
		{
			String threadName;
			String jobName;
			Timespan startTime;
			Timespan endTime;
		};

		struct FrameProfile
		{
			Timespan profilingStartTime;
			moodycamel::BlockingConcurrentQueue<JobProfile> jobProfiles;
		};

	private:
		std::atomic<bool> profileNextFrame;
		std::atomic<bool> profilingFrame;
		FrameProfile currentFrameProfile;

		void BeginFrameProfile();
		
		void EndFrameProfile();
		void EndFrameProfile(FilePath aWriteResultsTo);		

	public:
		Profiler();
		~Profiler();

		bool IsProfilingFrame() { return profilingFrame; }
		void ProfileNextFrame() { profileNextFrame = true; }
		void RecordJobProfile(const String& aJobName, Timespan aStartTime);
	};
}