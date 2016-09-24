#pragma once
#include "Sandbox2D_Common.h"

namespace Sandbox2D
{
	class Sandbox2D : public ProjectModule
	{
	public:
		virtual void LoadModule() override;
		virtual void UnloadModule() override;
		virtual World* CreateWorld() override;
	};
}