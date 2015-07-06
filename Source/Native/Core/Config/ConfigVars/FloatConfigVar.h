#pragma once
#include "ConfigVar.h"
#include "Core_Common.h"

namespace Vanguard
{
	class FloatConfigVar : public ConfigVar<float>
	{
	public:
		FloatConfigVar(const String& aConfigFile, const String& aConfigSection, const String& aName, const float& aDefaultValue)
		{
			Initialize(aConfigFile, aConfigSection, aName, aDefaultValue);
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