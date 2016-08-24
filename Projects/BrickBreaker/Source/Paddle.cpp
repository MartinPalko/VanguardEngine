#include "Paddle.h"
#include "SpriteComponent.h"
#include "BrickBreaker.h"

namespace BrickBreaker
{
	TYPE_DEFINITION(Paddle, Actor)

	Paddle::Paddle() : Actor()
		, input(0)
		, velocity(0)
		, drag(4.0f)
		, mass(1.0f)
		, movementForce(800.0f)
		, bounciness(0.4f)
		, paddleWidth(20.0f)
	{
		SpriteComponent* spriteComponent = AddComponent<SpriteComponent>();
		spriteComponent->SetDimensions(Vector2(paddleWidth, 2.0f));
		spriteComponent->SetColor(Color(150, 150, 150));

		EnableTick();
	}

	void Paddle::Tick(Frame* aFrame)
	{
		float normalizedInput = Math::Clamp(input, -1.0f, 1.0f);
		
		// Apply movement force
		float acceleration = (normalizedInput * movementForce) / mass;
		velocity += acceleration * aFrame->deltaTime.InSeconds();

		// Apply drag
		velocity *= Math::Clamp(1.0f - (drag * (float)aFrame->deltaTime.InSeconds()), 0.0f, 1.0f);

		// Move by velocity
		GetTransform()->position.x += velocity * aFrame->deltaTime.InSeconds();

		const float maxX = (BrickBreaker::PlayAreaSize.x - paddleWidth) / 2;
		const float minX = -maxX;

		// Detect collision with sides
		if (GetTransform()->position.x > maxX && velocity > 0)
		{
			GetTransform()->position.x = maxX;
			velocity *= -bounciness;
		}
		else if (GetTransform()->position.x < minX && velocity < 0)
		{
			GetTransform()->position.x = minX;
			velocity *= -bounciness;
		}
	}
}