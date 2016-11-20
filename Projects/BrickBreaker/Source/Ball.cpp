#include "Brick.h"
#include "Paddle.h"
#include "BrickBreakerWorld.h"
#include "SpriteRenderer.h"
#include "Ball.h"

namespace BrickBreaker
{
	TYPE_DEFINITION(Ball, Actor)

	START_REGISTER_PROPERTIES(Ball);
	{
		REGISTER_PROPERTY(Ball, speed);
		REGISTER_PROPERTY(Ball, acceleration);
		REGISTER_PROPERTY(Ball, direction);
	}
	FINISH_REGISTER_PROPERTIES(Ball);

	Ball::Ball() : Actor()
		, ballSize(1.0f, 1.0f)
		, speed(40)
		, acceleration(1)
		, direction(Vector3(1.0f ,1.0f ,0.0f))
		, previousBallBounds(Vector3(0.0f, 0.0f, 0.0f))
	{
		SpriteRenderer* spriteComponent = AddComponent<SpriteRenderer>();
		spriteComponent->SetDimensions(Vector2(2.0f, 2.0f));
		spriteComponent->SetColor(Color(200, 200, 200));
		spriteComponent->SetImage("ball.png");
		spriteComponent->SetBlendMode(eSpriteBlendMode::Blend);
	}

	void Ball::Tick(Frame* aFrame)
	{
		// Collide with edges
		const Vector2 maxPos = (BrickBreakerWorld::PlayAreaSize - ballSize) / 2;
		const Vector2 minPos = -maxPos;

		Vector3 position = GetTransform()->GetPosition();

		if (position.x > maxPos.x)
			direction.x = -1;
		else if (position.x < minPos.x)
			direction.x = 1;

		if (position.y > maxPos.y)
			direction.y = -1;
		else if (position.y < minPos.y)
			direction.y = 1;

		Box ballBounds = GetLocalBounds();
		ballBounds.min += position;
		ballBounds.max += position;

		// Collide with paddle
		Paddle* paddle = GetWorld()->GetInstances<Paddle>()[0];
		Box paddleBounds = paddle->GetLocalBounds();
		paddleBounds.min += paddle->GetTransform()->GetPosition();
		paddleBounds.max += paddle->GetTransform()->GetPosition();

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
			brickBounds.min += brick->GetTransform()->GetPosition();
			brickBounds.max += brick->GetTransform()->GetPosition();

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

				brick->Destroy();

				if (auto brickWorld = Type::SafeCast<BrickBreakerWorld>(GetWorld()))
				{
					brickWorld->IncrementScore();
				}

			}
		}

		// Move
		speed += acceleration * aFrame->GetDeltaTime().InSeconds();
		Vector3 velocity = direction.Normalize() * speed;
		position += velocity * aFrame->GetDeltaTime().InSeconds();

		GetTransform()->SetPosition(position);
		previousBallBounds = ballBounds;
	}
}