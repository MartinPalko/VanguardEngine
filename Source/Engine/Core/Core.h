#pragma once

#include "Config.h"
#include "Foundation.h"
#include "CoreMacros.h"

namespace Vanguard
{
	class ModuleManager;
	class Mono;

	class CORE_API Core
	{
	private:
		class ModuleManager* moduleManager;
		class Mono* monoInstance;

	public:
		int i;

		void Initialize();
		void Run();
		void ShutDown();

		String ReturningFoundationType();
		void TakingFoundationType(String aArgument);
	};
}