#pragma once
#include "ConfigVar.h"
#include "Core_Common.h"

namespace Vanguard
{
	class StringConfigVar : public ConfigVar<String>
	{
	public:
		StringConfigVar(const String& aConfigFile, const String& aConfigSection, const String& aName, const String& aDefaultValue)
		{
			Initialize(aConfigFile, aConfigSection, aName, aDefaultValue);
		}
	private:
		virtual String FromText(const String& aString) const
		{
			return aString;
		}

		virtual String ToText(const String& aTypeInstance) const
		{
			return aTypeInstance;
		}
	};
}