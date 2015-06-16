#pragma once

#include "Core.h"

namespace Vanguard
{
	class Frame;

	class IModule
	{
	public:
		virtual ~IModule(){}
		virtual void LoadModule(){ Log::Write("Loaded Native Module: " + GetModuleName()); }
		virtual void UnloadModule(){ Log::Write("Unloaded Native Module: " + GetModuleName()); }

		virtual void OnFrame(Frame* aFrame){};

		virtual String GetModuleName() = 0;
		virtual String GetModuleType() = 0;
	};
}

#define IMPLEMENT_MODULE(ModuleClass, ModuleName)