#include "Core.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

int main (int argc, char **argv)
{
#ifdef VANGUARD_PROJECT
	Vanguard::Application::ShowConsoleWindow();
	Vanguard::Core VanguardCore;
	VanguardCore.Initialize(argc, argv, TOSTRING(VANGUARD_PROJECT));
	VanguardCore.Run();
#endif
	return 0;
}