#pragma once

#include "Source/ThirdParty/ZLib/zlib-1.2.8/zlib.h"

#include "IntegerDefs.h"

namespace Vanguard
{
	class StringID
	{
	private:
		uint64 Hash;

		inline void HashString(const String& aString)
		{
			Hash = 0;
			Hash = crc32((uLong)Hash, (const Byte*)(aString.GetCharPointer()), (uLong)aString.GetLength());
		}
	public:
		inline bool operator == (const StringID& aOther) const { return Hash == aOther.Hash; }
		inline bool operator != (const StringID& aOther) const { return Hash != aOther.Hash; }
		inline bool operator < (const StringID& aOther) const { return Hash < aOther.Hash; }
		inline bool operator > (const StringID& aOther) const { return Hash > aOther.Hash; }

		StringID(const int& aHash)
		{
			Hash = aHash;
		}

		StringID(const String& aString)
		{
			HashString(aString);
		}

		StringID(const char* aCharPointer)
		{
            HashString(aCharPointer);
		}

		inline uint64 GetHash() const
		{
			return Hash;
		}

	};
}
