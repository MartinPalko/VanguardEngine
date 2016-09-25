#pragma once
#include "../ConfigVar.h"
#include "Core_Common.h"

namespace Vanguard
{
	class CORE_API BooleanConfigVar : public ConfigVar_T<bool>
	{
	public:
		BooleanConfigVar(const String& aConfigFile, StringID aConfigSection, StringID aName, bool aDefaultValue) 
			: ConfigVar_T<bool>(aConfigFile, aConfigSection, aName, aDefaultValue)
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
