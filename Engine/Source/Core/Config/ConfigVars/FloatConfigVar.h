#pragma once
#include "../ConfigVar.h"
#include "Core_Common.h"

namespace Vanguard
{
	class CORE_API FloatConfigVar : public ConfigVar_T<float>
	{
	public:
		FloatConfigVar(const String& aConfigFile, const String& aConfigSection, const String& aName, float aDefaultValue)
			: ConfigVar_T<float>(aConfigFile, aConfigSection, aName, aDefaultValue)
		{
			Register();
		}

	private:
		virtual float FromText(const String& aString) const
		{
			return aString.ToFloat();
		}

		virtual String ToText(const float& aTypeInstance) const
		{
			return String::FromFloat(aTypeInstance);
		}
	};

}
