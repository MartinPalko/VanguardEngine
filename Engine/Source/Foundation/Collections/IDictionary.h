#pragma once

#include "ICollection.h"

namespace Vanguard
{
	template<typename K, typename V>
    class IDictionary : public ICollection<std::tuple<K, V>>
	{
    public:
        //virtual V& Item(const K& aKey)           = 0;
        //virtual ICollection<K> Keys()      const = 0;
        //virtual ICollection<V> Values()    const = 0;
	};
}