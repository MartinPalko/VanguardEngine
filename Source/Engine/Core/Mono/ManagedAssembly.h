#include "Foundation.h"
#include "Config.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

namespace Vanguard
{
	class ManagedAssembly
	{
		friend class ManagedClass;
	private:
		String_Config(monoLibPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/lib").GetFullPathName())
		String_Config(monoConfigPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/etc").GetFullPathName())

		MonoDomain* domain;
		MonoAssembly* assembly;
		MonoImage* image;

	public:
		ManagedAssembly(const String& aMonoAssemblyName);
		~ManagedAssembly();
	};
}
