#include "Foundation.h"
#include "Config.h"

namespace Vanguard
{
	class Mono
	{
	private:
		String_Config(monoLibPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/lib").GetFullPathName())
		String_Config(monoConfigPath, "Core", (FileSystem::GetSystemProgramsDirectory() + "/Mono/etc").GetFullPathName())

	public:
		Mono();
		~Mono();
	};
}
