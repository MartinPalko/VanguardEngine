#pragma once
#include <map>
namespace Vanguard
{
	template<class K,class V> class Dictionary
	{
	private:
		std::map<K,V> map = std::map<K,V>();

	public:
		virtual ~Dictionary(){}

		V Get(K aKey)
		{
			return map[aKey];
		}

		void Set(K aKey, V aValue)
		{
			map[aKey] = aValue;
		}

		void Remove(K aKey)
		{
			map.erase(aKey);
		}

		void RemoveByValue(V aValue)
		{
			map.erase(aValue);
		}
	};
}
