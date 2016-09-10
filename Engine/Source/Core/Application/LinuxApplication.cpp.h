#include "Application.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

namespace Vanguard
{
	void Application::ShowConsoleWindow()
	{
		// TODO:
	}

	void Application::HideConsoleWindow()
	{
		// TODO:
	}

	class LinuxEventProcessor : public INativeEventProcessor
	{
	private:
		Display* display;

	public:
		LinuxEventProcessor(Display* aDisplay) : display(aDisplay) { Application::RegisterNativeEventProcessor(this); }
		~LinuxEventProcessor() { Application::UnregisterNativeEventProcessor(this); }

		virtual bool GetNextEvent(NativeEvent& aOutNextEvent) override
		{
			if (XPending(display))
			{
				XNextEvent(display, (XEvent*)aOutNextEvent);
				return true;
			}
			return false;
		}
	};

	WindowHandle Application::CreateNativeWindow(const WindowCreationParameters& aWindowParameters)
	{
		static Display* xDisplay = nullptr;

		if (!xDisplay)
		{
			xDisplay = XOpenDisplay(NULL);

			if (!xDisplay)
				return nullptr;
		}

		static LinuxEventProcessor eventProcessor(xDisplay);
		
		int xScreen = DefaultScreen(xDisplay);

		Window xWindow = XCreateSimpleWindow(
			xDisplay,
			RootWindow(xDisplay, xScreen),
			0,
			0,
			aWindowParameters.sizeX,
			aWindowParameters.sizeY,
			1,
			BlackPixel(xDisplay, xScreen),
			WhitePixel(xDisplay, xScreen)
		);

		long eventMask = ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
		XSelectInput(xDisplay, xWindow, eventMask);
		XMapWindow(xDisplay, xWindow);

		GC gc = XCreateGC(xDisplay, xWindow, 0, 0);

		// Process events to show new window.
		ProcessNativeEvents();
		return (void*)xWindow;
	}

	void Application::GetWindowSize(WindowHandle aWindowHandle, int& OUTSizeX, int& OUTSizeY)
	{

	}
}