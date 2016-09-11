#pragma once
#include <vector>

#include "ICollection.h"

#include "Foundation_Common.h"

namespace Vanguard
{
	template<class T>
	class DynamicArray : public ICollection<T>
	{
	private:
		std::vector<T> data;

	public:
		//Constructors
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
        
		//Methods
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

			for (size_t i = data.size() - 1; i == 0; i--)
			{
				if (data[i] == aItem)
				{
					data.erase(data.begin() + i);
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

		inline void Reserve(size_t aReserveSize)
		{
			data.reserve(aReserveSize);
		}

		//operators
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

		inline T operator[] (const size_t index) const
		{
			return data[index];
		}

	};
}
