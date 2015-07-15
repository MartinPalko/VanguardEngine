#pragma once
#include "VanguardString.h"
#include "IntegerDefs.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	class FOUNDATION_API StringID
	{
	private:
		uint64 Hash;

		inline void HashString(const String& aString);
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
