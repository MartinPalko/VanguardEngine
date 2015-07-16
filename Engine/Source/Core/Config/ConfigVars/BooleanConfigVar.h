#pragma once
#include "../ConfigVar.h"
#include "Core_Common.h"

namespace Vanguard
{
	class BooleanConfigVar : public ConfigVar<bool>
	{
	public:
		BooleanConfigVar(const String& aConfigFile, const String& aConfigSection, const String& aName, bool aDefaultValue) 
			: ConfigVar<bool>(aConfigFile, aConfigSection, aName, aDefaultValue)
		{
			Register();
		}

	private:
		virtual bool FromText(const String& aString) const
		{
			return aString.ToBoolean();
		}

		virtual String ToText(const bool& aTypeInstance) const
		{
			return String::FromBoolean(aTypeInstance);
		}
	};
}
