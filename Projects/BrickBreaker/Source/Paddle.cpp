#include "Paddle.h"
#include "SpriteComponent.h"

namespace Vanguard
{
	TYPE_DEFINITION(Paddle, Actor)

	Paddle::Paddle() : Actor()
		, input(0)
		, velocity(0)
		, drag(4)
		, mass(1)
		, movementForce(15)
	{
		SpriteComponent* spriteComponent = AddComponent<SpriteComponent>();
		spriteComponent->SetDimensions(Vector2(0.1f, 0.1f));
		spriteComponent->SetColor(Color(255, 120, 120, 255));

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
		GetComponent<Transform>()->position.x += velocity * aFrame->deltaTime.InSeconds();

		// Detect collision with sides
		if (GetTransform()->position.x > 1 && velocity > 0)
		{
			GetTransform()->position.x = 1;
			velocity *= -0.6;
		}
		else if (GetTransform()->position.x < 0 && velocity < 0)
		{
			GetTransform()->position.x = 0;
			velocity *= -0.6;
		}
	}
}