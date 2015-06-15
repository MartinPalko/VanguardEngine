#include "Frame.h"
#include "JobManager.h"
#include "JobThread.h"


namespace Vanguard
{
	void Frame::AddJob(jobEntryPoint aEntryPoint, JobPriority::Type aPriority)
	{
		jobs[(uint8)aPriority].push(new Job(aEntryPoint,this));
	}
}