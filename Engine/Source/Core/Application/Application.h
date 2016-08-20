#pragma once
#include "Core_Common.h"

namespace Vanguard
{
	class Core;

	struct CORE_API ApplicationArguments
	{
	public:
		ApplicationArguments(int aArgC, char** aArgV)
		{
			argc = aArgC;
			argv = aArgV;
		}
	
		int argc;
		char **argv;
	};

	struct NativeEvent
	{
		WindowHandle windowHandle;
		NativeMessage message;
	};

	class INativeEventHandler
	{
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) = 0;
	};

	class CORE_API Application
	{
		friend Core;
	private:
		static void SetApplicationArguments(int aArgC, char** aArgV);
		static void ProcessNativeEvents();
	public:
		static ApplicationArguments GetApplicationArguments();

		static void ShowConsoleWindow();
		static void HideConsoleWindow();

		static void RegisterNativeEventHandler(INativeEventHandler* aHandler);
		static void UnregisterNativeEventHandler(INativeEventHandler* aHandler);

		static void RegisterNativeWindow(WindowHandle aWindowHandle);
		static void UnregisterNativeWindow(WindowHandle aWindowHandle);
	};
}