#pragma once
#include <cstring>
#include "VanguardString.h"
#include "Foundation_Common.h"
#include "Utility/Crc32.h"

namespace Vanguard
{
	class FOUNDATION_API StringID
	{
		friend struct std::hash<Vanguard::StringID>;

	private:
		uint32 hash;

	public:
		inline bool operator == (const StringID& aOther) const { return hash == aOther.hash; }
		inline bool operator != (const StringID& aOther) const { return hash != aOther.hash; }
		inline bool operator < (const StringID& aOther) const { return hash < aOther.hash; }
		inline bool operator > (const StringID& aOther) const { return hash > aOther.hash; }

		operator uint32 () const { return hash; }

		inline StringID(uint32 aHash) 
			: hash(aHash)
		{
		}

		inline StringID(const String& aString)
			: hash(Crc32::CalcCrc(aString.GetCharPointer(), aString.GetLength()))
		{
		}

		inline StringID(const char* aCharPointer)
			: hash (Crc32::CalcCrc(aCharPointer, strlen(aCharPointer)))
		{
		}

		inline size_t GetHash() const
		{
			return hash;
		}

	};
}

namespace std
{
	template <> 
	struct hash<Vanguard::StringID>
	{
		size_t operator()(Vanguard::StringID const& x) const noexcept
		{
			return x.hash;
		}
	};
}