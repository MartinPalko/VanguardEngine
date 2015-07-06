#pragma once
#include "Foundation.h"
#include "ManagedClass.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Vanguard
{
	class ManagedAssembly;

	class ManagedObject : public ManagedClass
	{
	private:
		MonoObject* monoObject;
	public:
		ManagedObject(const String& aNamespace, const String& aClass, ManagedAssembly* aAssembly);
	public:
		MonoObject* CallMethod(const String& aMethodName);
		MonoObject* CallMethod(const String& aMethodName, const String& aStringArgument);
	};
}
