#pragma once
#include <vector>

#include "ICollection.h"
#include "IntegerDefs.h"

namespace Vanguard
{
	template<class T>
	class List : public ICollection<T>
	{
	private:
		std::vector<T> data;

	public:
		//Constructors
		List() = default;
		List(const List& rhs)
		{
			data = rhs.data;
		}

		//Methods
		inline void PushBack(T aItem)
		{
			data.push_back(aItem);
		}

		inline size_t Size() const
		{
			return data.size();
		}

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
			for (int i = data.size() - 1; i >= 0; i--)
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
		inline List& operator=(const List& others)
		{
			if (this != &others)
			{
				data = others.data;
			}
			return *this;
		}

		inline T operator[] (const int index) const
		{
			return data[index];
		}

	};
}