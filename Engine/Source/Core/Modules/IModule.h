#pragma once
#include "Core.h"
#include "Jobs/Frame.h"

namespace Vanguard
{
	class IModule
	{
	public:
		virtual ~IModule(){}
		virtual void LoadModule(){ }
		virtual void UnloadModule(){ }
	};
}

// Define the module's extern function names here, so they can be
// implemented (as a symbol) and then loaded at runtime (from a char*)
// with the same definition. using the TO_STRING() macro. This ensures
// the implemented function name and the one being searched for at
// runtime always match!
#define VANGUARD_MODULE_INST_FUNCTION InstantiateVanguardModule
#define VANGUARD_MODULE_NAME_FUNCTION GetVanguardModuleName
#define VANGUARD_MODULE_DEPENDENCY_FUNCTION GetVanguardModuleDependencies

// Macro to implement module's extern functions.
#define VANGUARD_DECLARE_MODULE(ModuleClass)\
	extern "C"{API_EXPORT Vanguard::IModule* VANGUARD_MODULE_INST_FUNCTION(){ return new ModuleClass; }}\
	extern "C"{API_EXPORT Vanguard::String VANGUARD_MODULE_NAME_FUNCTION(){ return TO_STRING(VANGUARD_LIB_NAME); }}\
	extern "C"{API_EXPORT Vanguard::String VANGUARD_MODULE_DEPENDENCY_FUNCTION() { return TO_STRING(VANGUARD_LIB_DEPENDENCIES); }}

namespace Vanguard
{
	// Typedefs for extern functions, used when finding them in the library at runtime.
	typedef IModule * (*T_VANGUARD_MODULE_INST_FUNCTION)();
	typedef String(*T_VANGUARD_MODULE_NAME_FUNCTION)();
	typedef String(*T_VANGUARD_MODULE_DEPENDENCY_FUNCTION)();
}