#pragma once
#include "EmptyProject_Common.h"

namespace EmptyProject
{
	class EmptyProject : public ProjectModule
	{
	public:
		virtual void LoadModule() override;
		virtual void UnloadModule() override;
		virtual World* CreateWorld() override;
	};
}