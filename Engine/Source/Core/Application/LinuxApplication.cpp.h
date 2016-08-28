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


		RegisterNativeWindow((void*)xWindow);

		// Process events to show new window.
		ProcessNativeEvents();
		return (void*)xWindow;


		// /* get the colors black and white (see section for details) */
		// unsigned long black,white;

		// /* use the information from the environment variable DISPLAY 
		// to create the X connection:
		// */	
		// dis = XOpenDisplay((char *)0);
		// screen = DefaultScreen(dis);
		// black = BlackPixel(dis, screen),	/* get color black */
		// white = WhitePixel(dis, screen);  /* get color white */

		// /* once the display is initialized, create the window.
		// This window will be have be 200 pixels across and 300 down.
		// It will have the foreground white and background black
		// */
		// win = XCreateSimpleWindow(
		// 	dis,
		// 	DefaultRootWindow(dis),
		// 	100,
		// 	100,	
		// 	aWindowParameters.sizeX, 
		// 	aWindowParameters.sizeY,
		// 	5, 
		// 	black,
		// 	white);

		// /* here is where some properties of the window can be set.
		// The third and fourth items indicate the name which appears
		// at the top of the window and the name of the minimized window
		// respectively.
		// */
		// XSetStandardProperties(
		// 	dis,
		// 	win,
		// 	aWindowParameters.title.GetCharPointer(),
		// 	aWindowParameters.title.GetCharPointer(),
		// 	None,
		// 	NULL,
		// 	0,
		// 	NULL);

		// /* this routine determines which types of input are allowed in
		// the input.  see the appropriate section for details...
		// */
		// XSelectInput(
		// 	dis,
		// 	win,
		// 	ExposureMask | ButtonPressMask | KeyPressMask);

		// /* create the Graphics Context */
		// gc = XCreateGC(dis, win, 0, 0);

		// /* clear the window and bring it on top of the other windows */
		// XClearWindow(dis, win);
		// XMapRaised(dis, win);

		// return (void*)win;
	}

	void Application::GetWindowSize(WindowHandle aWindowHandle, int& OUTSizeX, int& OUTSizeY)
	{

	}
}