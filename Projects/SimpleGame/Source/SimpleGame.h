#pragma once
#include "SimpleGame_Common.h"
#include "Modules/IModule.h"

#include "SpriteComponent.h"

namespace SimpleGame
{
	class SimpleGame : public IModule
	{
		SpriteComponent* sprite;

		virtual String GetModuleName() { return "SimpleGame_Native"; }
		virtual String GetModuleType() { return "Project"; }
		
		virtual void LoadModule()
		{
			DEBUG_LOG("Loaded Simple Game");
			sprite = new SpriteComponent();
		}

		virtual void UnloadModule()
		{
			DEBUG_LOG("Unloaded Simple Game");
			delete sprite;
		}
	};
}