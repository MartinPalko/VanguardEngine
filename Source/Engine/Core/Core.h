#pragma once

#include "Config.h"
#include "Foundation.h"
#include "CoreMacros.h"

namespace Vanguard
{
	class ModuleManager;
	class ManagedAssembly;

	class CORE_API Core
	{
	private:
		class ModuleManager* moduleManager;
		class ManagedAssembly* managedCore;

	public:
		int i;

		void Initialize();
		void Run();
		void ShutDown();

		String ReturningFoundationType();
		void TakingFoundationType(String aArgument);
	};
}