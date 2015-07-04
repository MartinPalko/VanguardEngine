#pragma once
#include <unordered_map>

#include "ICollection.h"
#include "IntegerDefs.h"
#include "VanguardString.h"
#include "StringID.h"

namespace Vanguard
{
	template<class T>
	class Hashtable : public ICollection <T>
	{
	private:
		std::unordered_map < uint64, T > data;
	public:
		Hashtable(){}

		// If the hashtable contains an element with the given key.
		inline bool Contains(const uint64 aKey)
		{
			return data.count() > 0;
		}

		// Number of elements in this hashtable.
		inline size_t Count()
		{
			return data.size();
		}

		inline T& operator[](const uint64 aKey) { return data[aKey]; }
		inline T& operator[](const StringID& aKey)  { return data[aKey.GetHash()]; }
	};
}
