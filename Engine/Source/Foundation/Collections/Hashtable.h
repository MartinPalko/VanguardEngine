#pragma once
#include <unordered_map>

#include "Collections/ICollection.h"
#include "Types/VanguardString.h"
#include "Types/StringID.h"

namespace Vanguard
{
	template<class T>
	class Hashtable : public ICollection <T>
	{
	private:
		std::unordered_map < size_t, T > data;
	public:
        //Constructors
		Hashtable(){}
        
        Hashtable(const Hashtable& rhs):
            data(rhs.data)
        {
        }
        
        //Move semantics
        Hashtable(Hashtable&& rhs):
            data(std::move(rhs.data))
        {
        }

        //Destructors
        ~Hashtable()
        {
        }
        
		// If the hashtable contains an element with the given key.
		inline bool Contains(const uint64 aKey)
		{
			return data.count() > 0;
		}

		// Number of elements in this hashtable.
		inline size_t Count() const{ return data.size(); }

		inline T& operator[](const uint64 aKey) { return data[aKey]; }
		inline T& operator[](const StringID& aKey)  { return data[aKey.GetHash()]; }
        inline Hashtable& operator=(const Hashtable& rhs)
        {
            data = rhs.data;
            return *this;
        }
	};
}
