#pragma once
#include <map>

#include "ICollection.h"
#include "List.h"
#include "IntegerDefs.h"

namespace Vanguard
{
	template<typename K, typename V>
	class Dictionary : public ICollection<V>
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

		inline void Set(const K& aKey, const V& aValue)
		{
			data[aKey] = aValue;
		}

		inline void Remove(const K& aKey)
		{
			if(data.count(aKey) == 1)
			{
				data.erase(aKey);
			}
		}

		inline bool Contains(const K& aKey) const
		{
			return data.count(aKey) == 1;
		}

		inline bool ContainsKey(const K& aKey) const
		{
			return Contains(aKey);
		}

		inline bool ContainsValue(const V& aValue) const
		{
			Vanguard::List<V> values = Values();
			return values.Contains(aValue);
		}

		inline void Clear()
		{
			data.clear();
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

		inline List<V> Values() const{
			List<V> values = List<V>();
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

		inline V& operator[](const K& aKey)
		{
			return data[aKey];
		}
	};
}
