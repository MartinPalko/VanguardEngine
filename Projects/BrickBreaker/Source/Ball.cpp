#include "Ball.h"
#include "BrickBreaker.h"
#include "SpriteComponent.h"

namespace BrickBreaker
{
	TYPE_DEFINITION(Ball, Actor)

	Ball::Ball() : Actor()
		, ballSize(1.f, 1.f)
		, speed(40)
		, direction(Vector3(1,1,0))
	{
		SpriteComponent* spriteComponent = AddComponent<SpriteComponent>();
		spriteComponent->SetDimensions(Vector2(2.0f, 2.0f));
		spriteComponent->SetColor(Color(200, 200, 200));
	}

	void Ball::Tick(Frame* aFrame)
	{
		// Move
		Vector3 velocity = direction.Normalize() * speed;
		GetTransform()->position += velocity * aFrame->deltaTime.InSeconds();

		// Collide with edges
		const Vector2 maxPos = (BrickBreaker::PlayAreaSize - ballSize) / 2;
		const Vector2 minPos = -maxPos;

		if (GetTransform()->position.x > maxPos.x)
		{
			direction.x = -1;
		}
		else if (GetTransform()->position.x < minPos.x)
		{
			direction.x = 1;
		}

		if (GetTransform()->position.y > maxPos.y)
		{
			direction.y = -1;
		}
		else if (GetTransform()->position.y < minPos.y)
		{
			direction.y = 1;
		}

	}

	Ball::ECollision Ball::CheckCollision(Actor* aOther)
	{
		return ECollision();
	}
}