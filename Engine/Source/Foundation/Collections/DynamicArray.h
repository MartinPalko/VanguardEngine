#pragma once
#include <vector>

#include "Foundation_Common.h"

namespace Vanguard
{
	template<class T>
	class DynamicArray
	{
	private:
		std::vector<T> data;

	public:
		DynamicArray() = default;
		DynamicArray(size_t aReserveSize) { data.reserve(aReserveSize); }
		DynamicArray(const DynamicArray& rhs)
		{
			data = rhs.data;
		}

        DynamicArray(DynamicArray&& rhs) :
            data(std::move(rhs.data))
        {
        }
        
        ~DynamicArray()
        {
        }

		class iterator
		{
		private:
			DynamicArray* const dynamicArray;
			size_t currentIndex;
		public:
			iterator(DynamicArray* const aDynamicArray, size_t aStartIndex = 0) 
				: dynamicArray(aDynamicArray)
				, currentIndex(aStartIndex)
			{}
			~iterator() {}
			iterator& operator++() { currentIndex++; return *this;}
			bool operator !=(const iterator& aOther) { return currentIndex != aOther.currentIndex || dynamicArray != aOther.dynamicArray; }
			T operator*() { return dynamicArray->At(currentIndex);}
		};

		iterator begin()
		{
			return iterator(this, 0);
		}
        
		iterator end()
		{
			return iterator(this, Count());
		}

		inline void PushBack(const T& aItem)
		{
			data.push_back(aItem);
		}

		inline void Concatenate(const DynamicArray& other)
		{
			data.insert( data.end(), other.data.begin(), other.data.end());
		}

		inline void PopBack()
		{
			data.pop_back();
		}

		inline T Back()
		{
			return data.back();
		}

        inline size_t Count() const{ return data.size(); }
        
		inline void Resize(uint32 aSize)
		{
			data.resize(aSize);
		}

		inline void Clear()
		{
			data.clear();
		}

		inline void Remove(const T& aItem)
		{
			if (!data.size())
				return;

			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i] == aItem)
				{
					data.erase(data.begin() + i);
					i--;
				}
			}
		}

		inline void RemoveAt(size_t aIndex)
		{
			data.erase(data.begin() + aIndex);
		}

		inline bool Contains(const T& aItem) const
		{
			for (typename std::vector<T>::const_iterator it = data.begin(); it != data.end(); it++)
			{
				if (*it == aItem)
				{
					return true;
				}
			}
			return false;
		}

		inline size_t IndexOf(const T& aItem) const
		{
			return find(data.begin(), data.end(), aItem) - data.begin();
		}

		inline void Reserve(size_t aReserveSize)
		{
			data.reserve(aReserveSize);
		}

		inline T At(size_t aIndex) const
		{
			return data[aIndex];
		}

		inline DynamicArray& operator=(const DynamicArray& other)
		{
			if (this != &other)
			{
				data = other.data;
			}
			return *this;
		}

		inline void operator+=(const DynamicArray& other)
		{
			Concatenate(other);
		}

		inline T operator[] (const size_t aIndex) const
		{
			return data[aIndex];
		}

	};
}
