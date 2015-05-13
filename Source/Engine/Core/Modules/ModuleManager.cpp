#include "ModuleManager.h"

namespace Vanguard
{
	ModuleManager::ModuleManager()
	{
	}


	ModuleManager::~ModuleManager()
	{
	}

	ModuleManager& ModuleManager::Get()
	{
		static ModuleManager* singleton = NULL;

		if (singleton == NULL)
		{
			singleton = new ModuleManager;
		}

		return *singleton;
	}
}
