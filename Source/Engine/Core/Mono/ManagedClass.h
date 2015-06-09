#pragma once
#include "Foundation.h"

struct _MonoClass;

namespace Vanguard
{
	class ManagedAssembly;

	class ManagedClass
	{
		friend ManagedAssembly;
	protected:
		ManagedAssembly* assembly;
		_MonoClass* monoClass;
		String namespaceName;
		String className;
	public:
		ManagedClass(const String& aNamespace, const String& aClass, ManagedAssembly* aAssembly);
	public:
		void CallStaticMethod(const String& aMethodName);
	};
}
