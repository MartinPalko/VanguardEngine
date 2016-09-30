#pragma once
#include <vector>

#include "Foundation_Common.h"

namespace Vanguard
{
	template<class T>
	class CircularBuffer
	{
	private:
		T* data;
		size_t dataSize;
		size_t used;
		size_t firstIndex;

	public:
		CircularBuffer() = delete;
		CircularBuffer(size_t aSize) 
		{ 
			dataSize = aSize;
			data = new T[aSize];
			used = 0;
			firstIndex = 0;
		}

		CircularBuffer(const CircularBuffer& rhs)
		{
			data = new T*[rhs.dataSize];
			memcpy(data, rhs.data, sizeof(T) * rhs.dataSize);
			dataSize = rhs.dataSize;
			used = rhs.used;
			firstIndex = rhs.firstIndex;
		}
        
        ~CircularBuffer()
        {
			delete[] data;
        }

		inline void PushBack(T aItem)
		{
			if (used < dataSize)
			{
				data[used] = aItem;
				used++;
			}
			else
			{
				data[firstIndex] = aItem;
				firstIndex = (firstIndex + 1) % dataSize;
			}
		}

		inline size_t Count() const { return used; }

		inline void Resize(size_t aSize)
		{
			T* newData = new T[aSize];

			// Copy from old data
			for (int i = 0; i < used && i < aSize; i++)
			{
				newData[i] = this->operator[](i);
			}
			used = Math::Min(used, aSize);
			firstIndex = 0;
			dataSize = aSize;

			delete[] data;
			data = newData;
		}

		inline bool Contains(const T& aItem) const
		{
			for (size_t i = 0; i < used; i++)
			{
				if (this->operator[](i) == aItem)
				{
					return true;
				}
			}
			return false;
		}

		inline size_t IndexOf(const T& aItem) const
		{
			for (size_t i = 0; i < used; i++)
			{
				if (this->operator[](i) == aItem)
				{
					return i;
				}
			}
			return -1;
		}

		inline T operator[] (const size_t index) const
		{
			return data[(index + firstIndex) % used];
		}

	};
}
