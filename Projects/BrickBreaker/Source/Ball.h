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
		Vector3 direction;

		Ball();

		virtual void Tick(Frame* aFrame) override;		
		ECollision CheckCollision(Actor* aOther);
	};
}