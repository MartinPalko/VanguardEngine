#pragma once

#include "Foundation.h"
#include "Config/Config.h"

struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;
struct _MonoString;

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

		void AddInternalCall(String aManagedFunction, const void* aNativeFunction);
	};
}
