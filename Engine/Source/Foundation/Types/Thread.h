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
		std::thread* stdThread;
		bool running;		
		
		void ThreadEntry();

	protected:
		bool wantsJoin;
		virtual void Run() = 0;

	public:
		explicit Thread(const String& aName);
		virtual ~Thread();

		static String CurrentThreadID();

		void Start();		

		void SetName(const String& aThreadName);
		String GetName();
		String GetID();
		void Join();
		bool IsRunning();
	};
}
