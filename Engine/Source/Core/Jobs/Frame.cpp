#include "Frame.h"
#include "JobManager.h"
#include "JobWorker.h"
#include "Core.h"


namespace Vanguard
{
	void Frame::AddJob(FrameJob* aJob)
	{
		unfinishedJobs++;

		if (started)
		{
			Core::GetInstance()->GetJobManager()->AddJob(aJob);
		}
		else
		{
			pendingJobs.PushBack(aJob);
		}
	}

	void Frame::AddJobs(FrameJob** aJobs, size_t aNumJobs)
	{
		unfinishedJobs += aNumJobs;
		Core::GetInstance()->GetJobManager()->AddJobs((Job**)aJobs, aNumJobs);
	}

	void Frame::Start()
	{
		started = true;
		for (int i = 0; i < pendingJobs.Count(); i++)
		{
			Core::GetInstance()->GetJobManager()->AddJob(pendingJobs[i]);
		}
		pendingJobs.Clear();		
	}

	bool Frame::Finished()
	{
		return started && unfinishedJobs == 0;
	}
}