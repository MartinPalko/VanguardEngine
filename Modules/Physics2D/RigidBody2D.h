#pragma once
#include "Physics2D_Common.h"

class b2Body;

namespace Vanguard
{
	class PHYSICS2D_API RigidBody2D : public Component
	{
		friend class PhysicsWorld2D;
		TYPE_DECLARATION(RigidBody2D, Component)

	protected:
		b2Body* box2DBody;

	public:
		RigidBody2D();
		virtual ~RigidBody2D();
	};
}