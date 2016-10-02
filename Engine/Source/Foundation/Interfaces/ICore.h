#pragma once
#include "Platforms/Platform.h"
#pragma warning (disable : 4275) // non dll-interface struct used as base for dll-interface class

namespace Vanguard
{
	struct ICore
	{
		virtual ~ICore() {};

		virtual void Initialize(int aArgC, char** aArgV, const char* aProjectName = "") = 0;
		virtual class World* CreateProjectWorld() = 0;
		virtual void Run() = 0;
		virtual void Quit() = 0;
		virtual void LoadModule(const char* aModuleName) = 0;
	};
}

#define VANGUARD_CREATE_CORE_FUNCTION InstantiateVanguardCore

#define VANGUARD_DECLARE_CORE_IMP(CoreImplClass)\
	extern "C" {API_EXPORT Vanguard::ICore* VANGUARD_CREATE_CORE_FUNCTION(){ return new CoreImplClass(); }}

namespace Vanguard
{
	typedef ICore* (*T_VANGUARD_CREATE_CORE_FUNCTION)();
}