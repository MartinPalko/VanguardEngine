#pragma once

#include "Foundation.h"
#include "ModuleManager.h"
#include "CoreMacros.h"

namespace Vanguard
{
	class CORE_API Core
	{
	private:
		ModuleManager moduleManager;

	public:
		int i;

		void Initialize();
		void Run();
		void ShutDown();

		String ReturningFoundationType();
		void TakingFoundationType(String aArgument);
	};
}