#include "BrickBreaker_Common.h"

namespace Vanguard
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
		float movementRange;
		float paddleWidth;

	public:
		Paddle();

		virtual void Tick(Frame* aFrame) override;
		void SetInput(float aInput) { input = aInput; }
	};
}