#pragma once

#include "Physics2D_Common.h"
#include "RigidBody2D.h"

class b2World;

namespace Vanguard
{
	class PhysicsWorld2D : protected IWorldEventListener
	{
	protected:
		b2World* box2DWorld;
		World* vanguardWorld;

	public:
		PhysicsWorld2D(World* aWorld);
		virtual ~PhysicsWorld2D();

	protected:
		void RegisterRigidbody(RigidBody2D* aRigidBody);
		void UnregisterRigidbody(RigidBody2D* aRigidBody);

		// IWorldEventListener Implementation
		virtual void OnWorldEvent(WorldEvent* aEvent) override;
	};
}