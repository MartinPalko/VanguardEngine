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

	class CORE_API Application
	{
		friend Core;
	private:
		static ApplicationArguments applicationArguments;
	public:
		static ApplicationArguments GetApplicationArguments();

		static void ShowConsoleWindow();
		static void HideConsoleWindow();
	};
}