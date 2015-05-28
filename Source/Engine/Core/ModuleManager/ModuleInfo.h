#pragma once

#include "Foundation.h"
#include "Core.h"

namespace Vanguard
{
	struct ModuleInfo
	{
	public:
		ModuleInfo();
		~ModuleInfo();

		//string Name;
		//string FilePath;
		bool IsLoaded;

		void* Handle;
		IModule* Module;
	};
}
