#include "Resources.h"

namespace Vanguard
{
	VANGUARD_DECLARE_MODULE(Resources)

	void Resources::LoadModule()
	{
		resourceManager = new ResourceManager();
		Core::GetInstance()->RegisterResourceManager(resourceManager);
	}

	void Resources::UnloadModule()
	{
		Core::GetInstance()->UnregisterResourceManager();
		delete resourceManager;
	}
}