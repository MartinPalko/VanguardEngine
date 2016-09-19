#pragma once
#include "Core_Common.h"
#include "Config/Config.h"

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

	typedef void* WindowHandle;
	typedef void* NativeEvent;
#ifdef VANGUARD_LINUX
	typedef void* X11Display;
#endif

	struct NativeWindow
	{
		WindowHandle handle;
#ifdef VANGUARD_LINUX
		X11Display display;
#endif
	};

	class INativeEventProcessor
	{
	public:
		virtual bool GetNextEvent(NativeEvent& aOutNextEvent) = 0;
	};

	class INativeEventHandler
	{
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) = 0;
	};

	struct WindowCreationParameters
	{
		bool fullscreen;
		String title;
		int sizeX;
		int sizeY;
		bool resizable;

		WindowCreationParameters()
			: fullscreen(false)
			, title("Vanguard")
			, sizeX(1280)
			, sizeY(760)
			, resizable(false)
		{}
	};

	class CORE_API Application
	{
		static BooleanConfigVar fullscreen;
		static Int32ConfigVar resolutionX;
		static Int32ConfigVar resolutionY;
		static BooleanConfigVar resizable;

		friend Core;
	private:
		static void SetApplicationArguments(int aArgC, char** aArgV);
		static void ProcessNativeEvents();
	public:
		static ApplicationArguments GetApplicationArguments();

		static void ShowConsoleWindow();
		static void HideConsoleWindow();

		static void RegisterNativeEventProcessor(INativeEventProcessor* aProcessor);
		static void UnregisterNativeEventProcessor(INativeEventProcessor* aProcessor);
		static void RegisterNativeEventHandler(INativeEventHandler* aHandler);
		static void UnregisterNativeEventHandler(INativeEventHandler* aHandler);

		static WindowCreationParameters GetWindowCreationParams();
	};
}