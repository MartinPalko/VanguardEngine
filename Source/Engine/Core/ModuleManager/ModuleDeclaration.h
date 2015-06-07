#pragma once

#include "IModule.h"

#define VANGUARD_DECLARE_MODULE(ModuleClass)\
	extern "C"{API_EXPORT Vanguard::IModule* InstantiateVanguardModule(){return new ModuleClass;}}
