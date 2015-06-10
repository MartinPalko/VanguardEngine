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
		String_Config(monoLibPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/lib").GetFullPathName())
		String_Config(monoConfigPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/etc").GetFullPathName())

		_MonoDomain* domain;
		_MonoAssembly* assembly;
		_MonoImage* image;

		Hashtable<String, ManagedClass*> classes;

	public:
		ManagedAssembly(const String& aMonoAssemblyName);
		~ManagedAssembly();
	};
}
