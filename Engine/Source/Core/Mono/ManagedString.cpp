#include "ManagedString.h"
#include "Types/VanguardString.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

namespace Vanguard
{
	String ManagedString::ToNative()
	{
		return mono_string_to_utf8((MonoString*)this);
	}
}