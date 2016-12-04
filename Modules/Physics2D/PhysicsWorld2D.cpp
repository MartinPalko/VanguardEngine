#include "Box2D/Box2D.h"

#include "PhysicsWorld2D.h"
#include "Collider2D.h"

#include "B2DrawImplementation.h"

namespace Vanguard
{
	PhysicsWorld2D::PhysicsWorld2D(World* aWorld)
		: timeStep(1.0 / 50.0)
		, velocityIterations(8)
		, positionIterations(3)
		, gravity (-9.8f)
	{
		vanguardWorld = aWorld;

		box2DWorld = new b2World(b2Vec2(0.0f, gravity));

		vanguardWorld->RegisterEventListener(this);
		Core::GetInstance()->RegisterSubsystem(this);

		for (RigidBody2D* rigidBody : vanguardWorld->GetInstances<RigidBody2D>())
		{
			//RegisterRigidbody(rigidBody);
		}

		Core::GetInstance()->GetPrimaryRenderer()->RegisterDebugDraw(this);

		// TEMP
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -70.0f);
		b2Body* groundBody = box2DWorld->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(80.0f, 10.0f);
		groundBody->CreateFixture(&groundBox, 0.0f);
	}

	PhysicsWorld2D::~PhysicsWorld2D()
	{
		Core::GetInstance()->GetPrimaryRenderer()->UnregisterDebugDraw(this);

		for (RigidBody2D* rigidBody : vanguardWorld->GetInstances<RigidBody2D>())
		{
			UnregisterRigidbody(rigidBody);
		}

		Core::GetInstance()->UnregisterSubsystem(this);
		vanguardWorld->UnregisterEventListener(this);

		delete box2DWorld;
	}

	void PhysicsWorld2D::RegisterRigidbody(RigidBody2D* aRigidBody)
	{
		Transform* rbTransform = aRigidBody->GetEntity()->GetComponent<Transform>();
		Vector2 rbPos = rbTransform->GetPosition2D();
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(rbPos.x, rbPos.y);
		bodyDef.angle = -rbTransform->GetRotation2D();

		aRigidBody->box2DBody = box2DWorld->CreateBody(&bodyDef);

		Collider2D* collider = aRigidBody->GetEntity()->GetComponent<Collider2D>();

		aRigidBody->box2DBody->CreateFixture(collider->GetBox2DShape(), 1.0f);
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
			// Update transform's position from rigidbody's movement
			for (RigidBody2D* rigidBody : vanguardWorld->GetInstances<RigidBody2D>())
			{
				Transform* transform = rigidBody->GetEntity()->GetComponent<Transform>();

				b2Vec2 position = rigidBody->box2DBody->GetPosition();
				transform->SetPosition2D(Vector2(position.x, position.y));
				transform->SetRotation2D(-rigidBody->box2DBody->GetAngle());
			}
		}
		else if (auto postTickEvent = Type::SafeCast<PostTickEvent>(aEvent))
		{
			// TODO: Apply any transform modifications to the b2Body
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

	bool PhysicsWorld2D::NeedsService(Timespan aCurrentTime, Timespan aLastServiced, Timespan& outEstNextServiceTime)
	{
		outEstNextServiceTime = aLastServiced + timeStep;
		double currentTime = aCurrentTime.InSeconds();
		double lastServiced = aLastServiced.InSeconds();
		double nextService = outEstNextServiceTime.InSeconds();

		return outEstNextServiceTime <= aCurrentTime;
	}

	void PhysicsWorld2D::ServiceSubsystem(Timespan aCurrentTime)
	{
		box2DWorld->Step(timeStep.InSeconds(), velocityIterations, positionIterations);
	}

	void PhysicsWorld2D::DebugDraw(IDebugCanvas* aCanvas)
	{
		B2DrawImplementation b2Drawer(aCanvas);

		b2Drawer.SetFlags(b2Draw::e_shapeBit || b2Draw::e_jointBit || b2Draw::e_centerOfMassBit);

		box2DWorld->SetDebugDraw(&b2Drawer);
		box2DWorld->DrawDebugData();
		box2DWorld->SetDebugDraw(nullptr);
	}
}
