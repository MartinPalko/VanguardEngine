#pragma once
#include <queue>
#include <functional>

#include "Core_Common.h"
#include "Foundation.h"

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

		static IOThread* ioThread;

		static void AddTask(IOTask* task);

	public:
		static void StartIOThread();
		static void JoinIOThread();

		static void WriteToFile(const FilePath& aFilePath, const String& aStringContents);
		static void AppendToFile(const FilePath& aFilePath, const String& aStringContents);
	};
}