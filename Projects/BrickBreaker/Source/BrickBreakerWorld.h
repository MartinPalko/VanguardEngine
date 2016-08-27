#pragma once
#include "BrickBreaker_Common.h"
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
	class BrickBreakerWorld : public World, public INativeEventHandler
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

	public:
		BrickBreakerWorld();
		~BrickBreakerWorld();

		// Override World
	protected:
		virtual void Tick(Frame* aFrame) override;

		// Implement INativeEventHandler
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) override;
	};
}