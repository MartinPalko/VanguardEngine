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
		FilePath filePath;
		bool isLoaded;

		void* Handle;
		IModule* Module;
	};
}
