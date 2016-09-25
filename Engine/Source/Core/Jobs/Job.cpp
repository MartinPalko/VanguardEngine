#include "Job.h"
#include "Core.h"

namespace Vanguard
{
	void Job::Execute()
	{
		running = true;
#ifdef JOB_PROFILING
		Timespan startTime = Timespan::GetElapsedSystemTime();
#endif
		DoJob();
#ifdef JOB_PROFILING
		Profiler* profiler = Core::GetInstance()->GetProfiler();
		if (profiler && profiler->IsProfilingFrame())
			profiler->RecordJobProfile(name, startTime);
#endif
		running = false;
		finished = true;
	}
}