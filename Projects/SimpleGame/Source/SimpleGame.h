#pragma once
#include "SimpleGame_Common.h"
#include "Modules\GameModule.h"

#include "SpriteComponent.h"

namespace SimpleGame
{
	class SimpleGame : public GameModule
	{
	private:
		//SpriteComponent* sprite;
		
	protected:
		virtual void SetupGame() override;
		virtual void UpdateGame(Frame* aFrame) override;
		virtual void CleanupGame() override;
	};
}