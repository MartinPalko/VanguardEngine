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
		inline void PushBack(T aItem)
		{
			data.push_back(aItem);
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

		inline void Remove(T aItem)
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

		//operators
		inline DynamicArray& operator=(const DynamicArray& others)
		{
			if (this != &others)
			{
				data = others.data;
			}
			return *this;
		}

		inline T operator[] (const size_t index) const
		{
			return data[index];
		}

	};
}
