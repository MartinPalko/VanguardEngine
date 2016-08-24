#pragma once
#include "BrickBreaker_Common.h"
#include "Modules/GameModule.h"
#include "Renderer/RenderView.h"

#include "Paddle.h"
#include "Brick.h"
#include "Ball.h"

namespace gainput
{
	class InputManager;
	class InputMap;
}

namespace BrickBreaker
{
	class BrickBreaker : public GameModule, public INativeEventHandler
	{
	public:
		const static Vector2 PlayAreaSize;
	private:
		WindowHandle gameWindow;
		RenderView* playerView;
		Paddle* paddle;
		Ball* ball;

		gainput::InputManager* inputManager;
		gainput::InputMap* inputMap;
		
		// Implement GameModule
	protected:
		virtual void SetupGame() override;
		virtual void UpdateGame(Frame* aFrame) override;
		virtual void CleanupGame() override;

		// Implement INativeEventHandler
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) override;
	};
}