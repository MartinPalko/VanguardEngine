#include "Box2D/Box2D.h"

#include "PhysicsWorld2D.h"
#include "Collider2D.h"

namespace Vanguard
{
	PhysicsWorld2D::PhysicsWorld2D(World* aWorld)
	{
		vanguardWorld = aWorld;

		b2Vec2 gravity(0.0f, -9.8f);
		box2DWorld = new b2World(gravity);

		vanguardWorld->RegisterEventListener(this);
	}

	PhysicsWorld2D::~PhysicsWorld2D()
	{
		vanguardWorld->UnregisterEventListener(this);
	}

	void PhysicsWorld2D::RegisterRigidbody(RigidBody2D* aRigidBody)
	{
		b2BodyDef bodyDef;

		Transform* rbTransform = aRigidBody->GetEntity()->GetComponent<Transform>();
		Vector2 rbPos = rbTransform->GetPosition2D();

		bodyDef.position.Set(rbPos.x, rbPos.y);
		bodyDef.angle = rbTransform->GetRotation2D();

		b2Body* body = box2DWorld->CreateBody(&bodyDef);

		Collider2D* collider = aRigidBody->GetEntity()->GetComponent<Collider2D>();

		body->CreateFixture(collider->GetBox2DShape(), 0);
	}

	void PhysicsWorld2D::UnregisterRigidbody(RigidBody2D* aRigidBody)
	{
		b2Body* body = aRigidBody->box2DBody;
		aRigidBody->box2DBody = nullptr;
		box2DWorld->DestroyBody(body);
	}

	void PhysicsWorld2D::OnWorldEvent(WorldEvent* aEvent)
	{
		if (auto preTickEvent = Type::SafeCast<PreTickEvent>(aEvent))
		{
			
		}
		else if (auto postTickEvent = Type::SafeCast<PostTickEvent>(aEvent))
		{

		}
		else if (auto objectAddedEvent = Type::SafeCast<ObjectAddedEvent>(aEvent))
		{
			if (auto rigidBody = Type::SafeCast<RigidBody2D>(objectAddedEvent->GetObject()))
			{
				RegisterRigidbody(rigidBody);
			}			
		}
		else if (auto objectRemovedEvent = Type::SafeCast<ObjectRemovedEvent>(aEvent))
		{
			if (auto rigidBody = Type::SafeCast<RigidBody2D>(objectAddedEvent->GetObject()))
			{
				UnregisterRigidbody(rigidBody);
			}	
		}
	}
}
