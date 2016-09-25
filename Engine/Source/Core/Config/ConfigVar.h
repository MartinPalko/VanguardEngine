#pragma once
#include "Foundation.h"
#include "ConfigTable.h"
#include "Core_Common.h"

namespace Vanguard
{
	class CORE_API ConfigVar
	{
		friend class ConfigTable;
	protected:
		String file;
		StringID section;
		StringID name;
		ConfigVar()
			: section((uint32)0)
			, name((uint32)0)
		{}
		virtual ~ConfigVar() {}
	};

	template <class T> class CORE_API ConfigVar_T : public ConfigVar
	{
		T defaultValue;

	protected:
		virtual T FromText(const String& aString) const = 0;
		virtual String ToText(const T& aTypeInstance) const = 0;

		// This needs to be called by the implementing class' constructor, otherwise the call to ToText() will try to call the pure virtual function.
		void Register()
		{			
			ConfigTable::OnConfigVarCreated(*this, ToText(defaultValue));
		}

	public:
		ConfigVar_T(const String& aConfigFile, StringID aConfigSection, StringID aName, T aDefaultValue)
		{
			file = aConfigFile;
			section = aConfigSection;
			name = aName;
			defaultValue = aDefaultValue;
		}
		virtual ~ConfigVar_T() {}

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