#include "ManagedModule.h"

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
		MonoObject* returnValue = managedObject->CallMethod("GetModuleName");
		const char* result = (const char*)mono_object_unbox(returnValue);
		return result;
	}

	String ManagedModule::GetModuleType()
	{
		MonoObject* returnValue = managedObject->CallMethod("GetModuleType");
		const char* result = (const char*)mono_object_unbox(returnValue);
		return result;
	}
}
