#pragma once
#include "IntegerDefs.h"
#include "VanguardString.h"
#include "EngineConfig.h"

namespace Vanguard
{
	template <class T> class ConfigVar
	{
	private:
		T value;
		String name;
	protected:
		virtual T ParseConfigType(const String& aString) = 0;
		virtual String SerializeConfigType(const T& aTypeInstance) = 0;

		void Initialize(String aName, T aDefaultValue)
		{
			name = aName;
			value = ParseConfigType(EngineConfig::GetConfigString(aName, SerializeConfigType(aDefaultValue)));
		}
	public:
		void SetValue(const T& aValue)
		{
			value = aValue;
			EngineConfig::SetConfigString(name, SerializeConfigType(aValue));
		}

		// Allow to be automatically cast to it's value
		operator T() const { return value; }
	};

	class Int32ConfigVar : public ConfigVar<int32>
	{
	public:
		Int32ConfigVar(const String& aName, const int32& aDefaultValue)
		{ 
			Initialize(aName, aDefaultValue);
		}
	private:
		int32 ParseConfigType(const String& aString)
		{
			return aString.ToInt32();
		}

		String SerializeConfigType(const int32& aTypeInstance)
		{
			return String::FromInt32(aTypeInstance);
		}
	};

	//class FloatConfigVar : public ConfigVar<float>
	//{
	//public:
	//	FloatConfigVar(String aName, float aDefaultValue) : ConfigVar(aName, aDefaultValue){}
	//private:
	//	virtual float ParseConfigType(const String& aString)
	//	{
	//		return aString.ToFloat();
	//	}

	//	virtual String SerializeConfigType(const float& aTypeInstance)
	//	{
	//		return String::FromFloat(aTypeInstance);
	//	}
	//};

	//class BooleanConfigVar : public ConfigVar<bool>
	//{
	//public:
	//	BooleanConfigVar(String aName, bool aDefaultValue) : ConfigVar(aName, aDefaultValue){}
	//private:
	//	virtual bool ParseConfigType(const String& aString)
	//	{
	//		return aString.ToBoolean();
	//	}

	//	virtual String SerializeConfigType(const int& aTypeInstance)
	//	{
	//		return String::FromBoolean(aTypeInstance);
	//	}
	//};

	//class StringConfigVar : public ConfigVar<String>
	//{
	//public:
	//	StringConfigVar(String aName, String aDefaultValue) : ConfigVar(aName, aDefaultValue){}
	//private:
	//	virtual String ParseConfigType(const String& aString)
	//	{
	//		return aString;
	//	}

	//	virtual String SerializeConfigType(const String& aTypeInstance)
	//	{
	//		return aTypeInstance;
	//	}
	//};
}