#include "Application.h"
#include "Core.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>

#include <thread>

namespace Vanguard
{
	BOOL WINAPI ConsoleEventHandler(_In_ DWORD dwCtrlType)
	{
		// See https://msdn.microsoft.com/en-us/library/windows/desktop/ms683242(v=vs.85).aspx for info

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
}