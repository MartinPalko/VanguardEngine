#include "MathFunctions.h"
#include <random>

namespace Vanguard
{
	namespace Math
	{
		std::minstd_rand random(0);

		float FRandom()
		{
			return (float)random() / (float)random.max();
		}

		int32 IRandom()
		{
			return random();
		}

		void SeedRandom(int aSeed)
		{
			return random.seed(aSeed);
		}
	}
}
