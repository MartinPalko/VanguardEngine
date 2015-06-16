#include "ManagedObject.h"

#include "ManagedAssembly.h"
#include "Log.h"

namespace Vanguard
{
	ManagedObject::ManagedObject(const String& aNamespace, const String& aClass, ManagedAssembly* aAssembly)
		: ManagedClass(aNamespace, aClass, aAssembly)
	{
		monoObject = mono_object_new(aAssembly->domain, monoClass); // Creates a new object inside mono
		mono_runtime_object_init(monoObject); // Calls the constructor of the class
	}

	MonoObject* ManagedObject::CallMethod(const String& aMethodName)
	{
		MonoMethodDesc* methodDescription = mono_method_desc_new((namespaceName + "." + className + ":" + aMethodName + "()").GetCharPointer(), true);
		MonoMethod* method = mono_method_desc_search_in_class(methodDescription, monoClass);
		if (method)
		{
			MonoObject* exception = nullptr;
			MonoObject* obj = mono_runtime_invoke(method, monoObject, NULL, &exception);

			if (exception != nullptr)
				Log::Write(mono_string_to_utf8(mono_object_to_string(exception, NULL))); // Show the exception
			return obj;
		}
		return NULL;
	}

	MonoObject* ManagedObject::CallMethod(const String& aMethodName, const String& aStringArgument)
	{
		MonoMethod* method = mono_class_get_method_from_name(monoClass, aMethodName.GetCharPointer(), 1);
		if (method)
		{
			void* args[1];
			args[0] = mono_string_new(assembly->domain, aStringArgument.GetCharPointer());

			MonoObject* exception = nullptr;
			MonoObject* obj = mono_runtime_invoke(method, monoObject, args, &exception);

			if (exception != nullptr)
				Log::Write( mono_string_to_utf8(mono_object_to_string(exception, NULL)));// Show the exception

			return obj;
		}
		return NULL;
	}
}

