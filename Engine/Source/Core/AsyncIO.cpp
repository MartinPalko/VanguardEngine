#include "blockingconcurrentqueue.h"
#include "AsyncIO.h"
#include "Core.h"

namespace Vanguard
{
	moodycamel::BlockingConcurrentQueue<AsyncIO::IOTask*> taskQueue;
	AsyncIO::IOThread* AsyncIO::ioThread = nullptr;

	void AsyncIO::IOThread::Run()
	{
		while (!wantsJoin)
		{
			IOTask* nextTask = nullptr;
			while (taskQueue.wait_dequeue_timed(nextTask, std::chrono::milliseconds(1)))
			{
				(*nextTask)();
				delete nextTask;
			}
		}
	}

	void AsyncIO::AddTask(IOTask* task)
	{
		taskQueue.enqueue(task);
	}

	void AsyncIO::StartIOThread()
	{
		if (!ioThread)
		{
			ioThread = new IOThread();
			ioThread->Start();
		}
	}

	void AsyncIO::JoinIOThread()
	{
		if (ioThread)
		{
			ioThread->Join();
			delete ioThread;
			ioThread = nullptr;
		}
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