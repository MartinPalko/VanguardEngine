#include "Frame.h"
#include "JobManager.h"
#include "JobThread.h"
#include "Job.h"

namespace Vanguard
{
	void Frame::AddJob(Job* aJob)
	{
		jobs[(uint8)aJob->GetPriority()].push(aJob);
	}
}