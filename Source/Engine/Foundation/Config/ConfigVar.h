#pragma once
#include "IntegerDefs.h"
#include "VanguardString.h"
#include "EngineConfig.h"

namespace Vanguard
{
	template <class T> class ConfigVar
	{
	private:
		String name;
		T defaultValue;
	protected:
		virtual T FromText(const String& aString) const = 0;
		virtual String ToText(const T& aTypeInstance) const = 0;

		void Initialize(String aName, T aDefaultValue)
		{
			name = aName;
			defaultValue = aDefaultValue;
			EngineConfig::OnConfigVarCreated(aName, ToText(aDefaultValue));
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
			return FromText(EngineConfig::GetConfigValueText(name));
		}
	};

	class Int32ConfigVar : public ConfigVar<int32>
	{
	public:
		Int32ConfigVar(const String& aName, const int32& aDefaultValue)
		{ 
			Initialize(aName, aDefaultValue);
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
		FloatConfigVar(String aName, float aDefaultValue)
		{
			Initialize(aName, aDefaultValue);
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
		BooleanConfigVar(String aName, bool aDefaultValue)
		{
			Initialize(aName, aDefaultValue);
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
		StringConfigVar(String aName, String aDefaultValue)
		{
			Initialize(aName, aDefaultValue);
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
#define int32_Config(varName, defaultValue) const Vanguard::Int32ConfigVar varName = Vanguard::Int32ConfigVar(#varName,defaultValue);
#define float_Config(varName, defaultValue) const Vanguard::FloatConfigVar varName = Vanguard::FloatConfigVar(#varName,defaultValue);
#define bool_Config(varName, defaultValue) const Vanguard::BooleanConfigVar varName = Vanguard::BooleanConfigVar(#varName,defaultValue);
#define String_Config(varName, defaultValue) const Vanguard::StringConfigVar varName = Vanguard::StringConfigVar(#varName,defaultValue);