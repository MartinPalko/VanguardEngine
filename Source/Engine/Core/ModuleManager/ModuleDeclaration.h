#pragma once

#include "ModuleInfo.h"

#define VANGUARD_DECLARE_MODULE(ModuleClass)\
	extern "C"{API_EXPORT Vanguard::IModule* InstantiateVanguardModule(){return new ModuleClass;}}