#include "EmptyProject.h"

namespace EmptyProject
{
	VANGUARD_DECLARE_MODULE(EmptyProject);

	void EmptyProject::LoadModule()
	{

	}

	void EmptyProject::UnloadModule()
	{

	}

	World* EmptyProject::CreateWorld()
	{
		// Return a new instance of your game world here.
		return nullptr;
	}
}