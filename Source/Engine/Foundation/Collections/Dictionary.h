#pragma once
#include <map>

#include "IDictionary.h"
#include "DynamicArray.h"
#include "IntegerDefs.h"

namespace Vanguard
{
	template<typename K, typename V>
	class Dictionary : public IDictionary<K,V>
	{
	private:
		std::map<K, V> data;

	public:
		//default constructor
		Dictionary() = default;

		//Copy constructor
		Dictionary(const Dictionary& rhs){
			data = rhs.data;
		}
        
        //Move semantics C++ 11
        Dictionary(Dictionary&& rhs) :
            data( std::move(rhs.data))
        {
        }

		//destrcutor
		~Dictionary() = default;
        
        
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

		//inline bool ContainsValue(const V& aValue) const
		//{
		//	Vanguard::DynamicArray<V> values = Values();
		//	return values.Contains(aValue);
		//}

		inline void Clear()
		{
			data.clear();
		}

		inline size_t Count() const
        {
			return data.size();
		}

        inline V& Item(const K& aKey)
        {
            return data[aKey];
        }
        
		//inline ICollection<K> Keys() const
  //      {
		//	DynamicArray<K> keys = DynamicArray<K>();
		//	for (auto& item : data){
		//		keys.PushBack(item.first);
		//	}
		//	return keys;
		//}

		//inline ICollection<V> Values() const
  //      {
		//	DynamicArray<V> values = DynamicArray<V>();
		//	for (auto& item : data){
		//		values.PushBack(item.second);
		//	}
		//	return values;
		//}

		// operators
		inline Dictionary& operator=(const Dictionary& other)
        {
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
