#pragma once
#include "IntegerDefs.h"
#include "VanguardString.h"
#include "EngineConfig.h"

namespace Vanguard
{
	class IConfigVar
	{
		friend class EngineConfig;
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
			EngineConfig::OnConfigVarCreated(*this, ToText(aDefaultValue));
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
			return FromText(EngineConfig::GetConfigValueText(*this));
		}
	};

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

// Macros for ease of use.
#define int32_Config(varName, configFile, defaultValue) const Vanguard::Int32ConfigVar varName = Vanguard::Int32ConfigVar(configFile,typeid(*this).name(),#varName,defaultValue);
#define float_Config(varName, configFile, defaultValue) const Vanguard::FloatConfigVar varName = Vanguard::FloatConfigVar(configFile,typeid(*this).name(),#varName,defaultValue)
#define bool_Config(varName, configFile, defaultValue) const Vanguard::BooleanConfigVar varName = Vanguard::BooleanConfigVar(configFile,typeid(*this).name(),#varName,defaultValue)
#define String_Config(varName, configFile, defaultValue) const Vanguard::StringConfigVar varName = Vanguard::StringConfigVar(configFile,typeid(*this).name(),#varName,defaultValue)