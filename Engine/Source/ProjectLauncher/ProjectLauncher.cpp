#include "Core.h"

int main (int argc, char **argv)
{
#ifdef VANGUARD_PROJECT
	Vanguard::Core VanguardCore;
	VanguardCore.Initialize(argc, argv, TO_STRING(VANGUARD_PROJECT));
	VanguardCore.Run();
#endif
	return 0;
}