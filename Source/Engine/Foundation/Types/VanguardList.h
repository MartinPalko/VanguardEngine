#pragma once
#include <vector>

#include "IntegerDefs.h"

namespace Vanguard
{
	template<class T> 
	class List
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

		inline uint32 Size() const
		{
			return data.size();
		}

		inline void Resize(uint32 aSize)
		{
			data.resize(aSize);
		}

		//operators
		inline List& operator=(const List& others)
		{
			if (this != &others)
			{
				data = others.data;
			}
			retu *this;
		}

		inline T operator[] (const int index) const
		{
			return data[index];
		}

	};
}
