#include "Core.h"
Vanguard::Core VanguardCore;



int main()
{
	VanguardCore.Initialize();
	VanguardCore.Run();
	VanguardCore.ShutDown();

	VanguardCore.TakingFoundationType("test");
	std::cout << VanguardCore.ReturningFoundationType() << "\n" << "\n";

	system("pause");
}