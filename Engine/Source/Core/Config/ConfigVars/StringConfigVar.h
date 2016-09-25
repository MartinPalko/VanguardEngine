#pragma once
#include "../ConfigVar.h"
#include "Core_Common.h"

namespace Vanguard
{
	class CORE_API StringConfigVar : public ConfigVar_T<String>
	{
	public:
		StringConfigVar(const String& aConfigFile, StringID aConfigSection, StringID aName, String aDefaultValue)
			: ConfigVar_T<String>(aConfigFile, aConfigSection, aName, aDefaultValue)
		{
			Register();
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
