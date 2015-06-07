#pragma once

#include "ThirdParty/ZLib/zlib-1.2.8/zlib.h"

#include "IntegerDefs.h"

namespace Vanguard
{
	class StringID
	{
	private:
		uint32 Hash;

		inline void HashString(const String& aString)
		{
			Hash = 0;
			Hash = crc32(Hash, (const Byte*)((const char*)aString), aString.GetLength());
		}
	public:
		inline bool operator == (const StringID& aOther) const { return Hash == aOther.Hash; }
		inline bool operator != (const StringID& aOther) const { return Hash != aOther.Hash; }
		inline bool operator < (const StringID& aOther) const { return Hash < aOther.Hash; }
		inline bool operator > (const StringID& aOther) const { return Hash > aOther.Hash; }

		StringID(const String& aString)
		{
			HashString(aString);
		}

		StringID(const char* aCharPointer)
		{
            HashString(aCharPointer);
		}

		uint32 GetHash()
		{
			return Hash;
		}

	};
}
