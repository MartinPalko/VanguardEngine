#pragma once
#include "Foundation.h"
#include "ManagedClass.h"

struct _MonoObject;

namespace Vanguard
{
	class ManagedAssembly;

	class ManagedObject : public ManagedClass
	{
	private:
		_MonoObject* monoObject;
	public:
		ManagedObject(const String& aNamespace, const String& aClass, ManagedAssembly* aAssembly);
	public:
		_MonoObject* CallMethod(const String& aMethodName);
		_MonoObject* CallMethod(const String& aMethodName, const String& aStringArgument);
	};
}
