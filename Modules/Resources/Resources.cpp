#include "Resources.h"

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(VanguardSDL)

	void VanguardSDL::LoadModule()
	{
		resourceManager = new ResourceManager();
		Core::GetInstance()->RegisterResourceManager(resourceManager);
	}

	void VanguardSDL::UnloadModule()
	{
		Core::GetInstance()->UnregisterResourceManager();
		delete resourceManager;
	}
}