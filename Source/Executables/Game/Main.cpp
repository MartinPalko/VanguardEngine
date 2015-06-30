#include "Core.h"

Vanguard::Core VanguardCore;

int main (int argc, char **argv)
{
	VanguardCore.Initialize(argc,argv);

	VanguardCore.LoadModule("PhysX");
	VanguardCore.LoadModule("ManagedModule");

	VanguardCore.Run();

	system("pause");
	return 0;
}

