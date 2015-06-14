#include "JobManager.h"
#include "JobThread.h"
#include "Job.h"
#include "Frame.h"

namespace Vanguard
{
	std::queue<Frame*> JobManager::queuedFrames;

	List<JobThread*> JobManager::jobThreads = List<JobThread*>();
	std::queue<JobThread*> JobManager::idleThreads = std::queue<JobThread*>();

	JobThread* JobManager::GetIdleThread()
	{
		if (idleThreads.size() > 0)
		{
			JobThread* idleThread = idleThreads.front();
			idleThreads.pop();
			return idleThread;
		}
		return nullptr;
	}

	Job* JobManager::GetNextJob()
	{
		if (queuedFrames.size() > 0)
		{
			Frame* frame = queuedFrames.front();

			for (int i = (uint8)JobPriority::qty - 1; i >= 0; i--)
			{
				if (frame->jobs[i].size() > 0)
				{
					Job* nextJob = frame->jobs[i].front();
					frame->jobs[i].pop();
					return nextJob;
				}
			}
		}
		return nullptr;
	}

	void JobManager::ThreadFinishedJob(JobThread* aThread, Job* aJob)
	{
		delete aJob;
		std::cout << "Job Finished" << "\n";
		Job* nextJob = GetNextJob();
		if (nextJob)
			aThread->RunJob(nextJob);
	}

	void JobManager::CreateThreads()
	{
		int32 targetThreads = SystemInfo::GetNumberOfCores();
		std::cout << "Created " << targetThreads << " job threads.\n";

		int32 threadsToCreate = targetThreads - jobThreads.Size();

		for (int i = 0; i < threadsToCreate; i++)
		{
			JobThread* newJobThread = new JobThread();
			jobThreads.PushBack(newJobThread);
			idleThreads.push(newJobThread);
		}
	}

	void JobManager::ProcessFrame(Frame* aFrame)
	{
		queuedFrames.push(aFrame);

		JobThread* idleThread = GetIdleThread();
		while (idleThread != nullptr)
		{
			idleThread->RunJob(GetNextJob());
			idleThread = GetIdleThread();
		}

		while (!aFrame->JobsFinished())
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		for (int i = 0; i < jobThreads.Size(); i++)
		{
			while (!jobThreads[i]->IsIdle())
				std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

}