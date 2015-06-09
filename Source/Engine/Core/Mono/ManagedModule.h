#pragma once

#include "IModule.h"
#include "ManagedObject.h"

namespace Vanguard
{
	class ManagedModule : public IModule
	{
	private:
		ManagedObject* managedObject;

	public:
		ManagedModule(ManagedObject* aManagedObject);
		virtual ~ManagedModule(){}

		virtual void LoadModule();
		virtual void UnloadModule();

		virtual String GetModuleName();
		virtual String GetModuleType();

		
	};
}