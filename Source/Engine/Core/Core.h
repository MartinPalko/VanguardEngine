#pragma once

#include "Foundation.h"
#include "CoreMacros.h"


namespace Vanguard
{
	class CORE_API ModuleManager;

	class CORE_API Core
	{
	private:
		class ModuleManager* moduleManager;

	public:
		int i;

		void Initialize();
		void Run();
		void ShutDown();

		String ReturningFoundationType();
		void TakingFoundationType(String aArgument);
	};
}