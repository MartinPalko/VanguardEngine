#pragma once
#include "Foundation_Common.h"
#include "VanguardString.h"

namespace std {
	class thread;
}

namespace Vanguard
{
	class FOUNDATION_API Thread
	{
	private:
		String name;
	protected:
		std::thread* stdThread;

	public:
		template<class F, class... Args>
		explicit Thread(F&& f, Args&&... args)
		{
			stdThread = new std::thread(std::forward<F>(f), std::forward<Args>(args)...);
			SetName("Vanguard Thread " + GetID());
		}

		~Thread()
		{
			delete stdThread;
		}

		static String CurrentThreadID();

		void SetName(const String& aThreadName);
		String GetName();
		String GetID();
	};
}
