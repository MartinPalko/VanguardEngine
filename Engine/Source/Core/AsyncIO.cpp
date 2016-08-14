#include "AsyncIO.h"
#include "Core.h"

namespace Vanguard
{
	void AsyncIO::IOThread::Run()
	{
		while (!wantsJoin)
		{
			IOTask* nextTask = GetNextTask();
			while (nextTask)
			{
				(*nextTask)();
				delete nextTask;
				nextTask = GetNextTask();
			}
		}
	}

	std::queue<AsyncIO::IOTask*> AsyncIO::taskQueue;
	Mutex AsyncIO::taskQueueMutex;
	AsyncIO::IOThread* AsyncIO::ioThread = nullptr;

	void AsyncIO::AddTask(IOTask* task)
	{
		taskQueueMutex.Lock();
		taskQueue.push(task);

		if (!ioThread)
		{
			ioThread = new IOThread();
			ioThread->Start();
		}
		
		taskQueueMutex.Unlock();
	}

	AsyncIO::IOTask* AsyncIO::GetNextTask()
	{
		taskQueueMutex.Lock();
		if (taskQueue.size())
		{
			IOTask* nextTask = taskQueue.front();
			taskQueue.pop();
			taskQueueMutex.Unlock();
			return nextTask;
		}
		else
		{
			taskQueueMutex.Unlock();
			return nullptr;
		}		
	}

	void AsyncIO::JoinIOThread()
	{
		ioThread->Join();
		delete ioThread;
		ioThread = nullptr;
	}

	void AsyncIO::WriteToFile(const FilePath & aFilePath, const String & aStringContents)
	{
		AddTask(new IOTask([aFilePath, aStringContents]() {FileSystem::WriteToFile(aFilePath, aStringContents);}));
	}

	void AsyncIO::AppendToFile(const FilePath & aFilePath, const String & aStringContents)
	{
		AddTask(new IOTask([aFilePath, aStringContents]() {FileSystem::AppendToFile(aFilePath, aStringContents); }));
	}
}