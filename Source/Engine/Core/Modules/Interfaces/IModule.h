#pragma once

#include "Core.h"
namespace Vanguard
{
	class IModule
	{
	public:
		virtual ~IModule(){}

		virtual void LoadModule(){ std::cout << "Loaded Native Module: " << GetModuleName() << "\n"; }
		virtual void UnloadModule(){ std::cout << "Unloaded Native Module: " << GetModuleName() << "\n"; }

		virtual String GetModuleName() = 0;
		virtual String GetModuleType() = 0;
	};
}

#define IMPLEMENT_MODULE(ModuleClass, ModuleName)