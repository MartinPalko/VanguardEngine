#include "ManagedAssembly.h"

#include "ManagedClass.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

namespace Vanguard
{
	ManagedAssembly::ManagedAssembly(const String& aMonoBinaryName)
	{
		// Working directory needs to be the bin folder when initializing mono.
		FilePath previousWorkingDirectory = FileSystem::GetCurrentWorkingDir();
		FileSystem::SetCurrentWorkingDir(FileSystem::GetEngineBinDirectory());

        std::cout << "Mono Initializing" << "\n";
		std::cout << "Mono Lib Path: " << monoLibPath << "\n";

		mono_config_parse(NULL);
		//mono_set_dirs(String(monoLibPath).GetCharPointer(), String(monoConfigPath).GetCharPointer());

		domain = mono_jit_init(aMonoBinaryName);
		assembly = mono_domain_assembly_open(domain, aMonoBinaryName.Append(".dll"));

		if (!assembly)
			std::cout << "error loading assembly " << aMonoBinaryName << "\n";

		image = mono_assembly_get_image(assembly);

		if (!image)
			std::cout << "error loading image for " << aMonoBinaryName << "\n";

		// Revert working dir to what it was previously.
		FileSystem::SetCurrentWorkingDir(previousWorkingDirectory);
	}

	ManagedAssembly::~ManagedAssembly()
	{
		mono_jit_cleanup(domain);
	}
}

