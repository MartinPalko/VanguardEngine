#include "BrickBreaker.h"
#include "BrickBreakerWorld.h"

namespace BrickBreaker
{
	VANGUARD_DECLARE_MODULE(BrickBreaker);

	void BrickBreaker::LoadModule()
	{

	}

	void BrickBreaker::UnloadModule()
	{

	}

	World* BrickBreaker::CreateWorld()
	{
		return new BrickBreakerWorld();
	}
}