#pragma once

#include "Collections/DynamicArray.h"
#include "IntegerDefs.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	class FOUNDATION_API String
	{
	private:
		std::string* data;

	public:
		String();
		String(const String& aString);
		String(const char* aCharPointer);
		String(const char& aChar);
		void operator = (const String& aOther);
		void operator += (const String& aOther);
		~String();

		// Comparison to other vanguard::string
		bool operator == (const String& aOther) const;
		bool operator != (const String& aOther) const;
		bool operator < (const String& aOther) const;
		bool operator <= (const String& aOther) const;
		bool operator > (const String& aOther) const;
		bool operator >= (const String& aOther) const;

		const char* GetCharPointer() const;				

		// Gets the value that is return when no position is found (for functions that check position)
		static size_t NPos();

		// Access character by index
		char operator[](size_t aIndex) const;
		// To and from boolean
		static String FromBoolean(const bool& aBool);
		bool ToBoolean() const;

		// To and from int32
		static String FromInt(int32 aInt);
		static String FromInt(int64 aInt);
		static String FromInt(uint32 aInt);
		static String FromInt(uint64 aInt);
		int32 ToInt32() const;

		// From size
		static String FromSize(size_t aSize);

		// To and from float
		static String FromFloat(float aFloat);
		float ToFloat() const;

		// Returns the number of characters in this string
		size_t GetLength() const;

		bool IsEmpty() const;

		// Returns a string with the specified string appended to the end of it.
		String Append(const String& aString) const;

		// Split the string into a list of strings by the specified character.
		DynamicArray<String> Split(const char& aSplitBy) const;

		// Joins an array of strings into a single string, with the specified seperator between each element.
		static String Join(const DynamicArray<String>& aStringList, const char& aSeperatorCharacter);

		// Returns true if the string contains at least one instance of the specified substring
		bool Contains(const String& aString) const;

		// Returns true if the string contains at least one instance of the specified character
		bool Contains(const char& aCharacter) const;

		// Returns true if the string contains at least one instance of any of the specified characters
		bool ContainsAny(const String& aCharacters) const;

		// Returns true if the first character of the string is the specified character
		bool BeginsWith(const char& aCharacter) const;

		// Returns true if the first character of the string is any of the specified characters
		bool BeginsWithAny(const String& aCharacters) const;

		// Returrns true if the last character of the string is the specified character
		bool EndsWith(const char& aCharacter) const;

		// Returns true if the first character of the string is any of the specified characters
		bool EndsWithAny(const String& aCharacters) const;

		// Returns the index of the first of the specified character
		size_t FirstIndexOf(const char& aCharacter) const;

		// Returns the index of the first of any of the specified characters
		size_t FirstIndexOfAny(const String& aCharacters) const;

		// Returns the index of the last of the specified character
		size_t LastIndexOf(const char& aCharacter) const;

		// Returns the index of the last of any of the specified characters
		size_t LastIndexOfAny(const String& aCharacters) const;

		// Returns the index of the first character not to match the specified character
		size_t FirstIndexNotOf(const char& aCharacter) const;

		// Returns the index of the first character not to match the and of the specified characters
		size_t FirstIndexNotOfAny(const String& aCharacters) const;

		// Returns the index of the last character not to match the specified character
		size_t LastIndexNotOf(const char& aCharacter) const;

		// Returns the index of the last character not to match the and of the specified characters
		size_t LastIndexNotOfAny(const String& aCharacters) const;

		// Returns a string with all instances of a specified character withplaced with another.
		String Replace(const char& aChar, const char& aWithChar) const;

		// Returns a string with all instances of a specified string withplaced with another.
		// If possible, use the overloaded version that takes a character, as it's much faster.
		String Replace(const String& aString, const String& aWithString) const;

		// Returns a string with the given string insterted starting at the given index.
		String Insert(const int32& aAtIndex, const String& aStringToInsert) const;

		// Returns a string with the given string insterted starting at the given index.
		String Remove(const int32& aAtIndex, const int32& aCharactersToRemove) const;

		// Returns a string with all instances of the specified character removed.
		String Remove(const char& aChar) const;

		// Returns a string with all instances of the specified string removed.
		String Remove(const String& aString) const;

		// Returns a string with all characters between the two indexes removed.
		String RemoveBetween(const size_t& aFirstIndex, const size_t& aSecondIndex) const;

		// Returns a string with all characters after the specified index removed.
		String RemoveAfter(const size_t& aIndex);

		// Returns a string with all characters before the specified index removed.
		String RemoveBefore(const size_t& aIndex);

		// Returns a string that has all instances of all characters specified removed from the string.
		String RemoveCharacters(const char* aCharacters) const;

		// Returns a string with leading whitespace removed.
		String TrimStart() const;

		// Returns a string with trailing whitespace removed.
		String TrimEnd() const;

		// Returns a string with all leading and trailing whitespace removed.
		String Trim() const;

		// Trims any of the specified characters from the start of the string
		String TrimStart(const char* aChars) const;

		// Trims any of the specified character from the start of the string
		String TrimStart(const char& aChar) const;

		// Trims any of the specified characters off the end of the string
		String TrimEnd(const char* aChars) const;

		// Trims any of the specified character off the end of the string
		String TrimEnd(const char& aChar) const;

		// Trims any of the specified characters from the start and end of the string
		String Trim(const char* aChars) const;

		// Trims any of the specified character from the start and end of the string
		String Trim(const char& aChar) const;

		// Return a lowercase version of this string
		String ToLower() const;

		// Return an uppercase version of this string
		String ToUpper() const;
	};

	// Addition
	inline String operator+ (const String& lhs, const String& rhs) { return lhs.Append(rhs); }
	inline String operator+ (const char* lhs, const String& rhs) { return String(lhs).Append(rhs); }
	inline String operator+ (const String& lhs, const char* rhs) { return lhs.Append(rhs); }

	// Comparison to char*
	inline bool operator == (const String& lhs, const char* rhs) { return lhs == String(rhs); }
	inline bool operator == (const char* lhs, const String& rhs) { return rhs == String(lhs); }
}
