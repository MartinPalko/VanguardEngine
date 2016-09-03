#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <thread>

#include "Application.h"
#include "Core.h"

namespace Vanguard
{
	void Application::ProcessNativeEvents()
	{
		for (size_t w = 0; w < nativeWindows.Count(); w++)
		{
			MSG msg;
			WindowHandle handle = nativeWindows[w];
			while (PeekMessage(&msg, (HWND)handle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				NativeEvent nativeEvent;
				nativeEvent.message = &msg;
				nativeEvent.windowHandle = handle;

				for (size_t h = 0; h < nativeEventHandlers.Count(); h++)
				{
					nativeEventHandlers[h]->HandleNativeEvent(nativeEvent);
				}
			}
		}
	}

	BOOL WINAPI ConsoleEventHandler(_In_ DWORD dwCtrlType)
	{
		// For info, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683242(v=vs.85).aspx
		if (dwCtrlType == CTRL_CLOSE_EVENT)
		{
			Core* core = Core::GetInstance();

			if (core)
			{
				// Exit core properly when console windows closes
				core->ShutDown();

				// Wait for core to shut down.
				while (core->GetState() < CoreState::ShutDown)
					std::this_thread::yield();

				return true;
			}
		}
		return false;
	}

	void Application::ShowConsoleWindow()
	{
		AllocConsole();

		// Redirect output to console
		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle(HandleToLong(handle_out), _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w");
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;

		// Redirect input to console
		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle(HandleToLong(handle_in), _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;

		// Grey out close button (accidentally hitting it will close the whole app.
		HWND hwnd = GetConsoleWindow();
		HMENU hmenu = GetSystemMenu(hwnd, FALSE);
		EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

		// Set console event handler
		SetConsoleCtrlHandler(ConsoleEventHandler, true);
	}

	void Application::HideConsoleWindow()
	{
		FreeConsole();
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			nativeWindows.Remove(hWnd);
			if (!nativeWindows.Count()) // Last window destroyed
			{
				PostQuitMessage(0);
				Core::GetInstance()->ShutDown();
			}
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}

	void Application::RegisterNativeWindow(WindowHandle aWindowHandle)
	{
		nativeWindows.PushBack(aWindowHandle);
		// Override the WNDPROC to use ours
		SetWindowLongPtr((HWND)aWindowHandle, GWLP_WNDPROC, (LONG_PTR)&WndProc);
	}

	WindowHandle Application::CreateNativeWindow(const WindowCreationParameters& aWindowParameters)
	{
		static TCHAR szWindowClass[] = "win32app";

		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = NULL;
		wcex.lpfnWndProc = WndProc;		
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

		if (!RegisterClassEx(&wcex))
		{
			return nullptr;
		}

		long windowFlags = 0;
		if (aWindowParameters.resizable)
		{
			windowFlags = WS_OVERLAPPEDWINDOW;
		}
		else
		{
			windowFlags = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
		}

		HWND hWnd = CreateWindow(
			szWindowClass,
			aWindowParameters.title.GetCharPointer(),
			windowFlags,
			CW_USEDEFAULT, CW_USEDEFAULT,
			aWindowParameters.sizeX, 
			aWindowParameters.sizeY,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		if (!hWnd)
		{
			return nullptr;
		}

		ShowWindow(hWnd, SW_SHOW);

		RegisterNativeWindow(hWnd);

		return hWnd;
	}

	void Application::GetWindowSize(WindowHandle aWindowHandle, int& OUTSizeX, int& OUTSizeY)
	{
		RECT windowRect;
		GetClientRect((HWND)aWindowHandle, &windowRect);
		OUTSizeX = windowRect.right - windowRect.left;
		OUTSizeY = windowRect.bottom - windowRect.top;
	}
}