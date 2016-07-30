#include "VanguardString.h"

#include <string>
#include <cstring>
#include <functional>
#include <sstream>
#include <algorithm>
#include <cctype>

#define TRUE_STRING "true"
#define FALSE_STRING "false"

namespace Vanguard
{
	String::String() { data = new std::string(""); }
	String::String(const String& aString) { data = new std::string(*aString.data); }
	String::String(const char* aCharPointer) { data = new std::string(aCharPointer); }
	String::String(const char& aChar) { data = new std::string(1, aChar); }
	void String::operator = (const String& aOther) { data = new std::string(*aOther.data); }
	void String::operator += (const String& aOther) { data->assign(*Append(aOther).data); }
	String::~String() { delete data; }

	bool String::operator == (const String& aOther) const { return *data == *aOther.data; }
	bool String::operator != (const String& aOther) const { return *data != *aOther.data; }
	bool String::operator < (const String& aOther) const { return *data < *aOther.data; }
	bool String::operator <= (const String& aOther) const { return *data <= *aOther.data; }
	bool String::operator > (const String& aOther) const { return *data > *aOther.data; }
	bool String::operator >= (const String& aOther) const { return *data >= *aOther.data; }

	const char* String::GetCharPointer() const { return data->c_str(); }

	size_t String::NPos()
	{
		return std::string::npos;
	}

	char String::operator [] (size_t aIndex) const
	{
#if VANGUARD_DEBUG
		// Guard aginst out of range
		if (aIndex < 0 || aIndex >= GetLength())
			throw std::invalid_argument("String index out of range");
#endif
		return (*data)[aIndex];
	}

	String String::FromBoolean(const bool& aBool)
	{
		return aBool ? TRUE_STRING : FALSE_STRING;
	}

	bool String::ToBoolean() const
	{
		return this->ToLower() == TRUE_STRING;
	}

	// To and from int32
	String String::FromInt32(int32 aInt32)
	{
		return ::std::to_string(aInt32).c_str();
	}

	int32 String::ToInt32() const
	{
		if (*data == "")
			return 0;
		else
			return std::stoi(*data);
	}

	String String::FromFloat(float aFloat)
	{
		return String(::std::to_string(aFloat).c_str()).TrimEnd("0").TrimEnd(".");
	}

	float String::ToFloat() const
	{
		return std::stof(*data);
	}

	size_t String::GetLength() const
	{
		return data->length();
	}

	bool String::IsEmpty() const
	{
		return Trim().GetLength() == 0;
	}

	String String::Append(const String& aString) const
	{
		std::string newString = *data;
		newString.append(*aString.data);
		return newString.c_str();
	}

	DynamicArray<String> String::Split(const char& aSplitBy) const
	{
		DynamicArray<String> splitList = DynamicArray<String>();

		std::stringstream strStream(*data);
		std::string segment;

		while (std::getline(strStream, segment, aSplitBy))
		{
			splitList.PushBack(segment.c_str());
		}

		return splitList;
	}

	String String::Join(const DynamicArray<String>& aStringList, const char& aSeperatorCharacter)
	{
		if (aStringList.Count() <= 0)
			return String();

		String newString = aStringList[0];

		for (uint32 i = 1; i < aStringList.Count(); i++)
		{
			newString += ";";
			newString += aStringList[i];
		}
		return newString;
	}

	bool String::Contains(const String& aString) const
	{
		return !(data->find(aString.GetCharPointer()) == std::string::npos);
	}

	bool String::Contains(const char& aCharacter) const
	{
		return !(data->find(aCharacter) == std::string::npos);
	}

	bool String::ContainsAny(const String& aCharacters) const
	{
		for (size_t i = 0; i < aCharacters.GetLength(); i++)
		{
			if (!(data->find(aCharacters[i]) == std::string::npos))
				return true;
		}
		return false;
	}

	bool String::BeginsWith(const char& aCharacter) const
	{
		return FirstIndexOf(aCharacter) == 0;
	}

	bool String::BeginsWithAny(const String& aCharacters) const
	{
		return FirstIndexOfAny(aCharacters) == 0;
	}

	bool String::EndsWith(const char& aCharacter) const
	{
		return LastIndexOf(aCharacter) == GetLength() - 1;
	}

	bool String::EndsWithAny(const String& aCharacters) const
	{
		return LastIndexOfAny(aCharacters) == GetLength() - 1;
	}

	size_t String::FirstIndexOf(const char& aCharacter) const
	{
		return data->find_first_of(aCharacter);
	}

	size_t String::FirstIndexOfAny(const String& aCharacters) const
	{
		return data->find_first_of(aCharacters.GetCharPointer());
	}

	size_t String::LastIndexOf(const char& aCharacter) const
	{
		return data->find_last_of(aCharacter);
	}

	size_t String::LastIndexOfAny(const String& aCharacters) const
	{
		return data->find_last_of(aCharacters.GetCharPointer());
	}

	size_t String::FirstIndexNotOf(const char& aCharacter) const
	{
		return data->find_first_not_of(aCharacter);
	}

	size_t String::FirstIndexNotOfAny(const String& aCharacters) const
	{
		return data->find_first_not_of(aCharacters.GetCharPointer());
	}

	size_t String::LastIndexNotOf(const char& aCharacter) const
	{
		return data->find_last_not_of(aCharacter);
	}

	size_t String::LastIndexNotOfAny(const String& aCharacters) const
	{
		return data->find_last_not_of(aCharacters.GetCharPointer());
	}

	String String::Replace(const char& aChar, const char& aWithChar) const
	{
		std::string newString = *data;
		std::replace(newString.begin(), newString.end(), aChar, aWithChar);
		return newString.c_str();
	}

	String String::Replace(const String& aString, const String& aWithString) const
	{
		size_t start_pos = 0;
		std::string newString = *data;
		while ((start_pos = newString.find(aString.GetCharPointer(), start_pos)) != std::string::npos)
		{
			newString.replace(start_pos, aString.GetLength(), aWithString.GetCharPointer());
			start_pos += aWithString.GetLength(); // Handles case where 'aWithString' is a substring of 'aString'
		}
		return newString.c_str();
	}

	String String::Insert(const int32& aAtIndex, const String& aStringToInsert) const
	{
		std::string newString = *data;
		newString.insert(aAtIndex, aStringToInsert.GetCharPointer());
		return newString.c_str();
	}

	String String::Remove(const int32& aAtIndex, const int32& aCharactersToRemove) const
	{
		std::string newString = *data;
		newString.erase(aAtIndex, aCharactersToRemove);
		return newString.c_str();
	}

	String String::Remove(const char& aChar) const
	{
		std::string newString = *data;
		newString.erase(std::remove(newString.begin(), newString.end(), aChar), newString.end());
		return newString.c_str();
	}

	String String::Remove(const String& aString) const
	{
		return Replace(aString, "");
	}

	String String::RemoveBetween(const size_t& aFirstIndex, const size_t& aSecondIndex) const
	{
		std::string newString = *data;
		if (aFirstIndex < aSecondIndex)
			newString.erase(aFirstIndex, aSecondIndex - aFirstIndex);
		else if (aSecondIndex < aFirstIndex)
			newString.erase(aSecondIndex, aFirstIndex - aSecondIndex);
		return newString.c_str();
	}

	String String::RemoveAfter(const size_t& aIndex)
	{
		return RemoveBetween(aIndex + 1, GetLength());
	}

	String String::RemoveBefore(const size_t& aIndex)
	{
		return RemoveBetween(0, aIndex + 1);
	}

	String String::RemoveCharacters(const char* aCharacters) const
	{
		std::string newString = *data;
		for (unsigned int i = 0; i < strlen(aCharacters); ++i)
			newString.erase(std::remove(newString.begin(), newString.end(), aCharacters[i]), newString.end());
		return newString.c_str();
	}

	String String::TrimStart() const
	{
		std::string newString = *data;
		newString.erase(newString.begin(), std::find_if(newString.begin(), newString.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return newString.c_str();
	}

	String String::TrimEnd() const
	{
		std::string newString = *data;
		newString.erase(std::find_if(newString.rbegin(), newString.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), newString.end());
		return newString.c_str();
	}

	String String::Trim() const
	{
		return TrimStart().TrimEnd();
	}

	String String::TrimStart(const char* aChars) const
	{
		std::string newString = *data;
		size_t newStart = newString.find_first_not_of(aChars);
		newString.erase(0, newStart);
		return newString.c_str();
	}

	String String::TrimStart(const char& aChar) const { return TrimStart(&aChar); }

	String String::TrimEnd(const char* aChars) const
	{
		std::string newString = *data;
		size_t newLength = newString.find_last_not_of(aChars) + 1;
		newString.resize(newLength);
		return newString.c_str();
	}

	String String::TrimEnd(const char& aChar) const
	{
		return TrimEnd(&aChar);
	}

	String String::Trim(const char* aChars) const
	{
		return TrimStart(aChars).TrimEnd(aChars);
	}

	String String::Trim(const char& aChar) const
	{
		return Trim(&aChar);
	}

	String String::ToLower() const
	{
		std::string newString = *data;
		transform(newString.begin(), newString.end(), newString.begin(), ::tolower);
		return newString.c_str();
	}

	String String::ToUpper() const
	{
		std::string stdstring = *data;
		transform(stdstring.begin(), stdstring.end(), stdstring.begin(), ::toupper);
		return stdstring.c_str();
	}

	// Implement osstream support
	::std::ostream& operator<<(::std::ostream& os, const String& aString) { return os << aString.GetCharPointer(); }

}
