#include "Mono.h"

//#include <glib/glib.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Vanguard
{
    static MonoDomain* domain;
    static MonoAssembly* assembly;

	Mono::Mono()
	{
        std::cout << "Mono Initializing";

		std::cout << "Mono Lib Path: " << monoLibPath << "\n";
		//mono_set_dirs(String(monoLibPath).GetCharPointer(), String(monoConfigPath).GetCharPointer());



		domain = mono_jit_init("Vanguard");
		assembly = mono_domain_assembly_open(domain, "ScriptCore.dll");

		if (!assembly)
			std::cout << "error loading assembly\n";
		//mono_jit_exec(domain, assembly, __argc - 1, __argv + 1);
	}

	Mono::~Mono()
	{
	}
}

