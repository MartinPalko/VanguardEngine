#pragma once

#include "Physics2D_Common.h"
#include "RigidBody2D.h"
#include "ISubsystem.h"

class b2World;

namespace Vanguard
{
	class PhysicsWorld2D : protected IWorldEventListener, protected ISubsystem, protected IDebugDraw
	{
	private:
		Timespan timeStep;
		int32 velocityIterations;
		int32 positionIterations;
		float gravity;

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

		// ISubsystem Implementation
		virtual bool NeedsService(Timespan aCurrentTime, Timespan aLastServiced, Timespan& outEstNextServiceTime) override;
		virtual void ServiceSubsystem(Timespan aCurrentTime) override;

		// IDebugDraw Implementation
		virtual void DebugDraw(IDebugCanvas* aCanvas) override;
	};
}