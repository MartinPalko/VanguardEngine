#include "Sandbox2D.h"
#include "Sandbox2DWorld.h"

namespace Sandbox2D
{
	VANGUARD_DECLARE_MODULE(Sandbox2D);

	void Sandbox2D::LoadModule()
	{

	}

	void Sandbox2D::UnloadModule()
	{

	}

	World* Sandbox2D::CreateWorld()
	{
		return new Sandbox2DWorld();
	}
}