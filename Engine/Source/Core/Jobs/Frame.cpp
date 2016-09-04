#include "Frame.h"
#include "JobManager.h"
#include "JobWorker.h"
#include "Core.h"


namespace Vanguard
{
	void Frame::AddJob(String aName, std::function<void()> aEntryPoint)
	{
		unfinishedJobs++;
		Job* newJob = new Job(aName, [aEntryPoint, this]()
		{
			aEntryPoint(); 
			this->unfinishedJobs--;
		});

		if (started)
		{
			Core::GetInstance()->GetJobManager()->AddJob(newJob);
		}
		else
		{
			pendingJobs.PushBack(newJob);
		}
	}

	void Frame::Start()
	{
		for (int i = 0; i < pendingJobs.Count(); i++)
		{
			Core::GetInstance()->GetJobManager()->AddJob(pendingJobs[i]);
		}
		pendingJobs.Clear();
		
		started = true;
	}

	bool Frame::Finished()
	{
		return started && unfinishedJobs == 0;
	}
}