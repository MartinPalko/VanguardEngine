#pragma once
#include "Core_Common.h"
#include "Foundation.h"

#include <queue>
#include <functional>

namespace Vanguard
{
	class CORE_API AsyncIO
	{
	public:
		typedef std::function<void()> IOTask;
	private:
		class IOThread : public Thread
		{
		public:
			IOThread() : Thread("IO Thread") {}
			virtual void Run() override;
		};

		static std::queue<IOTask*> taskQueue;
		static Mutex taskQueueMutex;
		static IOThread* ioThread;

		static void AddTask(IOTask* task);
		static IOTask* GetNextTask();

	public:
		static void JoinIOThread();

		static void WriteToFile(const FilePath& aFilePath, const String& aStringContents);
		static void AppendToFile(const FilePath& aFilePath, const String& aStringContents);
	};
}