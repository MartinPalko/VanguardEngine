#pragma once
#include "ConfigVar.h"
#include "CoreMacros.h"

namespace Vanguard
{
	class Int32ConfigVar : public ConfigVar<int32>
	{
	public:
		Int32ConfigVar(const String& aConfigFile, const String& aConfigSection, const String& aName, const int32& aDefaultValue)
		{
			Initialize(aConfigFile, aConfigSection, aName, aDefaultValue);
		}
	private:
		int32 FromText(const String& aString) const
		{
			return aString.ToInt32();
		}

		String ToText(const int32& aTypeInstance) const
		{
			return String::FromInt32(aTypeInstance);
		}
	};
}