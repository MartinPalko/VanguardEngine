#include "Foundation.h"
#include "Config.h"
//#include <glib/glib.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Vanguard
{
	class Mono
	{
	private:
		String_Config(monoLibPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/lib").GetFullPathName())
		String_Config(monoConfigPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/etc").GetFullPathName())

		MonoDomain* domain;
		MonoAssembly* assembly;

	public:
		Mono();
		~Mono();
	};
}