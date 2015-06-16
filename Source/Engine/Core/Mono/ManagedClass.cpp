#include "ManagedClass.h"

#include "ManagedAssembly.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Vanguard
{
	ManagedClass::ManagedClass(const String& aNamespace, const String& aClass, ManagedAssembly* aAssembly)
	{
		assembly = aAssembly;
		className = aClass;
		namespaceName = aNamespace;
		monoClass = mono_class_from_name(aAssembly->image, aNamespace.GetCharPointer(), aClass.GetCharPointer());

		if (!monoClass)
			throw Exception((String("Error: Class named ") + aClass + " not found in namespace " + aNamespace + "\n\n").GetCharPointer());
	}

	void ManagedClass::CallStaticMethod(const String& aMethodName)
	{
		MonoMethodDesc* methodDescription = mono_method_desc_new((namespaceName + "." + className + ":" + aMethodName + "()").GetCharPointer(), true);
		MonoMethod* method = mono_method_desc_search_in_class(methodDescription, monoClass);
		if (method)
		{
			MonoObject* exception = nullptr;
			MonoObject* obj = mono_runtime_invoke(method, NULL, NULL, &exception);

			if (exception != nullptr)
				Log::Write(mono_string_to_utf8(mono_object_to_string(exception, NULL)));
		}
	}
}

