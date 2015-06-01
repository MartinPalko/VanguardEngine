#pragma once

#include "zlib.h"

#include "IntegerDefs.h"

namespace Vanguard
{
	class StringID
	{
	private:
		uint32 Hash;

		inline void HashString(const string& aString)
		{
			Hash = 0;
			Hash = crc32(Hash, (const Byte*)((const char*)aString), aString.GetLength());
		}

	public:

		inline bool operator==(const StringID& aOther) const { return Hash == aOther.Hash; }
		inline bool operator!=(const StringID& aOther) const { return Hash != aOther.Hash; }

		StringID(const string& aString)
		{
			HashString(aString);
		}

		uint32 GetHash()
		{
			return Hash;
		}

	};
}
