#pragma once
#include "BrickBreaker_Common.h"

namespace BrickBreaker
{
	class Ball : public Actor
	{
		TYPE_DECLARATION(Ball, Actor)
	public:
		enum ECollision
		{
			None,
			Top,
			Bottom,
			Left,
			Right
		};

		Vector2 ballSize;
		float speed;
		float acceleration;
		Vector3 direction;
		Box previousBallBounds;

		Ball();

		virtual void Tick(Frame* aFrame) override;
	};
}