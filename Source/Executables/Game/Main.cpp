#include "Core.h"
Vanguard::Core VanguardCore;

int main()
{
	VanguardCore.Initialize();

	VanguardCore.LoadModule("PhysX");
	VanguardCore.LoadModule("ManagedModule");

	VanguardCore.Run();

	VanguardCore.ShutDown();
	system("pause");
}