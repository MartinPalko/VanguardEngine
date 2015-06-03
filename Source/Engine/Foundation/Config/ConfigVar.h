#pragma once
#include "IntegerDefs.h"
#include "VanguardString.h"
#include "ConfigTable.h"

namespace Vanguard
{
	class IConfigVar
	{
		friend class ConfigTable;
	protected:
		String file;
		String section;
		String name;
	};

	template <class T> class ConfigVar : IConfigVar
	{
		T defaultValue;
	protected:
		virtual T FromText(const String& aString) const = 0;
		virtual String ToText(const T& aTypeInstance) const = 0;

		void Initialize(const String& aConfigFile, const String& aConfigSection, const String& aName, T aDefaultValue)
		{
			file = aConfigFile;
			section = aConfigSection;
			name = aName;
			defaultValue = aDefaultValue;
			ConfigTable::OnConfigVarCreated(*this, ToText(aDefaultValue));
		}
	public:

		T GetDefault()
		{
			return defaultValue;
		}

		// Allow to be automatically cast to it's value
		operator T() const
		{
			// TODO: Cache the value somewhere so it doesn't have to be de-serialized every time it's used.
			return FromText(ConfigTable::GetConfigValueText(*this));
		}
	};
}