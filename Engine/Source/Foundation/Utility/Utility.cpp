#include "Utility.h"

#include <limits>


float Vanguard::LowestFloat()
{
	return std::numeric_limits<float>::lowest();
}

float Vanguard::MinFloat()
{
	return std::numeric_limits<float>::min();
}

float Vanguard::MaxFloat()
{
	return std::numeric_limits<float>::max();
}
