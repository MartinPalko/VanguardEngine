#pragma once
#include <future>

#include "Foundation_Common.h"
#include "VanguardString.h"

namespace std { class thread; }

namespace Vanguard
{
	class FOUNDATION_API Thread
	{
	private:
		String name;
		size_t affinityMask;
		std::thread* stdThread;
		bool running;

		size_t threadID;
		
		void ThreadEntry();

	protected:
		std::atomic<bool> wantsJoin;
		virtual void Run() = 0;

	public:
		explicit Thread(const String& aName);
		virtual ~Thread();

		static size_t CurrentThreadID();

		void Start();		

		void SetName(const String& aThreadName);
		void SetAffinityMask(size_t aAffinityMask);
		String GetName();
		size_t GetID();
		void Join();
		bool IsRunning();
	};
}
