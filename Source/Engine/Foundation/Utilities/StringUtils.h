#pragma once
#include <string>
#include <algorithm>

#include "Types.h"
#include "juce_core.h"

namespace Vanguard
{
	namespace StringUtils
	{
		extern const string TRUE_STRING;
		extern const string FALSE_STRING;

		inline string ToLower(const string& aString)
		{
			string rString = aString;
			transform(rString.begin(), rString.end(), rString.begin(), ::tolower);
			return rString;
		}

		inline string ToUpper(const string& aString)
		{
			string rString = aString;
			transform(rString.begin(), rString.end(), rString.begin(), ::toupper);
			return rString;
		}

		inline bool ToBoolean(const string& aString)
		{
			return ToLower(aString) == TRUE_STRING;
		}

		inline int ToInt(const string& aString)
		{
			return std::stoi(aString);
		}

		inline float ToFloat(const string& aString)
		{
			return std::stof(aString);
		}

		inline string FromBoolean(const bool& aBool)
		{
			return aBool ? TRUE_STRING : FALSE_STRING;
		}
		
		inline string FromInt(const int32& aInt32)
		{
			return ::std::to_string(aInt32);
		}

		inline string FromFloat(const float& aFloat)
		{
			return ::std::to_string(aFloat);
		}

		inline string FromJuceString(const juce::String& aJuceString)
		{
			return string(aJuceString.getCharPointer());
		}
	}
}