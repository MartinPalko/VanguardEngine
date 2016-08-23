#pragma once
#include "VanguardString.h"
#include "Foundation_Common.h"

namespace Vanguard
{
	class FOUNDATION_API StringID
	{
	private:
		size_t Hash;

		void HashString(const String& aString);
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

		inline size_t GetHash() const
		{
			return Hash;
		}

	};
}
