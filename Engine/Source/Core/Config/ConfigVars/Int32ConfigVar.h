#pragma once
#include "../ConfigVar.h"
#include "Core_Common.h"

namespace Vanguard
{
	class Int32ConfigVar : public ConfigVar<int32>
	{
	public:
		Int32ConfigVar(const String& aConfigFile, const String& aConfigSection, const String& aName, int32 aDefaultValue)
			: ConfigVar<int32>(aConfigFile, aConfigSection, aName, aDefaultValue)
		{
			Register();
		}

	private:
		virtual int32 FromText(const String& aString) const
		{
			return aString.ToInt32();
		}

		virtual String ToText(const int32& aTypeInstance) const
		{
			return String::FromInt32(aTypeInstance);
		}
	};
}
