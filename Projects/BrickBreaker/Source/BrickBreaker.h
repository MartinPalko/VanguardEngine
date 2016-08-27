#pragma once
#include "BrickBreaker_Common.h"

namespace gainput
{
	class InputManager;
	class InputMap;
}

namespace BrickBreaker
{
	class BrickBreaker : public ProjectModule
	{
	public:
		virtual void LoadModule() override;
		virtual void UnloadModule() override;
		virtual World* CreateWorld() override;
	};
}