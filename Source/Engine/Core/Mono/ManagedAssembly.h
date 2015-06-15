#pragma once

#include "Foundation.h"
#include "Config.h"

struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;

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

		Hashtable<String, ManagedClass*> classes;

	public:
		ManagedAssembly(const String& aMonoAssemblyName);
		~ManagedAssembly();
	};
}
