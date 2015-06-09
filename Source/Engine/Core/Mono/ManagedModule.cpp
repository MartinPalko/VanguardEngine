#include "ManagedModule.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Vanguard
{
	ManagedModule::ManagedModule(ManagedObject* aManagedObject)
	{
		managedObject = aManagedObject;
	}

	void ManagedModule::LoadModule(){}
	void ManagedModule::UnloadModule(){}

	String ManagedModule::GetModuleName()
	{ 
		_MonoObject* returnValue = managedObject->CallMethod("GetModuleName");
		const char* result = (const char*)mono_object_unbox(returnValue);
		return result;
	}

	String ManagedModule::GetModuleType()
	{ 
		_MonoObject* returnValue = managedObject->CallMethod("GetModuleType");
		const char* result = (const char*)mono_object_unbox(returnValue);
		return result;
	}
}