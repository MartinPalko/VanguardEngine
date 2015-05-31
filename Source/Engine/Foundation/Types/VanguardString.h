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

		inline void operator += (const string& aOther) { data = Append(aOther).data; }

		// Access character by index
		inline char operator[](int32 aIndex) const
		{
#if VANGUARD_DEBUG
			// Guard aginst out of range
			if (aIndex < 0 || aIndex >= GetLength())
				throw std::invalid_argument("String index out of range");
#endif
			return data[aIndex]; 
		}

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
		inline int32 GetLength() const
		{
			return data.length();
		}

		inline bool IsEmpty() const
		{
			return Trim().GetLength() == 0;
		}

		// Returns a string with the specified string appended to the end of it.
		inline string Append(const string& aString) const
		{
			std::string newString = data;
			newString.append(aString.data);
			return newString;
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

		// Joins an array of strings into a single string, with the specified seperator between each element.
		static string Join(const List<string>& aStringList, const char& aSeperatorCharacter)
		{
			if (aStringList.size() <= 0)
				return string();

			string newString = aStringList[0];

			for (uint32 i = 1; i < aStringList.size(); i++)
			{
				newString += ";";
				newString += aStringList[i];
			}
			return newString;
		}

		// Returns true if the string contains at least one instance of the specified substring
		inline bool Contains(const string& aString) const
		{
			return !(data.find(aString) == std::string::npos);
		}

		// Returns true if the string contains at least one instance of the specified character
		inline bool Contains(const char& aCharacter) const
		{
			return !(data.find(aCharacter) == std::string::npos);
		}

		// Returns true if the string contains at least one instance of any of the specified characters
		inline bool ContainsAny(const string& aCharacters) const
		{
			for (int i = 0; i < aCharacters.GetLength(); i++)
			{
				if (!(data.find(aCharacters[i]) == std::string::npos))
					return true;
			}
			return false;
		}

		// Returns the index of the first of the specified character
		inline int32 FirstIndexOf(const char& aCharacter) const
		{
			return data.find_first_of(aCharacter);
		}

		// Returns the index of the first of any of the specified characters
		inline int32 FirstIndexOfAny(const string& aCharacters) const
		{
			return data.find_first_of(aCharacters);
		}

		// Returns the index of the last of the specified character
		inline int32 LastIndexOf(const char& aCharacter) const
		{
			return data.find_last_of(aCharacter);
		}

		// Returns the index of the last of any of the specified characters
		inline int32 LastIndexOfAny(const string& aCharacters) const
		{
			return data.find_last_of(aCharacters);
		}

		// Returns the index of the first character not to match the specified character
		inline int32 FirstIndexNotOf(const char& aCharacter) const
		{
			return data.find_first_not_of(aCharacter);
		}

		// Returns the index of the first character not to match the and of the specified characters
		inline int32 FirstIndexNotOfAny(const string& aCharacters) const
		{
			return data.find_first_not_of(aCharacters);
		}

		// Returns the index of the last character not to match the specified character
		inline int32 LastIndexNotOf(const char& aCharacter) const
		{
			return data.find_last_not_of(aCharacter);
		}

		// Returns the index of the last character not to match the and of the specified characters
		inline int32 LastIndexNotOfAny(const string& aCharacters) const
		{
			return data.find_last_not_of(aCharacters);
		}

		// Returns a string with all instances of a specified character withplaced with another.
		inline string Replace(const char& aChar, const char& aWithChar) const
		{
			std::string newString = data;
			std::replace(newString.begin(), newString.end(), aChar, aWithChar);
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
				start_pos += aWithString.GetLength(); // Handles case where 'aWithString' is a substring of 'aString'
			}
			return newString;
		}

		// Returns a string with the given string insterted starting at the given index.
		inline string Insert(const int32& aAtIndex, const string& aStringToInsert) const
		{
			std::string newString = data;
			newString.insert(aAtIndex, aStringToInsert);
			return newString;
		}

		// Returns a string with the given string insterted starting at the given index.
		inline string Remove(const int32& aAtIndex, const int32& aCharactersToRemove) const
		{
			std::string newString = data;
			newString.erase(aAtIndex,aCharactersToRemove);
			return newString;
		}

		// Returns a string with all characters between the two indexes removed.
		inline string RemoveBetween(const int32& aFirstIndex, const int32& aSecondIndex) const
		{
			std::string newString = data;
			if (aFirstIndex < aSecondIndex)
				newString.erase(aFirstIndex, aSecondIndex - aFirstIndex);
			else if (aSecondIndex < aFirstIndex)
				newString.erase(aSecondIndex, aFirstIndex - aSecondIndex);
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
		inline string Remove(const string& aString) const
		{
			return Replace(aString, ""); 
		}

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

	// Addition
	inline string operator+ (const string& lhs, const string& rhs) { return lhs.Append(rhs); }

	// Comparison to char*
	inline bool operator == (const string& lhs, const char* rhs) { return (std::string)lhs == rhs; }
	inline bool operator == (const char* lhs, const string& rhs) { return (std::string)rhs == lhs; }

	// Comparison to juce::String
	inline bool operator == (const string& lhs, const juce::String& rhs) { return (std::string)lhs == rhs.getCharPointer(); }
	inline bool operator == (const juce::String& lhs, const string& rhs) { return (std::string)rhs == lhs.getCharPointer(); }

	// Implement osstream support
	inline ::std::ostream& operator<<(::std::ostream& os, const string& aString) { return os << (std::string)aString; }

}
