#include "Core.h"
Vanguard::Core VanguardCore;



int main()
{
	VanguardCore.Initialize();
	VanguardCore.Run();
	VanguardCore.ShutDown();
	system("pause");
}