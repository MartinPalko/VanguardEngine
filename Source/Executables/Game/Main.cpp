#include "Core.h"

Vanguard::Core VanguardCore;

int main (int argc, char **argv)
{
	VanguardCore.Initialize();

	VanguardCore.LoadModule("PhysX");
	VanguardCore.LoadModule("ManagedModule");

	VanguardCore.Run();

	system("pause");
	return 0;
}

