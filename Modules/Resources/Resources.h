#pragma once
#include "Resources_Common.h"
#include "Interfaces/IModule.h"
#include "ResourceManager.h"

namespace Vanguard
{
	class RESOURCES_API Resources : public IModule
	{
		ResourceManager* resourceManager;

	public:

		// Implement IModule
		virtual void LoadModule();
		virtual void UnloadModule();
	};
}