#pragma once
#include <map>

#include "VanguardList.h"
#include "IntegerDefs.h"

namespace Vanguard
{
	template<typename K, typename V>
	class Hashtable
	{
	private:
		std::map<K, V> data;

	public:
		//default constructor
		Hashtable() = default;

		//Copy constructor
		Hashtable(const Hashtable& rhs){
			data = rhs.data;
		}

		//destrcutor
		~Hashtable() = default;

		//methods 

		inline void Add(const K& aKey, const V& aValue){
			data.emplace(aKey, aValue);
		}

		inline void Remove(const K& aKey){
			if(data.count(aKey) == 1){
				data.erase(aKey);
			}
		}

		inline uint32 Count() const{
			return data.size();
		}

		inline List<K> Keys() const{
			List<K> keys = List<K>();
			for (auto& item : data){
				keys.PushBack(item.first);
			}
			return keys;
		}

		inline List<K> Values() const{
			List<K> values = List<K>();
			for (auto& item : data){
				values.PushBack(item.second);
			}
			return values;
		}

		// operators
		inline Hashtable& operator=(const Hashtable& other){
			if (this != &other){
				data = other.data;
			}
			return *this;
		}
	};
}
