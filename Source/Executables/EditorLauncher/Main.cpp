#include "Core.h"

Vanguard::Core* VanguardCore = nullptr;

int VanguardMain()
{
	VanguardCore = new Vanguard::Core();

	VanguardCore->Initialize();

	VanguardCore->LoadModule("Editor_UI");

	VanguardCore->Run();

	VanguardCore->ShutDown();

	delete VanguardCore;
	//system("pause");
	return 0;
}

#ifdef VANGUARD_WINDOWS

#include "Windows.h"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return VanguardMain();
}

#else

int main (int argc, char **argv)
{
	return VanguardMain();
}


#endif

