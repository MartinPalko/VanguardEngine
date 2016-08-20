#pragma once
#include "SimpleGame_Common.h"
#include "Modules/GameModule.h"

#include "SpriteComponent.h"

#include "Renderer/RenderView.h"

namespace SimpleGame
{
	class SimpleGame : public GameModule
	{
	private:
		RenderView* playerView;
		Entity* paddle;
		
	protected:
		virtual void SetupGame() override;
		virtual void UpdateGame(Frame* aFrame) override;
		virtual void CleanupGame() override;
	};
}