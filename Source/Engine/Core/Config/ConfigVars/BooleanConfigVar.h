#pragma once
#include "ConfigVar.h"
#include "CoreMacros.h"

namespace Vanguard
{	
	class BooleanConfigVar : public ConfigVar<bool>
	{
	public:
		BooleanConfigVar(const String& aConfigFile, const String& aConfigSection, const String& aName, const bool& aDefaultValue)
		{
			Initialize(aConfigFile, aConfigSection, aName, aDefaultValue);
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