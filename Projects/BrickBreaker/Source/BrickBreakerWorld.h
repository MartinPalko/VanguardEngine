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
		TYPE_DECLARATION(BrickBreakerWorld, World)

	public:
		const static Vector2 PlayAreaSize;
	private:
		Camera* playerCamera;
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
		virtual RenderView* GetPrimaryRenderView() override { return playerView; }


		// Implement INativeEventHandler
	public:
		virtual void HandleNativeEvent(NativeEvent aEvent) override;
	};
}