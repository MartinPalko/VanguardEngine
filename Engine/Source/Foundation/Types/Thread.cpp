#include <thread>
#include <sstream>
#include <future>

#include "Thread.h"

#include "Platforms/Platform.h"

#ifdef VANGUARD_WINDOWS
#include <Windows.h>
#endif // VANGUARD_WINDOWS

namespace Vanguard
{
	void Thread::ThreadEntry()
	{
		wantsJoin = false;
		running = true;
		Run();
		running = false;
	}

	Thread::Thread(const String& aString)
		: wantsJoin (false)
		, running (true)
		, threadID (0)
		, stdThread (nullptr)
		, name (aString)
		, affinityMask (0)
	{
	}

	Thread::~Thread()
	{
		Join();
	}

	size_t Thread::CurrentThreadID()
	{
		return std::hash<std::thread::id>()(std::this_thread::get_id());
	}

	void Thread::Start()
	{
		if (!IsRunning())
		{
			

			stdThread = new std::thread(&Thread::ThreadEntry, this);
			if (name == "")
				name = "Vanguard Thread " + String::FromInt(GetID());
			SetName(name);
			if (affinityMask)
				SetAffinityMask(affinityMask);
		}
	}

	void Thread::SetName(const String& aThreadName)
	{
		name = aThreadName;
		#ifdef VANGUARD_WINDOWS
		if (stdThread)
		{
			const DWORD MS_VC_EXCEPTION = 0x406D1388;
			#pragma pack(push,8)
			typedef struct tagTHREADNAME_INFO
			{
				DWORD dwType; // Must be 0x1000.
				LPCSTR szName; // Pointer to name (in user addr space).
				DWORD dwThreadID; // Thread ID (-1=caller thread).
				DWORD dwFlags; // Reserved for future use, must be zero.
			} THREADNAME_INFO;
			#pragma pack(pop)
			THREADNAME_INFO info;
			info.dwType = 0x1000;
			info.szName = name.GetCharPointer();
			info.dwThreadID = GetThreadId(static_cast<HANDLE>(stdThread->native_handle()));
			info.dwFlags = 0;
			#pragma warning(push)
			#pragma warning(disable: 6320 6322)
			__try {
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
			}
			#pragma warning(pop)
		}
		#endif // VANGUARD_WINDOWS
	}

	void Thread::SetAffinityMask(size_t aAffinityMask)
	{
		affinityMask = aAffinityMask;
#ifdef VANGUARD_WINDOWS
		if (stdThread)
		{
			SetThreadAffinityMask(static_cast<HANDLE>(stdThread->native_handle()), aAffinityMask);
		}
#endif
	}

	String Thread::GetName()
	{
		return name;
	}

	size_t Thread::GetID()
	{
		return std::hash<std::thread::id>()(stdThread->get_id());
	}

	void Thread::Join()
	{
		if (stdThread && stdThread->joinable())
		{
			wantsJoin = true;
			stdThread->join();
			delete stdThread;
			stdThread = nullptr;
		}
	}

	bool Thread::IsRunning()
	{
		if (!stdThread)
		{
			return false;
		}

		return running;
	}
}