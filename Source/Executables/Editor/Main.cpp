#include "Core.h"

Vanguard::Core* VanguardCore = nullptr;

int VanguardMain()
{
	VanguardCore->Initialize();

	VanguardCore->LoadModule("Editor");

	VanguardCore->Run();

	VanguardCore->ShutDown();
	//system("pause");
	return 0;
}

#ifdef VANGUARD_WINDOWS

#include "Windows.h"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	VanguardCore = new Vanguard::Core();
	return VanguardMain();
}

#else

int main (int argc, char **argv)
{
	VanguardCore = new Vanguard::Core();
	return VanguardMain();
}


#endif

