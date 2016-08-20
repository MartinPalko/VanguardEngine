#include "Core.h"
#include "Config/Config.h"

int main (int argc, char **argv)
{
#ifdef VANGUARD_PROJECT
	Vanguard::Core VanguardCore;
	//VanguardCore.Initialize(argc, argv, TO_STRING(VANGUARD_PROJECT));
	//VanguardCore.Run();
#endif
#if MEMORY_LEAK_DEBUG_ENABLED
	VANGUARD_DUMP_MEMORY_LEAK_DEBUG();
#endif
	return 0;

}