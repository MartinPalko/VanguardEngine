#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "Modules/IModule.h"
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
