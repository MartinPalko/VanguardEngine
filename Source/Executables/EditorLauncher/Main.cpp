#include "Core.h"

int main (int argc, char **argv)
{
	Vanguard::Application::ShowConsoleWindow();

	Vanguard::Core VanguardCore;

	VanguardCore.Initialize(argc, argv);
	
	VanguardCore.LoadModule("EditorUI");
	VanguardCore.LoadModule("PhysX");

	//VanguardCore.LoadModule("Juce_EditorUIRenderer");
	VanguardCore.LoadModule("Qt_EditorUI");

	VanguardCore.Run();

	return 0;
}

