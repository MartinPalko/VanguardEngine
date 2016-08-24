#pragma once
#include "BrickBreaker_Common.h"

namespace BrickBreaker
{
	class Paddle : public Actor
	{
		TYPE_DECLARATION(Paddle, Actor)

	protected:
		float input;
		float velocity;
		float drag;
		float mass;
		float movementForce;
		float bounciness;
		float paddleWidth;

	public:
		Paddle();

		virtual void Tick(Frame* aFrame) override;
		void SetInput(float aInput) { input = aInput; }
	};
}