#pragma once
#include "Core.h"
#include "Frame.h"

namespace Vanguard
{
	class IModule
	{
	public:
		virtual ~IModule(){}
		virtual void LoadModule(){ }
		virtual void UnloadModule(){ }

		virtual void OnFrame(Frame* aFrame){};

		virtual String GetModuleName() = 0;
		virtual String GetModuleType() = 0;
	};
}

#define IMPLEMENT_MODULE(ModuleClass, ModuleName)