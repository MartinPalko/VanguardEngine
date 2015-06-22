#include "Core.h"

int VanguardMain()
{
	Vanguard::Application::ShowConsoleWindow();

	Vanguard::Core VanguardCore;

	VanguardCore.Initialize();

	VanguardCore.LoadModule("Juce_EditorUIRenderer");
	VanguardCore.LoadModule("EditorUI");
	VanguardCore.LoadModule("PhysX");

	VanguardCore.Run();

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

