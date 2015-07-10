#include "Core.h"

int main (int argc, char **argv)
{
	Vanguard::Application::ShowConsoleWindow();
	Vanguard::Core VanguardCore;
	VanguardCore.Initialize(argc, argv, VANGUARD_PROJECT);
	VanguardCore.Run();

	return 0;
}