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
	};
}

#define IMPLEMENT_MODULE(ModuleClass, ModuleName)