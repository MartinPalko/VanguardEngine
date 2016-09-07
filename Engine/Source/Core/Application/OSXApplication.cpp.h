#include "Application.h"

namespace Vanguard
{
	void Application::ProcessNativeEvents()
	{
		// TODO:
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
		// TODO:
	}

	void Application::GetWindowSize(WindowHandle aWindowHandle, int& OUTSizeX, int& OUTSizeY)
	{

	}
}