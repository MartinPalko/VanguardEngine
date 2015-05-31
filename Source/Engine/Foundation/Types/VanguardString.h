#pragma once
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>

#include "juce_core.h"

#include "List.h"
#include "IntegerDefs.h"

namespace Vanguard
{
	class string
	{
	private:
		std::string data;

	public:
		string() {}
		string(const string& aString) { data.assign(aString.data); }

		// Comparison to other vanguard::string
		inline bool operator == (const string& aOther) const {return this->data == aOther.data; }

		// Create from and cast to std string
		string(const std::string& aSTDString){ data = aSTDString; }
		//inline operator std::string& () { return data; }

		// Create from and cast to const char*
		string(const char* aCharPointer){ data.assign(aCharPointer); }
		inline operator const char*() const { return data.c_str(); }

		// Create from and cast to juce string
		string(const juce::String& aJuceString) { data = std::string(aJuceString.getCharPointer()); }
		inline operator const juce::String() const { return juce::String(data.c_str()); }
		inline operator const juce::StringRef() const { return data.c_str(); }

		// To and from boolean
		static const string TRUE_STRING;
		static const string FALSE_STRING;
		static string FromBoolean(const bool& aBool) { return std::string(aBool ? TRUE_STRING : FALSE_STRING); }
		inline bool ToBoolean() const {	return this->ToLower() == TRUE_STRING; }

		// To and from int32
		static string FromInt32(int32 aInt32) { return ::std::to_string(aInt32); }
		inline int32 ToInt32() const { return std::stoi(data); }

		//// To and from float
		static string FromFloat(float aFloat){ return string(::std::to_string(aFloat)).TrimEnd("0").TrimEnd("."); }
		inline float ToFloat() const { return std::stof(data); }

		// Returns the number of characters in this string
		inline uint32 GetLength() const
		{
			return data.length();
		}

		// Split the string into a list of strings by the specified character.
		inline List<string> Split(const char& aSplitBy) const
		{
			List<string> splitList = List<string>();

			std::stringstream strStream = std::stringstream(data);
			std::string segment;

			while (std::getline(strStream, segment, aSplitBy))
			{
				splitList.push_back(segment);
			}

			return splitList;
		}

		// Returns a string with all instances of a specified character withplaced with another.
		inline string Replace(const char& aChar, const char& aWithChar) const
		{
			std::string newString = data;
			std:replace(newString.begin(), newString.end(), aChar, aWithChar);
			return newString;
		}

		// Returns a string with all instances of a specified string withplaced with another. 
		// If possible, use the overloaded version that takes a character, as it's much faster.
		inline string Replace(const string& aString, const string& aWithString) const
		{
			uint32 start_pos = 0;
			std::string newString = data;
			while ((start_pos = newString.find(aString, start_pos)) != std::string::npos)
			{
				newString.replace(start_pos, aString.GetLength(), aWithString);
				start_pos += aWithString.GetLength(); // Handles case where 'to' is a substring of 'from'
			}
			return newString;
		}		

		// Returns a string with all instances of the specified character removed.
		inline string Remove(const char& aChar) const
		{
			std::string newString = data;
			newString.erase(std::remove(newString.begin(), newString.end(), aChar), newString.end());
			return newString;
		}

		// Returns a string with all instances of the specified string removed.
		inline string Remove(const string& aString) const { return Replace(aString, ""); }

		// Returns a string that has all instances of all characters specified removed from the string.
		inline string RemoveCharacters(const char* aCharacters) const
		{
			std::string newString = data;
			for (unsigned int i = 0; i < strlen(aCharacters); ++i)
				newString.erase(std::remove(newString.begin(), newString.end(), aCharacters[i]), newString.end());
			return newString;
		}

		// Returns a string with leading whitespace removed.
		inline string TrimStart() const
		{
			std::string newString = data;
			newString.erase(newString.begin(), std::find_if(newString.begin(), newString.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return newString;
		}

		// Returns a string with trailing whitespace removed.
		inline string TrimEnd() const
		{
			std::string newString = data;
			newString.erase(std::find_if(newString.rbegin(), newString.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), newString.end());
			return newString;
		}

		// Returns a string with all leading and trailing whitespace removed.
		inline string Trim() const
		{
			return TrimStart().TrimEnd();
		}

		// Trims any of the specified characters from the start of the string
		inline string TrimStart(const char* aChars) const
		{
			std::string newString = data;
			uint32 newStart = newString.find_first_not_of(aChars);
			newString.erase(0, newStart);
			return newString;
		}

		// Trims any of the specified character from the start of the string
		inline string TrimStart(const char& aChar) const { return TrimStart(&aChar); }

		// Trims any of the specified characters off the end of the string
		inline string TrimEnd(const char* aChars) const
		{
			std::string newString = data;
			uint32 newLength = newString.find_last_not_of(aChars) + 1;
			newString.resize(newLength);
			return newString;
		}

		// Trims any of the specified character off the end of the string
		inline string TrimEnd(const char& aChar) const { return TrimEnd(&aChar); }

		// Trims any of the specified characters from the start and end of the string
		inline string Trim(const char* aChars) const
		{
			return TrimStart(aChars).TrimEnd(aChars);
		}

		// Trims any of the specified character from the start and end of the string
		inline string Trim(const char& aChar) const { return Trim(&aChar); }

		// Return a lowercase version of this string
		inline string ToLower() const
		{
			std::string newString = data;
			transform(newString.begin(), newString.end(), newString.begin(), ::tolower);
			return newString;
		}

		// Return an uppercase version of this string
		inline string ToUpper() const
		{
			std::string stdstring = data;
			transform(stdstring.begin(), stdstring.end(), stdstring.begin(), ::toupper);
			return stdstring;
		}
	};

	// Comparison to std::string
	inline bool operator == (const string& lhs, const std::string& rhs) { return (std::string)lhs == rhs; }
	inline bool operator == (const std::string& lhs, const string& rhs) { return (std::string)rhs == lhs; }

	// Comparison to char*
	inline bool operator == (const string& lhs, const char* rhs) { return (std::string)lhs == rhs; }
	inline bool operator == (const char* lhs, const string& rhs) { return (std::string)rhs == lhs; }

	// Comparison to juce::String
	inline bool operator == (const string& lhs, const juce::String& rhs) { return (std::string)lhs == rhs.getCharPointer(); }
	inline bool operator == (const juce::String& lhs, const string& rhs) { return (std::string)rhs == lhs.getCharPointer(); }

	// Implement osstream support
	inline ::std::ostream& operator<<(::std::ostream& os, const string& aString) { return os << (std::string)aString; }

}
