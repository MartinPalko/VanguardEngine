#include "Foundation.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

namespace Vanguard
{
	class ManagedClass
	{
		friend class ManagedAssembly;
	private:
		MonoClass* monoClass;
		MonoObject* instance;
		String namespaceName;
		String className;
		ManagedClass(const String& aNamespace, const String& aClass, const ManagedAssembly* aAssembly);
	public:
		void CallMethod(const String& aMethodName);
	};
}
