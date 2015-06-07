#pragma once

#include "Core.h"
namespace Vanguard
{
	class IModule
	{
	public:
		virtual ~IModule(){}

		virtual void LoadModule(){}
		virtual void UnloadModule(){}

		virtual String GetPluginName() = 0;
		virtual String GetPluginType() = 0;
	};
}

#define IMPLEMENT_MODULE(ModuleClass, ModuleName)