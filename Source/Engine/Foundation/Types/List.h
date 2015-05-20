#pragma once
#include <vector>
namespace Vanguard
{
	template<class T> class List : public std::vector<T>
	{
	public:
		virtual ~List(){}
	};
}
