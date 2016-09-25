#include "Sandbox2DCamera.h"

namespace Sandbox2D
{
	TYPE_DEFINITION(Sandbox2DCamera, Camera)

	Sandbox2DCamera::Sandbox2DCamera() : Camera()
		, input(0.0f, 0.0f, 0.0f)
		, velocity(0.0f, 0.0f, 0.0f)
		, drag(4.0f)
		, brakingFriction(500.0f)
		, movementForce(800.0f)
		, zoomSpeed(1.0f)
	{
		EnableTick();
	}

	void Sandbox2DCamera::Tick(Frame* aFrame)
	{
		//const Vector2 normalizedInput = input.Normalize() * Math::Min(1.0f, input.Length());
		const Vector3 normalizedInput = input;
		const float deltaSeconds = (float)aFrame->deltaTime.InSeconds();

		// Apply movement force
		Vector3 acceleration = (normalizedInput * movementForce);
		velocity += acceleration * aFrame->deltaTime.InSeconds();

		// Apply drag
		velocity *= Math::Clamp(1.0f - (drag * deltaSeconds), 0.0f, 1.0f);

		// Apply braking friction
		if (input == Vector3::Zero && velocity != Vector3::Zero)
		{
			velocity = velocity.Normalize() * (Math::Max(0.0f, velocity.Length() - brakingFriction * deltaSeconds));
		}

		// Move by velocity
		GetTransform()->position += velocity * deltaSeconds * (fov / 100);
		fov -= velocity.z * aFrame->deltaTime.InSeconds() * (fov / 100) * zoomSpeed;
	}
}