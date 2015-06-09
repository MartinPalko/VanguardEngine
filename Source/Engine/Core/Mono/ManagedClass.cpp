#include "ManagedClass.h"

#include "ManagedAssembly.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

namespace Vanguard
{
	ManagedClass::ManagedClass(const String& aNamespace, const String& aClass, const ManagedAssembly* aAssembly)
	{
		className = aClass;
		namespaceName = aNamespace;
		monoClass = mono_class_from_name(aAssembly->image, aNamespace, aClass);

		if (!monoClass)
			std::cout << "Error: Class named " << aClass << " not found in namespace " << aNamespace << "\n\n";

		instance = mono_object_new(aAssembly->domain, monoClass); // Creates a new object inside mono
		mono_runtime_object_init(instance); // Calls the constructor of the class
	}

	void ManagedClass::CallMethod(const String& aMethodName)
	{
		MonoMethodDesc* methodDescription = mono_method_desc_new(namespaceName + "." + className + ":" + aMethodName + "()", true);
		MonoMethod* method = mono_method_desc_search_in_class(methodDescription, monoClass);
		if (method) 
		{
			MonoObject* exception = nullptr;
			MonoObject* obj = mono_runtime_invoke(method, instance, NULL, &exception);

			if (exception != nullptr)
				std::cout << mono_string_to_utf8(mono_object_to_string(exception, NULL)); // Show the exception
		}
	}
}

