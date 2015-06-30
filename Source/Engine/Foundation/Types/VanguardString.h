#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "juce_core.h"

#include "VanguardList.h"
#include "IntegerDefs.h"

namespace Vanguard
{
	class String
	{
	private:
		std::string data;

	public:
		String() {}
		String(const String& aString) { data.assign(aString.data); }

		// Comparison to other vanguard::string
		inline bool operator == (const String& aOther) const { return data == aOther.data; }
		inline bool operator != (const String& aOther) const { return data != aOther.data; }
		inline bool operator < (const String& aOther) const { return data < aOther.data; }
		inline bool operator <= (const String& aOther) const { return data <= aOther.data; }
		inline bool operator > (const String& aOther) const { return data > aOther.data; }
		inline bool operator >= (const String& aOther) const { return data >= aOther.data; }

		// Create from and cast to std string
		String(const std::string& aSTDString){ data = aSTDString; }
		inline operator const std::string& () const { return data; }

		// Create from and cast to const char*
		String(const char* aCharPointer){ data.assign(aCharPointer); }
		inline const char* GetCharPointer() const { return data.c_str(); }

		// Create from single char
		String(const char& aChar) { data = aChar; }

		// Create from and cast to juce string
		String(const juce::String& aJuceString) { data = std::string(aJuceString.getCharPointer()); }
		inline operator const juce::String() const { return juce::String(data.c_str()); }
		inline operator const juce::StringRef() const { return data.c_str(); }

		inline void operator += (const String& aOther) { data = Append(aOther).data; }

		// Access character by index
		inline char operator[](size_t aIndex) const
		{
#if VANGUARD_DEBUG
			// Guard aginst out of range
			if (aIndex < 0 || aIndex >= GetLength())
				throw std::invalid_argument("String index out of range");
#endif
			return data[aIndex];
		}

		// To and from boolean
		static const String TRUE_STRING;
		static const String FALSE_STRING;
		static String FromBoolean(const bool& aBool) { return aBool ? TRUE_STRING : FALSE_STRING; }
		inline bool ToBoolean() const {	return this->ToLower() == TRUE_STRING; }

		// To and from int32
		static String FromInt32(int32 aInt32) { return ::std::to_string(aInt32); }
		inline int32 ToInt32() const
		{
			return std::stoi(data);
		}

		//// To and from float
		static String FromFloat(float aFloat){ return String(::std::to_string(aFloat)).TrimEnd("0").TrimEnd("."); }
		inline float ToFloat() const { return std::stof(data); }

		// Returns the number of characters in this string
		inline size_t GetLength() const
		{
			return data.length();
		}

		inline bool IsEmpty() const
		{
			return Trim().GetLength() == 0;
		}

		// Returns a string with the specified string appended to the end of it.
		inline String Append(const String& aString) const
		{
			std::string newString = data;
			newString.append(aString.data);
			return newString;
		}

		// Split the string into a list of strings by the specified character.
		inline List<String> Split(const char& aSplitBy) const
		{
			List<String> splitList = List<String>();

			std::stringstream strStream(data);
			std::string segment;

			while (std::getline(strStream, segment, aSplitBy))
			{
				splitList.PushBack(segment);
			}

			return splitList;
		}

		// Joins an array of strings into a single string, with the specified seperator between each element.
		static String Join(const List<String>& aStringList, const char& aSeperatorCharacter)
		{
			if (aStringList.Size() <= 0)
				return String();

			String newString = aStringList[0];

			for (uint32 i = 1; i < aStringList.Size(); i++)
			{
				newString += ";";
				newString += aStringList[i];
			}
			return newString;
		}

		// Returns true if the string contains at least one instance of the specified substring
		inline bool Contains(const String& aString) const
		{
			return !(data.find(aString.GetCharPointer()) == std::string::npos);
		}

		// Returns true if the string contains at least one instance of the specified character
		inline bool Contains(const char& aCharacter) const
		{
			return !(data.find(aCharacter) == std::string::npos);
		}

		// Returns true if the string contains at least one instance of any of the specified characters
		inline bool ContainsAny(const String& aCharacters) const
		{
			for (size_t i = 0; i < aCharacters.GetLength(); i++)
			{
				if (!(data.find(aCharacters[i]) == std::string::npos))
					return true;
			}
			return false;
		}

		// Returns true if the first character of the string is the specified character
		inline bool BeginsWith(const char& aCharacter) const
		{
			return FirstIndexOf(aCharacter) == 0;
		}

		// Returns true if the first character of the string is any of the specified characters
		inline bool BeginsWithAny(const String& aCharacters) const
		{
			return FirstIndexOfAny(aCharacters) == 0;
		}

		// Returrns true if the last character of the string is the specified character
		inline bool EndsWith(const char& aCharacter) const
		{
			return LastIndexOf(aCharacter) == GetLength() - 1;
		}

		// Returns true if the first character of the string is any of the specified characters
		inline bool EndsWithAny(const String& aCharacters) const
		{
			return LastIndexOfAny(aCharacters) == GetLength() - 1;
		}

		// Returns the index of the first of the specified character
		inline size_t FirstIndexOf(const char& aCharacter) const
		{
			return data.find_first_of(aCharacter);
		}

		// Returns the index of the first of any of the specified characters
		inline size_t FirstIndexOfAny(const String& aCharacters) const
		{
			return data.find_first_of(aCharacters.GetCharPointer());
		}

		// Returns the index of the last of the specified character
		inline size_t LastIndexOf(const char& aCharacter) const
		{
			return data.find_last_of(aCharacter);
		}

		// Returns the index of the last of any of the specified characters
		inline size_t LastIndexOfAny(const String& aCharacters) const
		{
			return data.find_last_of(aCharacters.GetCharPointer());
		}

		// Returns the index of the first character not to match the specified character
		inline size_t FirstIndexNotOf(const char& aCharacter) const
		{
			return data.find_first_not_of(aCharacter);
		}

		// Returns the index of the first character not to match the and of the specified characters
		inline size_t FirstIndexNotOfAny(const String& aCharacters) const
		{
			return data.find_first_not_of(aCharacters.GetCharPointer());
		}

		// Returns the index of the last character not to match the specified character
		inline size_t LastIndexNotOf(const char& aCharacter) const
		{
			return data.find_last_not_of(aCharacter);
		}

		// Returns the index of the last character not to match the and of the specified characters
		inline size_t LastIndexNotOfAny(const String& aCharacters) const
		{
			return data.find_last_not_of(aCharacters.GetCharPointer());
		}

		// Returns a string with all instances of a specified character withplaced with another.
		inline String Replace(const char& aChar, const char& aWithChar) const
		{
			std::string newString = data;
			std::replace(newString.begin(), newString.end(), aChar, aWithChar);
			return newString;
		}

		// Returns a string with all instances of a specified string withplaced with another.
		// If possible, use the overloaded version that takes a character, as it's much faster.
		inline String Replace(const String& aString, const String& aWithString) const
		{
			size_t start_pos = 0;
			std::string newString = data;
			while ((start_pos = newString.find(aString.GetCharPointer(), start_pos)) != std::string::npos)
			{
				newString.replace(start_pos, aString.GetLength(), aWithString.GetCharPointer());
				start_pos += aWithString.GetLength(); // Handles case where 'aWithString' is a substring of 'aString'
			}
			return newString;
		}

		// Returns a string with the given string insterted starting at the given index.
		inline String Insert(const int32& aAtIndex, const String& aStringToInsert) const
		{
			std::string newString = data;
			newString.insert(aAtIndex, aStringToInsert.GetCharPointer());
			return newString;
		}

		// Returns a string with the given string insterted starting at the given index.
		inline String Remove(const int32& aAtIndex, const int32& aCharactersToRemove) const
		{
			std::string newString = data;
			newString.erase(aAtIndex,aCharactersToRemove);
			return newString;
		}

		// Returns a string with all instances of the specified character removed.
		inline String Remove(const char& aChar) const
		{
			std::string newString = data;
			newString.erase(std::remove(newString.begin(), newString.end(), aChar), newString.end());
			return newString;
		}

		// Returns a string with all instances of the specified string removed.
		inline String Remove(const String& aString) const
		{
			return Replace(aString, "");
		}

		// Returns a string with all characters between the two indexes removed.
		inline String RemoveBetween(const size_t& aFirstIndex, const size_t& aSecondIndex) const
		{
			std::string newString = data;
			if (aFirstIndex < aSecondIndex)
				newString.erase(aFirstIndex, aSecondIndex - aFirstIndex);
			else if (aSecondIndex < aFirstIndex)
				newString.erase(aSecondIndex, aFirstIndex - aSecondIndex);
			return newString;
		}

		// Returns a string with all characters after the specified index removed.
		inline String RemoveAfter(const size_t& aIndex)
		{
			return RemoveBetween(aIndex + 1, GetLength());
		}

		// Returns a string with all characters before the specified index removed.
		inline String RemoveBefore(const size_t& aIndex)
		{
			return RemoveBetween(0, aIndex + 1);
		}

		// Returns a string that has all instances of all characters specified removed from the string.
		inline String RemoveCharacters(const char* aCharacters) const
		{
			std::string newString = data;
			for (unsigned int i = 0; i < strlen(aCharacters); ++i)
				newString.erase(std::remove(newString.begin(), newString.end(), aCharacters[i]), newString.end());
			return newString;
		}

		// Returns a string with leading whitespace removed.
		inline String TrimStart() const
		{
			std::string newString = data;
			newString.erase(newString.begin(), std::find_if(newString.begin(), newString.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
			return newString;
		}

		// Returns a string with trailing whitespace removed.
		inline String TrimEnd() const
		{
			std::string newString = data;
			newString.erase(std::find_if(newString.rbegin(), newString.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), newString.end());
			return newString;
		}

		// Returns a string with all leading and trailing whitespace removed.
		inline String Trim() const
		{
			return TrimStart().TrimEnd();
		}

		// Trims any of the specified characters from the start of the string
		inline String TrimStart(const char* aChars) const
		{
			std::string newString = data;
			size_t newStart = newString.find_first_not_of(aChars);
			newString.erase(0, newStart);
			return newString;
		}

		// Trims any of the specified character from the start of the string
		inline String TrimStart(const char& aChar) const { return TrimStart(&aChar); }

		// Trims any of the specified characters off the end of the string
		inline String TrimEnd(const char* aChars) const
		{
			std::string newString = data;
			size_t newLength = newString.find_last_not_of(aChars) + 1;
			newString.resize(newLength);
			return newString;
		}

		// Trims any of the specified character off the end of the string
		inline String TrimEnd(const char& aChar) const { return TrimEnd(&aChar); }

		// Trims any of the specified characters from the start and end of the string
		inline String Trim(const char* aChars) const
		{
			return TrimStart(aChars).TrimEnd(aChars);
		}

		// Trims any of the specified character from the start and end of the string
		inline String Trim(const char& aChar) const { return Trim(&aChar); }

		// Return a lowercase version of this string
		inline String ToLower() const
		{
			std::string newString = data;
			transform(newString.begin(), newString.end(), newString.begin(), ::tolower);
			return newString;
		}

		// Return an uppercase version of this string
		inline String ToUpper() const
		{
			std::string stdstring = data;
			transform(stdstring.begin(), stdstring.end(), stdstring.begin(), ::toupper);
			return stdstring;
		}
	};

	// Comparison to std::string
	inline bool operator == (const String& lhs, const std::string& rhs) { return lhs == (String)rhs; }
	inline bool operator == (const std::string& lhs, const String& rhs) { return rhs == (String)lhs; }

	// Addition
	inline String operator+ (const String& lhs, const String& rhs) { return lhs.Append(rhs); }
	inline String operator+ (const char* lhs, const String& rhs) { return String(lhs).Append(rhs); }
	inline String operator+ (const String& lhs, const char* rhs) { return lhs.Append(rhs); }

	// Comparison to char*
	inline bool operator == (const String& lhs, const char* rhs) { return lhs == String(rhs); }
	inline bool operator == (const char* lhs, const String& rhs) { return rhs == String(lhs); }

	// Comparison to juce::String
	inline bool operator == (const String& lhs, const juce::String& rhs) { return lhs == String(rhs); }
	inline bool operator == (const juce::String& lhs, const String& rhs) { return rhs == String(lhs); }

	// Implement osstream support
	inline ::std::ostream& operator<<(::std::ostream& os, const String& aString) { return os << aString.GetCharPointer(); }

}
