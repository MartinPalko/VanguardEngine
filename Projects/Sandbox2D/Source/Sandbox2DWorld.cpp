#include "gainput/gainput.h"

#include "Foundation_Common.h"

#include "Jobs/Frame.h"
#include "Sandbox2D.h"
#include "WorldObjects/Transform.h"
#include "SpriteRenderer.h"
#include "Sandbox2DWorld.h"
#include "Sandbox2DCamera.h"

#include "RigidBody2D.h"
#include "RectangleCollider.h"

namespace eGameButton
{
	enum eType
	{
		Exit = 0,
		ProfileFrame,
		CameraMoveLeft,
		CameraMoveRight,
		CameraMoveUp,
		CameraMoveDown,
		CameraZoomIn,
		CameraZoomOut,
		CameraRotateLeft,
		CameraRotateRight
	};
}

namespace Sandbox2D
{
	TYPE_DEFINITION(Sandbox2DWorld, World);

	Sandbox2DWorld::Sandbox2DWorld() : World("Brick Breaker")
	{
		Application::RegisterNativeEventHandler(this);

		// Create a world and a renderview.
		Core* core = Core::GetInstance();
		playerCamera = SpawnEntity<Sandbox2DCamera>();
		playerCamera->SetFov(100);
		playerCamera->SetOrthographic(true);
		playerCamera->GetTransform()->SetPosition(Vector3(12, 5, 0));

		IRenderer* primaryRenderer = core->GetPrimaryRenderer();
		playerView = core->GetPrimaryRenderer()->CreateRenderView(playerCamera);

		// Background sprite
		Actor* backgroundEntity = SpawnEntity<Actor>();
		SpriteRenderer* backgroundSprite = backgroundEntity->AddComponent<SpriteRenderer>();
		backgroundSprite->SetDimensions(Vector2(1000, 1000));
		backgroundSprite->SetColor(Color(100, 149, 237));

		Math::SeedRandom(DateAndTime::CurrentTimeMillis());

		// TestSprites
		Vector3 testSpriteLocations[] = {
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0),
			Vector3(Math::FRandomRange(-50.0f, 50.0f), Math::FRandomRange(-50.0f, 50.0f), 0)
		};

		for (int i = 0; i < sizeof(testSpriteLocations) / sizeof(Vector3); i++)
		{
			Actor* testSpriteActor = SpawnEntity<Actor>();
			testSpriteActor->GetTransform()->SetPosition(testSpriteLocations[i]);
			SpriteRenderer* testSprite = testSpriteActor->AddComponent<SpriteRenderer>();
			testSprite->SetDimensions(Vector2(10, 10));
			testSprite->SetImage("Sprite.png");
			testSprite->SetBlendMode(Vanguard::eSpriteBlendMode::Blend);
			RigidBody2D* testRigidBody = testSpriteActor->AddComponent<RigidBody2D>();
			RectangleCollider* testRectCollider = testSpriteActor->AddComponent<RectangleCollider>();
			testRectCollider->SetDimensions(Vector2(3,5));
		}	

		// Setup Gainput
		inputManager = new gainput::InputManager(false);
		gainput::DeviceId keyboardId = inputManager->CreateDevice<gainput::InputDeviceKeyboard>();

		inputMap = new gainput::InputMap(*inputManager);
		inputMap->MapBool(eGameButton::Exit, keyboardId, gainput::KeyEscape);
		inputMap->MapBool(eGameButton::ProfileFrame, keyboardId, gainput::KeyP);
		inputMap->MapBool(eGameButton::CameraMoveLeft, keyboardId, gainput::KeyLeft);
		inputMap->MapBool(eGameButton::CameraMoveLeft, keyboardId, gainput::KeyA);
		inputMap->MapBool(eGameButton::CameraMoveRight, keyboardId, gainput::KeyRight);
		inputMap->MapBool(eGameButton::CameraMoveRight, keyboardId, gainput::KeyD);
		inputMap->MapBool(eGameButton::CameraMoveUp, keyboardId, gainput::KeyUp);
		inputMap->MapBool(eGameButton::CameraMoveUp, keyboardId, gainput::KeyW);
		inputMap->MapBool(eGameButton::CameraMoveDown, keyboardId, gainput::KeyDown);
		inputMap->MapBool(eGameButton::CameraMoveDown, keyboardId, gainput::KeyS);
		inputMap->MapBool(eGameButton::CameraZoomIn, keyboardId, gainput::KeyE);
		inputMap->MapBool(eGameButton::CameraZoomOut, keyboardId, gainput::KeyQ);
		inputMap->MapBool(eGameButton::CameraRotateLeft, keyboardId, gainput::KeyR);
		inputMap->MapBool(eGameButton::CameraRotateRight, keyboardId, gainput::KeyT);

		// Register debug draw
		Core::GetInstance()->GetPrimaryRenderer()->RegisterDebugDraw(this);
	}

	Sandbox2DWorld::~Sandbox2DWorld()
	{
		Core::GetInstance()->GetPrimaryRenderer()->UnregisterDebugDraw(this);

		Application::UnregisterNativeEventHandler(this);
		delete inputMap;
		delete inputManager;
	}

	void Sandbox2DWorld::Tick(Vanguard::Frame* aFrame)
	{
		// Update Gainput
		inputManager->Update(aFrame->GetDeltaTime().InMilliseconds());

		if (inputMap->GetBool(eGameButton::Exit))
		{
			Core::GetInstance()->Quit();
		}

		if (inputMap->GetBoolIsNew(eGameButton::ProfileFrame))
			Core::GetInstance()->GetProfiler()->ProfileNextFrame();

		Vector3 input(0, 0, 0);
		if (inputMap->GetBool(eGameButton::CameraMoveLeft))
			input.x -= 1;
		if (inputMap->GetBool(eGameButton::CameraMoveRight))
			input.x += 1;
		if (inputMap->GetBool(eGameButton::CameraMoveUp))
			input.y += 1;
		if (inputMap->GetBool(eGameButton::CameraMoveDown))
			input.y -= 1;
		if (inputMap->GetBool(eGameButton::CameraZoomIn))
			input.z += 1;
		if (inputMap->GetBool(eGameButton::CameraZoomOut))
			input.z -= 1;

		if (inputMap->GetBool(eGameButton::CameraRotateLeft))
			playerCamera->GetTransform()->SetRotation2D(playerCamera->GetTransform()->GetRotation2D() + 0.2f * aFrame->GetDeltaTime().InSeconds());
		if (inputMap->GetBool(eGameButton::CameraRotateRight))
			playerCamera->GetTransform()->SetRotation2D(playerCamera->GetTransform()->GetRotation2D() - 0.2f * aFrame->GetDeltaTime().InSeconds());

		playerCamera->SetInput(input);

		// Super's tick AFTER we update input
		World::Tick(aFrame);
	}
	
	void Sandbox2DWorld::HandleNativeEvent(NativeEvent aEvent)
	{
		#if defined(VANGUARD_WINDOWS)
			// Forward any input messages to Gainput
			inputManager->HandleMessage((MSG*)aEvent);
		#elif defined(VANGUARD_LINUX)
			inputManager->HandleEvent((XEvent*)aEvent);
		#endif
	}

	void Sandbox2DWorld::DebugDraw(IDebugCanvas* aCanvas)
	{
		//aCanvas->DrawCircle(Vector2(100, 100), 50, Color(255,0,255), false);

		//aCanvas->DrawPoint(Vector2(200, 100), 10, Color(255,0,255));

		//aCanvas->DrawLine(Vector2(300, 100), Vector2(400, 120), Color(255,0,255));

		//DynamicArray<Vector2> polygonVerts;
		//polygonVerts.PushBack(Vector2(500, 100));
		//polygonVerts.PushBack(Vector2(600, 100));
		//polygonVerts.PushBack(Vector2(600, 200));
		//polygonVerts.PushBack(Vector2(500, 200));
		//aCanvas->DrawPolygon(polygonVerts, Color(255,0,0), false);
	}
}