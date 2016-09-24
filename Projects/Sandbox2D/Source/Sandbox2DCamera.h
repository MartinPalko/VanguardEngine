#pragma once
#include "Sandbox2D_Common.h"

namespace Sandbox2D
{
	class Sandbox2DCamera : public Camera
	{
		TYPE_DECLARATION(Sandbox2DCamera, Camera)
	protected:
		Vector3 input;
		Vector3 velocity;
		float drag;
		float brakingFriction;
		float movementForce;
		float zoomSpeed;

	public:
		enum ECollision
		{
			None,
			Top,
			Bottom,
			Left,
			Right
		};

		Sandbox2DCamera();

		virtual void Tick(Frame* aFrame) override;

		void SetInput(Vector3 aInput) { input = aInput; }
	};
}