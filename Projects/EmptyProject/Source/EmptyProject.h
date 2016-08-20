#pragma once
#include "EmptyProject_Common.h"
#include "Interfaces/IModule.h"

namespace EmptyProject
{
	class EmptyProject : public IModule
	{
		virtual String GetModuleName() { return "EmptyProject_Native"; }
		virtual String GetModuleType() { return "Project"; }
		
		virtual void LoadModule()
		{
			DEBUG_LOG("Loaded Empty project native module!");
		}

		virtual void UnloadModule()
		{
			DEBUG_LOG("Unloaded Empty project native module!");
		}
	};
}