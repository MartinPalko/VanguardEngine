#pragma once

#include "Core.h"
namespace Vanguard
{
	class Frame;

	class IModule
	{
	public:
		virtual ~IModule(){}
		virtual void LoadModule(){ std::cout << "Loaded Native Module: " << GetModuleName() << "\n"; }
		virtual void UnloadModule(){ std::cout << "Unloaded Native Module: " << GetModuleName() << "\n"; }

		virtual void OnPreFrame(Frame* aFrame){};
		virtual void OnFrame(Frame* aFrame){};
		virtual void OnPostFrame(Frame* aFrame){};

		virtual String GetModuleName() = 0;
		virtual String GetModuleType() = 0;
	};
}

#define IMPLEMENT_MODULE(ModuleClass, ModuleName)