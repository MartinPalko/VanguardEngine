#include "ManagedAssembly.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

#include "ManagedClass.h"
#include "Log.h"


namespace Vanguard
{
	ManagedAssembly::ManagedAssembly(const String& aMonoBinaryName)
	{
		// Working directory needs to be the bin folder when initializing mono.
		FilePath previousWorkingDirectory = FileSystem::GetCurrentWorkingDir();
		FileSystem::SetCurrentWorkingDir(FileSystem::GetEngineBinDirectory());

		Log::Write("Mono Initializing");

		mono_config_parse(NULL);
		mono_set_dirs(String(FileSystem::GetEngineBinDirectory().GetFullPathName() + "/mono/lib/").GetCharPointer(), String(FileSystem::GetEngineBinDirectory().GetFullPathName() + "/mono/lib/").GetCharPointer());

		domain = mono_jit_init_version(aMonoBinaryName.GetCharPointer(),"v4.0.1");
		assembly = mono_domain_assembly_open(domain, aMonoBinaryName.Append(".dll").GetCharPointer());

		if (!assembly)
			Log::Write("error loading assembly " + aMonoBinaryName);

		image = mono_assembly_get_image(assembly);

		if (!image)
			Log::Write("error loading image for " + aMonoBinaryName);

		// Revert working dir to what it was previously.
		FileSystem::SetCurrentWorkingDir(previousWorkingDirectory);
	}

	ManagedAssembly::~ManagedAssembly()
	{
		mono_jit_cleanup(domain);
	}
}

