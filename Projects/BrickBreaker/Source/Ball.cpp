#include "Ball.h"
#include "BrickBreaker.h"
#include "SpriteComponent.h"

namespace BrickBreaker
{
	TYPE_DEFINITION(Ball, Actor)

	Ball::Ball() : Actor()
		, ballSize(1.f, 1.f)
		, speed(40)
		, acceleration(1)
		, direction(Vector3(1,1,0))
		, previousBallBounds(Vector3(0.0f, 0.0f, 0.0f))
	{
		SpriteComponent* spriteComponent = AddComponent<SpriteComponent>();
		spriteComponent->SetDimensions(Vector2(2.0f, 2.0f));
		spriteComponent->SetColor(Color(200, 200, 200));
	}

	void Ball::Tick(Frame* aFrame)
	{
		// Collide with edges
		const Vector2 maxPos = (BrickBreaker::PlayAreaSize - ballSize) / 2;
		const Vector2 minPos = -maxPos;

		if (GetTransform()->position.x > maxPos.x)
			direction.x = -1;
		else if (GetTransform()->position.x < minPos.x)
			direction.x = 1;

		if (GetTransform()->position.y > maxPos.y)
			direction.y = -1;
		else if (GetTransform()->position.y < minPos.y)
			direction.y = 1;

		Box ballBounds = GetLocalBounds();
		ballBounds.min += GetTransform()->position;
		ballBounds.max += GetTransform()->position;

		// Collide with paddle
		Paddle* paddle = GetWorld()->GetInstances<Paddle>()[0];
		Box paddleBounds = paddle->GetLocalBounds();
		paddleBounds.min += paddle->GetTransform()->position;
		paddleBounds.max += paddle->GetTransform()->position;

		if (ballBounds.Intersects(paddleBounds))
		{
			direction.y = 1;
		}

		// Collide with bricks
		DynamicArray<Brick*> bricks = GetWorld()->GetInstances<Brick>();
		for (size_t i = 0; i < bricks.Count(); i++)
		{
			Brick* brick = bricks[i];

			if (!brick->Enabled())
				continue;

			Box brickBounds = brick->GetLocalBounds();
			brickBounds.min += brick->GetTransform()->position;
			brickBounds.max += brick->GetTransform()->position;

			if (ballBounds.Intersects(brickBounds))
			{
				// Bounce
				if (previousBallBounds.max.x < brickBounds.min.x && ballBounds.max.x >= brickBounds.min.x)
					direction.x = -1; // Right of ball
				else if (previousBallBounds.min.x > brickBounds.max.x && ballBounds.min.x <= brickBounds.max.x)
					direction.x = 1; // Left of ball
				
				if (previousBallBounds.max.y < brickBounds.min.y && ballBounds.max.y >= brickBounds.min.y)
					direction.y = -1; // Top of ball
				else if (previousBallBounds.min.y > brickBounds.max.y && ballBounds.min.y <= brickBounds.max.y)
					direction.y = 1; // Bottom of ball

				// TODO: Disable/destroy brick
				brick->Disable();
			}
		}

		// Move
		speed += acceleration * aFrame->deltaTime.InSeconds();
		Vector3 velocity = direction.Normalize() * speed;
		GetTransform()->position += velocity * aFrame->deltaTime.InSeconds();

		previousBallBounds = ballBounds;
	}
}