#include "Application.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

static Display *xDisplay;
Window xWindow;


namespace Vanguard
{
	void Application::ProcessNativeEvents()
	{
		if (xDisplay)
		{
			XEvent event;
			while (XPending(xDisplay))
			{
				XNextEvent(xDisplay, &event);

				NativeEvent nativeEvent;
				nativeEvent.message = &event;
				
				for (size_t h = 0; h < nativeEventHandlers.Count(); h++)
				{
					nativeEventHandlers[h]->HandleNativeEvent(nativeEvent);
				}
			}
		}
	}

	void Application::ShowConsoleWindow()
	{
		// TODO:
	}

	void Application::HideConsoleWindow()
	{
		// TODO:
	}

	void Application::RegisterNativeWindow(NativeWindow aWindowHandle)
	{
		nativeWindows.PushBack(aWindowHandle);
	}

	WindowHandle Application::CreateNativeWindow(const WindowCreationParameters& aWindowParameters)
	{
		if (!xDisplay)
		{
			xDisplay = XOpenDisplay(NULL);

			if (!xDisplay)
			{
				return nullptr;
			}
		}		
		
		int xScreen = DefaultScreen(xDisplay);

		xWindow = XCreateSimpleWindow(
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

		RegisterNativeWindow(NativeWindow{ (void*)xWindow, (void*)xDisplay });

		// Process events to show new window.
		ProcessNativeEvents();
		return (void*)xWindow;
	}

	void Application::GetWindowSize(WindowHandle aWindowHandle, int& OUTSizeX, int& OUTSizeY)
	{

	}
}