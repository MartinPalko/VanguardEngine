#pragma once
#include "Resources_Common.h"
#include "Interfaces/IModule.h"
#include "ResourceManager.h"

namespace Vanguard
{
	class RESOURCES_API VanguardSDL : public IModule
	{
		ResourceManager* resourceManager;

	public:

		// Implement IModule
		virtual void LoadModule();
		virtual void UnloadModule();
	};
}