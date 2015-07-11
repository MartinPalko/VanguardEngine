#pragma once

#include "Foundation.h"
#include "Config.h"

struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;
class _MonoString;

namespace Vanguard
{
    class ManagedClass;
    class ManagedObject;

	class ManagedAssembly
	{
		friend ManagedClass;
		friend ManagedObject;
	private:
		_MonoDomain* domain;
		_MonoAssembly* assembly;
		_MonoImage* image;

		Hashtable<ManagedClass*> classes;

	public:
		ManagedAssembly(const String& aMonoAssemblyName);
		~ManagedAssembly();

		void AddInternalCall(String aManagedFunction, const void* aNativeFunction)
		{
			mono_add_internal_call(aManagedFunction.GetCharPointer(), aNativeFunction);			
		}
	};
}
